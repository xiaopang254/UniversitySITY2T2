/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                             M5StickC-Plus
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
*
* Describe: MQTT
* Date: 2021/11/5
*******************************************************************************
*/
#include "M5StickCPlus.h"
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// Configure the name and password of the connected wifi and your MQTT Serve
// host.
const char* ssid = "Pew pew";
const char* password = "pewpew123";
const char* mqtt_server = "192.168.43.132";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int toggleLED = 0;

void setupWifi();
void callback(char* topic, byte* payload, unsigned int length);
void reConnect();

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  pinMode(M5_LED, OUTPUT);
  digitalWrite(M5_LED, toggleLED);
  setupWifi();
  client.setServer(mqtt_server, 1883);  // Sets the server details.
  client.setCallback(callback);         // Sets the message callback function.
}

void loop() {
  if (!client.connected()) {
    reConnect();
  }
  client.loop();  // This function is called periodically to allow clients to
                  // process incoming messages and maintain connections to the
                  // server.

  unsigned long now = millis();  // Obtain the host startup duration.
  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      // snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);  // Format to the specified string and store it in MSG.
      snprintf(msg, MSG_BUFFER_SIZE, "B!");
      //digitalWrite(M5_LED, LOW);
      M5.Lcd.print("Publish message: ");
      M5.Lcd.println(msg);
      client.publish("ledNodeB", msg);  // Publishes a message to the specified
                                        // topic.
      if (value % 7 == 0) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
      }
    }
  }
}

void setupWifi() {
  delay(10);
  M5.Lcd.printf("Connecting to %s", ssid);
  WiFi.mode(WIFI_STA);         // Set the mode to WiFi station mode.
  WiFi.begin(ssid, password);  // Start Wifi connection.

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.printf("\nSuccess\n");
}

void callback(char* topic, byte* payload, unsigned int length) {
  M5.Lcd.print("Message arrived [");
  M5.Lcd.print(topic);
  M5.Lcd.print("] ");
  for (int i = 0; i < length; i++) {
    M5.Lcd.print((char)payload[i]);
  }
  M5.Lcd.println();
  // if ((char)payload[0] == 'A!') {
  //   Serial.println('I AM AT THE LED STATE');

  //   digitalWrite(M5_LED, toggleLED);
  // }
  if ((char)payload[0] == 'A') {
    if (toggleLED == 0) {
      toggleLED = 1;
    } else {
      toggleLED = 0;
    }
  }

  digitalWrite(M5_LED, toggleLED);
}

void reConnect() {
  while (!client.connected()) {
    M5.Lcd.print("Attempting MQTT connection...");
    // Create a random client ID.
    String clientId = "M5Stack-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect.
    if (client.connect(clientId.c_str())) {
      M5.Lcd.printf("\nSuccess\n");
      // Once connected, publish an announcement to the topic.

      client.publish("csc2006yay", "Hello, I am Node A. LED ON!!!");
      // ... and resubscribe.
      client.subscribe("ledNodeA");
    } else {
      M5.Lcd.print("failed, rc=");
      M5.Lcd.print(client.state());
      M5.Lcd.println("try again in 5 seconds");
      delay(5000);
    }
  }
}