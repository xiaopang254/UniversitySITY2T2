#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define TIME_SLOTS  11   // 2048 msec

#define PIN_1   22    //pin ext PIR1
#define PIN_2   23    //pin ext PIR2

#define SCK     5     // GPIO5  -- SX1278's SCK
#define MISO    19    // GPIO19 -- SX1278's MISO
#define MOSI    27    // GPIO27 -- SX1278's MOSI
#define SS      10    // GPIO18 -- SX1278's CS
#define RST     7    // GPIO14 -- SX1278's RESET
#define DI0     2    // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    433E6
#define LED     2     // Blue LED

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//store the sensors state
int curState1, curState2, state;
int lastState1, lastState2;
bool pastState1 = LOW;
bool pastState2 = LOW;
bool Presence = LOW;

unsigned int counter = 0;

String packetString;    // received message for display
String message;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mesh Nodes Parameters
byte myNode = 2;    // my node number
byte sensorNode;    // sensors node number
byte relayNode;     // relaying node number

unsigned long universalTime = 0xFFFFFC00; // to take reference to synchronise with lowest node number, start high so as to detect for rollover errors (otherwise every 49 days)
unsigned long timeOffset  = 0xFFFFFC00;   // offset from local millis counter - numbers set so if node 0, it does a refresh immediately
unsigned long timePIR1, timePIR2;

String radioString = "";

//Number of nodes in the mesh
#define nNodes  8

int nodeSensors[nNodes];              // Presence Sensor value
int nodeTTLs[nNodes];                 // TimeToLive value
int encNode[nNodes]={0};              // encrypted
unsigned long nodeTimestamps[nNodes]; // timestamps for each nodes
int lowestNode = 255; // 0 to 15, 255 is false, 0-15 is node last got a time sync signal from

void setup()
{
  randomSeed(myNode);       // initialise random seed to each node
  pinMode(LED, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address or 0x3D for
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
    {
        delay(1000);
    }
  }

  // Setup oled display
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner

  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  lastState1 = lastState2 = LOW;

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);   // while OLED is running, must set GPIO16 in high

  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.print(F("LoRa Mesh "));

  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  }
  
  Serial.println(F("init ok"));
  Serial.print(F("Mesh node ")); // debug output
  Serial.println(myNode);

  display.clearDisplay();
  display.println(F("Setup successful"));
  display.setCursor(0, 8);     // write at the second line
  display.print("Node ID: ");
  display.print(myNode);
  display.display();
  delay(2000);    // 2 sec
}

void loop()
{
  //get the sensors status
  if (myNode == 3 || myNode == 5 || myNode == 7)
  {
    curState1 = digitalRead(PIN_1); // use real data
    curState2 = digitalRead(PIN_2);

    if ((lastState1 == LOW) && curState1 == HIGH) {
      timePIR1 = ((unsigned long) millis());
      pastState1 = HIGH;
      //    Serial.println("Pass 1");
    }
    if ((lastState2 == LOW) && curState2 == HIGH) {
      timePIR2 = ((unsigned long) millis());
      pastState2 = HIGH;
      //    Serial.println("Pass 2");
    }
    if (lastState1 != curState1)
      lastState1 = curState1; // to detect change of state
    if (lastState2 != curState2)
      lastState2 = curState2; // to detect change of state

    if (pastState1 == HIGH && pastState2 == HIGH)
    {
      if (timePIR1 > timePIR2) {
        state = HIGH;
        counter++;
        Serial.println(F("Entering"));
      }
      else if (timePIR2 > timePIR1) {
        state = LOW;
        counter--;
        Serial.println(F("Exiting"));
      }
      pastState1 = pastState2 = LOW;
    }

    if (curState1 == LOW && curState2 == HIGH)
      Presence = HIGH;
    if (curState2 == LOW && curState1 == HIGH)
      Presence = HIGH;

    if (curState1 == LOW && curState2 == LOW)
    {
      if (Presence == HIGH) {
        Serial.println(F("Both Sensors Ready"));
        Presence = LOW;
      }
      pastState1 = pastState2 = LOW;
      // only send data when sensors are not active
      NodeTimeSlot();   // send data
    }
  }
  else
    NodeTimeSlot();   // send data

  int packetSize = LoRa.parsePacket();
  if (packetSize)     // receive data
    cbk(packetSize);

}

