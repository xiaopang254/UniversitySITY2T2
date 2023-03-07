// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <RHReliableDatagram.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define node_id "Elmo"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
#define CLIENT_ADDRESS 8
#define SERVER_ADDRESS 2

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Initialise datagram manager
RHReliableDatagram manager(rf95, CLIENT_ADDRESS);

void (*resetFunc)(void) = 0;  //declare reset function at address 0

void setup() {
  Serial.begin(9600);
  delay(100);

  // Manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address or 0x3D for
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {
      delay(1000);
    }
  }
  // Setup oled display
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner
  delay(2000);
  display.clearDisplay();

  while (!manager.init()) {
    Serial.println(F("LoRa radio init failed"));
    display.println(F("Setup Failed"));
    display.display();
    while (1);
  }
  Serial.println(F("LoRa radio init OK!"));

  // Defaults after init are 915.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println(F("setFrequency failed"));
    display.println(F("Setup Failed"));
    display.display();
    while (1)
      ;
  }
  Serial.print(F("Set Freq to:"));
  Serial.println(RF95_FREQ);

  // Defaults after init are 915.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(13, false);

  display.println(F("Setup Successful"));
  display.display();
  delay(2000);
}

uint8_t packetnum = 0;  // packet counter, we increment per transmission

void loop() {
  Serial.println(F("Sending to Node"));

  // Send a message to rf95_rx
  uint8_t buf[48];
  uint8_t data[32];
  sprintf(data, "Hello World %d from %s", int(packetnum++), node_id);
  manager.setTimeout(2000);  

  uint8_t result = 0;
  while (result == 0) {
    display.setCursor(0, 0);
    display.clearDisplay();
    display.println("Sending Message");
    display.display();
    if (manager.sendtoWait((uint8_t*)data, sizeof(data), SERVER_ADDRESS)) {
      // Now wait for a reply from the server
      uint8_t len = sizeof(buf);
      uint8_t from;
      display.clearDisplay();
      display.println("Waiting for Reply");
      display.display();
      if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
        Serial.print(F("got reply from Node:"));
        Serial.print(from, HEX);
        Serial.print(F(": "));
        Serial.println((char*)buf);
        result = 1;
        display.clearDisplay();
        display.println("Message Received:");
        display.println((char*)buf);
        display.display();
      } else {
        Serial.println(F("No Reply"));
      }
    } else {
      Serial.println(F("Failed Sending Message"));
    }
    delay(7000);
  }
}