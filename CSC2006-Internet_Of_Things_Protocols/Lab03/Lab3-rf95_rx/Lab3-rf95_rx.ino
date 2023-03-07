#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// LoRa 9x_RX
// -*- mode:C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_TX

#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <RHReliableDatagram.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// LED Screen
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram RHManager(rf95, 222);

// Initialize screen display.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char input_string[100];

 
void(* resetFunc) (void) = 0; //declare reset function at address 0

void setup() 
{
  Serial.begin(9600);
  delay(100);
  
  // Manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // while (!rf95.init()) {
  //   Serial.println("LoRa radio init failed");
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Setup Failed"));
    display.display();      // Show initial text
    delay(100);
  //   while (1);
  // }
  Serial.println("LoRa radio init OK!");
  if(!RHManager.init())
  {
    Serial.println("RHManager Init Failed");
  }
  else{
    Serial.println("RHManager Init Ok!");
  }
  // Defaults after init are 915.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  // if (!rf95.setFrequency(RF95_FREQ)) {
  //   Serial.println("setFrequency failed");
  //   display.clearDisplay();
  //   display.setTextSize(1); // Draw 2X-scale text
  //   display.setTextColor(SSD1306_WHITE);
  //   display.setCursor(10, 0);
  //   display.println(F("Setup Failed"));
  //   display.display();      // Show initial text
  //   delay(100);
  //   while (1);
  // }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 915.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(13, false);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
}
Serial.println(F("SSD1306 allocation successful"));
  display.clearDisplay();

  display.display();
  delay(200);


  //To be displayed in LED when setup is successful
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Setup Successful"));
  display.display();      // Show initial text
  delay(100);
}

void loop()
{


  //To be displayed in LEd when message received
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 0);
      display.println(F("Waiting..."));
      display.display();      // Show initial text
      delay(100);

  if (rf95.available())
  {
    
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t from = 111;
    uint8_t to = 213;

    //To be displayed in LEd when message received
      display.clearDisplay();
      display.setTextSize(1); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 0);
      display.println(F("Waiting for Reply"));
      display.display();      // Show initial text
      delay(100);

      if (RHManager.available()){
        if(RHManager.recvfromAck(buf, &len, 111, 213))
        {
          Serial.println("Received Something..");

          RHManager.setHeaderFrom(&from);
          RHManager.setHeaderTo(&to);

          RHManager.sendtoWait(buf, &len, &to);
        }
      }
    
    // if (rf95.recv(buf, &len))
    // {
    //   // Set specific headerid, headerfrom and headerto
    //   //rf95.setHeaderId(rf95.headerId());
    //   rf95.setHeaderId((uint8_t) 123);
    //   // rf95.setHeaderFrom(rf95.headerFrom());
    //   rf95.setHeaderFrom((uint8_t) 213);
    //   rf95.setHeaderTo(rf95.headerFrom());
    //   // rf95.setHeaderTo((uint8_t) 213);

    //   // Message received

    //   RH_RF95::printBuffer("Received: ", buf, len);
    //   Serial.print("Got: ");
    //   Serial.println((char*)buf);
    //   Serial.print("RSSI: ");
    //   Serial.println(rf95.lastRssi(), DEC);

    //   //To be displayed in LEd when message received
    //   display.clearDisplay();
    //   display.setTextSize(1); // Draw 2X-scale text
    //   display.setTextColor(SSD1306_WHITE);
    //   display.setCursor(20, 0);
    //   display.println(F("Message Received"));
    //   display.display();      // Show initial text
    //   delay(100);
    //   Serial.print("HeaderID: ");
    //   Serial.println(rf95.headerId());
    //   Serial.print("HeaderTo: ");
    //   Serial.println(rf95.headerTo());
    //   Serial.print("HeaderFrom: ");
    //   Serial.println(rf95.headerFrom());
      
    //   // Send a reply
    //   uint8_t data[] = "I am Barney! No Joke!";
    //   rf95.send(data, sizeof(data));
    //   rf95.waitPacketSent();
    //   Serial.println("Sent a reply");
      
    //   //To be displayed in LED when it is waiting for reply
    
    //   display.clearDisplay();
    //   display.setTextSize(1); // Draw 2X-scale text
    //   display.setTextColor(SSD1306_WHITE);
    //   display.setCursor(20, 0);
    //   display.println(F("Sending Message"));
    //   display.display();      // Show initial text
    //   delay(300);

    //   Serial.println("Message sent");

      
    // }
    // else
    // {
    //   Serial.println("Receive failed");
    // }
  }
}