void cbk(int packetLength)
{
  byte c;
  packetString = "";
  for (int i = 0; i < packetLength; i++)
  {
    c = (char) LoRa.read();
    processRadioString(c); // is it a command?
  }
}


void processRadioString(char c) // converts a byte to a char, so can work with the number
{
  if ((c > 31) && (c < 127)) // only send printable characters and 13,, takes 2 bytes to send a value, but can use 13 as an end of line marker
  {
    radioString += c; // add to the string
  }
  if (c == 13) // process the command if there is a carriage return
  {
    Serial.print(F("|"));
    if (radioString.startsWith("Time="))
      processTimeMessage();
    else if (radioString.startsWith("Data="))
      processDataMessage();
    
    packetString = radioString.substring(5); // received data messages
    Serial.println(packetString);
    radioString = "";
  }
}

void processTimeMessage() // pass Time=NN TTTTTTTT where NN is the node this came from
{
  String s;
  unsigned long remoteTime;
  unsigned long mytime;
  int messageFrom;
  s = stringMid(radioString, 6, 2);
  s = "000000" + s;
  messageFrom = (int) hexToUlong(s);
  s = radioString;
  s = stringMid(s, 9, 8);

  if ((messageFrom <= lowestNode) && (myNode != 0)) // current time slot is less or equal than the last node, update sequence number and universal time
  {
    mytime = (unsigned long) millis();
    remoteTime = hexToUlong(s);       // convert back to number
    remoteTime = remoteTime + 250;    // offset to allow for delay with transmission, determined by trial and error, if slow, then add more
    timeOffset = remoteTime - mytime; // update new offset value to sychronize with universalTime
    lowestNode = messageFrom; // update with the new current lowest node number
  }
}

String ulongToHex(unsigned long n) // convert an unsigned long number to a hex string 8 characters long
{
  int i;
  byte x;
  String s = "";
  for (i = 0; i < 8; i++) {
    x = n & 0x0000000F;      // mask
    if (x < 10) {
      s = (char)(x + 48) + s; // 0 to 9
    } else {
      s = (char)(x + 55) + s; // A to F
    }
    n = n >> 4;              // bit shift right
  }
  return s;
}

unsigned long hexToUlong(String s)
{
  unsigned long n = 0;        // return value
  int i;                      // general counter
  byte a;                     // byte at the position in the string
  unsigned long x = 1;        // multiply by this number
  for (i = 7; i >= 0; i--) {  // end to the start of the string
    a = (char) s.charAt(i);   // get the character and convert to ascii value
    if (a < 65) {             // if less than 'A'
      n = n + ((a - 48) * x); // convert the number
    } else {
      n = n + ((a - 55) * x); // convert the letter
    }
    x = x << 4;               // multiply by 16
  }
  return n;
}

void synchronizeTime()  // synchronize time with reference to time from lowest node number
{
  universalTime = ((unsigned long) millis()) + timeOffset;
}

void transmitTime()     // send my local time and converge on the lowest node number's time
{
  String nodeNumber;
  synchronizeTime();        // get my current universalTime count
  nodeNumber = stringMid(ulongToHex(myNode), 7, 2);
  String t = "Time=";
  t = t + nodeNumber;         // hex value
  t = t + "-";
  t = t + ulongToHex(universalTime);  // NN TTTTTTTT

  // send packet
  LoRa.beginPacket();
  LoRa.println(t);
  LoRa.endPacket();
}

String stringLeft(String s, int i) // stringLeft("mystring",2) returns "my"
{
  String t;
  t = s.substring(0, i);
  return t;
}

String stringMid(String s, int i, int j) // stringmid("mystring",4,3) returns "tri" (index is 1, not zero)
{
  String t;
  t = s.substring(i - 1, i + j - 1);
  return t;
}

