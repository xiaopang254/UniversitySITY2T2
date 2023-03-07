#include <SPI.h>
#include <LoRa.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

String outgoing;           // outgoing message
byte msgCount = 0;         // count of outgoing messages
byte localAddress = 0xBE;  // address of this device
byte destination = 0xFF;   // destination to send to
long lastSendTime = 0;     // last send time
int interval = 2000;       // interval between sends

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  if (millis() - lastSendTime > interval) {
    String message = "Hello Smart Fridge";  // send a message
    sendMessage(message);
    Serial.println("Sending " + message);
    lastSendTime = millis();         // timestamp the message
    interval = random(2000) + 1000;  // 2-3 seconds
    LoRa.receive();                  // go back into receive mode
  }
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();             // start packet
  LoRa.write(destination);        // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}


void onReceive(int packetSize) {
  // received a packet
  if (packetSize == 0) return;  // if there's no packet, return

  Serial.print("Received packet '");

  int recipient = LoRa.read();        // recipient address
  byte sender = LoRa.read();          // sender address
  byte incomingMsgId = LoRa.read();   // incoming msg ID
  byte incomingLength = LoRa.read();  // incoming msg length

  String incoming = "";

  // read packet
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }


  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Received from: 0x" + String(sender, HEX));

  Serial.print(incoming);
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}