// Data=0312AAAABBBBTTTTTTTTcrlf 
// where 03 is relay from, 12 is node (hex), AAAA is data, BBBB is the hops & TTTTTTTT is timestamp
void processDataMessage() 
{
  String s;
  unsigned long node;
  unsigned long valueSensor;
  unsigned long hopCount;
  unsigned long timestamp;
  unsigned long from;
  unsigned long age;
  unsigned long previousage;

  String receivedMessage;
  display.setCursor(0, 16);
  display.setTextColor(WHITE, BLACK); //Overwrite the screen
  display.print(F("Recv Data Message"));
  display.display();

  if (radioString.length() == 25)
	receivedMessage = radioString;  // process unencrypted field
  else
	return; // do not process for others (ie encrypted data)

  s = "000000" + stringMid(receivedMessage, 6, 2); // relay node (source node)
  from = hexToUlong(s);
  // store the relay node in 'sourceNode'
  String sourceNode = stringMid(receivedMessage, 6, 2);

  if(radioString.length() == 49)  // identify and differentiate encypted packet for processing
    encNode[from] = 1;
  else
    encNode[from] = 0;
  
  s = "000000" + stringMid(receivedMessage, 8, 2); // destination node of data
  node = hexToUlong(s);
  // store the node ID in 'destinationNode'
  String destinationNode = stringMid(receivedMessage, 8, 2);

  s = "0000" + stringMid(receivedMessage, 10, 4); // get the 4 bytes sensor value
  valueSensor = hexToUlong(s);

  // display in serial
  Serial.println("Destination Node: " + destinationNode);
  Serial.println("Source Node: " + sourceNode);

  // check if destination node is current node id which is node 2
  // and source node is previous node id which is 1
  if (destinationNode == "02" && sourceNode == "01"){
    Serial.println("Message received is: " + stringMid(receivedMessage, 10, 4));

    display.setCursor(0, 0);  // write at the first line
    display.setTextColor(WHITE, BLACK);   // overwrite the screen
    // source node ID which is suppose to be node 1
    display.print("Source ID: " + stringMid(receivedMessage, 6, 2) + "   ");

    display.setCursor(0, 24); // write at the fourth line
    display.setTextColor(WHITE, BLACK);   // overwrite the screen
    display.print("Data: " + stringMid(receivedMessage, 10, 4));  // AAAA data

    display.display();

    // store the message received from node 1
    message = stringMid(receivedMessage, 10, 4);
  }
  
  s = "0000" + stringMid(receivedMessage, 14, 4); // get the 4 bytes hopcount value
  hopCount = hexToUlong(s);

  s = stringMid(receivedMessage, 18, 8);          // get 8 bytes of timestamp value
  timestamp = hexToUlong(s);

  nodeTimestamps[node] = timestamp;               // update the time stamp
  if (valueSensor > 0x7FFF)                       // negative values
    valueSensor -= 0x10000;
  
  nodeSensors[node] = (int) valueSensor;          // update the sensor value
  nodeTTLs[node] = (int) ++hopCount;              // update hop count

  if (nodeTTLs[node] > 8) {                       // 8 hops or 1 full cycles time (~16 sec)
    nodeSensors[node] = 0;                        // reset sensor value to 0
    nodeTTLs[node] = 0;                           // reset to zero
  }
  relayNode = from;                               // data relay from node
  sensorNode = node;                              // data sent from node
}

void createDataMessage() // read A0 and A1 create data string
{
  String s;
  byte i;
  unsigned long u;
  String myNodeString;
  String buildMessage;

  updateMyValue();                              // update my sensor values
  Serial.print(F("My counter = "));
  Serial.print((String) nodeSensors[myNode]);
  Serial.print(F(","));
  Serial.println((String) nodeTTLs[myNode]);

  u = (unsigned long) myNode;
  s = ulongToHex(u);
  myNodeString = stringMid(s, 7, 2);            // from me
  delay(200);                                   // for any previous message to go through

  //iterate through XX nodes
  for (i = 0; i < nNodes; i++) {
    buildMessage = myNodeString;        // start building a string
    u = (unsigned long) i;              // 0 to 7 - 2 bytes for the actual node number
    s = ulongToHex(u);
    buildMessage += stringMid(s, 7, 2);
    u = (unsigned long) nodeSensors[i];
    s = ulongToHex(u);
    // buildMessage += stringMid(s, 5, 4); // data value in hex for sensor value

    buildMessage += message;    // AAAA data

    u = (unsigned long) nodeTTLs[i];
    s = ulongToHex(u);
    buildMessage += stringMid(s, 5, 4); // data value in hex for timestamp
    s = ulongToHex(nodeTimestamps[i]);  // timestamp value for this node
    buildMessage += s;// add this

    String sendMessage;
    sendMessage = "Data=" + buildMessage;

    // send packet
    LoRa.beginPacket();
    LoRa.println(sendMessage);
    LoRa.endPacket();
    Serial.println(sendMessage);        // print sent Data

    display.setCursor(0, 16);           // write at the third line
    display.setTextColor(WHITE, BLACK); // overwrite the screen 
    display.print(F("Sent Data Message"));
    display.display();

    packetString = ""; // clear received data messages
    delay(200); // errors on 75, ok on 100
  }
}

void outputSensors() // output all mesh values in one long hex string into aaaabbbbccccdddd etc where a is node 0 value 0, b is node 0 value 1, c is node 1 value 0
{
  String buildMessage;
  byte i;
  String s;
  unsigned long u;
  buildMessage = "";
  for (i = 0; i < nNodes; i++) {
    u = (unsigned long) nodeSensors[i];
    s = ulongToHex(u);
    buildMessage += stringMid(s, 5, 4); // data value in hex for Sensor Value
    u = (unsigned long) nodeTTLs[i];
    s = ulongToHex(u);
    buildMessage += stringMid(s, 5, 4); // data value in hex for TTL
  }
  Serial.println(buildMessage);
}

void updateMyValue()
{
  synchronizeTime(); // get the current universalTime count

  if (myNode == 3 || myNode == 5 || myNode == 7)  
    nodeSensors[myNode] = counter;        // get real sensor count
  else
    nodeSensors[myNode] += random(-5, 5); // generate simulated value for demo

//Serial.println(nodeSensors[myNode]);
  nodeTTLs[myNode] = 0;                     // reset Time-To-Live hop counter
  nodeTimestamps[myNode] = universalTime;   // update timestamp
}

void NodeTimeSlot() // takes 100ms at beginning of slot so don't tx during this time.
// time slots are 2048 milliseconds - easier to work with binary maths
{
  unsigned long t;
  unsigned long remainder;
  int timeSlot = 0;
  synchronizeTime();                  // get the current universalTime count
  t = universalTime >> TIME_SLOTS;  // divide
  t = t << TIME_SLOTS;            // round to start of timeslot
  remainder = universalTime - t;
  if (( remainder >= 0) && (remainder < 100)) // beginning of time slot
  {
    timeSlot = getTimeSlot();
    if (timeSlot == 0)
      lowestNode = 255;         // reset if node zero, get time from the lowest node number, 0 to 15 and if 255 then has been reset
    if (timeSlot == myNode)     // transmit if in my time slot
    {
      transmitTime();           // all nodes transmit its central time, when listening, it take the lowest number and ignore others
      digitalWrite(LED, HIGH);  // led on while tx long data message
      createDataMessage();      // send out all my local data via radio
      digitalWrite(LED, LOW);   // led off
      relayNode = 255;
    }
  }
}

int getTimeSlot() // find out which time slot we are in
{
  int n;
  unsigned long x;
  synchronizeTime();
  x = universalTime >> TIME_SLOTS;  // divide 
  x = x & 0x00000007;               // hardcoded to mask to one of 8 nodes
  n = (int) x;
  return n;
}

void printNode(int x) // print current node
{
  Serial.print((String) x);
  Serial.print(F(" = "));
  Serial.print((String) nodeSensors[x]);
  Serial.print(F(","));
  Serial.println((String) nodeTTLs[x]);
}

void displayOutput() // query nodes values to follow
{
  int y;
  y = nodeSensors[sensorNode];
  Serial.print(F("Counter = "));
  Serial.println(y);
  y = nodeTTLs[sensorNode];
  Serial.print(F("TTL = "));
  Serial.println(y);
}

