#include <BLEDevice.h>
#include <BLEServer.h>
//#include <BLEUtils.h>
//#include <BLE2902.h>
#include <M5StickCPlus.h>
#include "AXP192.h"
//#include <Wire.h>

//Default Temperature in Celsius
#define temperatureCelsius

//LED
#define LEDCharacteristics

//change to unique BLE server name
#define bleServerName "CSC2006-BLE2100582#01"

#define BUTTON_HOME 37
#define LED_PIN 10

float tempC = 25.0;
float tempF;
float vBatt = 5.0;  // initial value

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1500;   // update refresh every 15sec

bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "85dcc2ed-92c9-4de5-8c86-ba3107aea437"

// Temperature Characteristic and Descriptor
#ifdef temperatureCelsius
  BLECharacteristic imuTemperatureCelsiusCharacteristics("00595611-3a94-4b48-8cf6-31e5d794aaac", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor imuTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
#else
  BLECharacteristic imuTemperatureFahrenheitCharacteristics("1b9cb02c-a8c7-46df-b9a1-2903f2557780", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor imuTemperatureFahrenheitDescriptor(BLEUUID((uint16_t)0x2902));
#endif

// Battery Voltage Characteristic and Descriptor
BLECharacteristic axpVoltageCharacteristics("96007940-0d22-4583-b468-7a94a705b289", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor axpVoltageDescriptor(BLEUUID((uint16_t)0x2903));

//LED Characteristic and Descriptor
BLECharacteristic imuLEDCharacteristics("9589dd48-9660-4613-afb4-642d7f2bb9e8", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor imuLEDDescriptor(BLEUUID((uint16_t)0x2904));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("MyServerCallbacks::Connected...");
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("MyServerCallbacks::Disconnected...");
  }
};

void setup() {
  // Start serial communication 
  Serial.begin(115200);

  // put your setup code here, to run once:
  M5.begin();

  // Battery Voltage
  M5.Axp.begin();

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.printf("BLE Server", 0);

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bleService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics and Create a BLE Descriptor
  // Temperature
  #ifdef temperatureCelsius
    bleService->addCharacteristic(&imuTemperatureCelsiusCharacteristics);
    imuTemperatureCelsiusDescriptor.setValue("IMU Temperature(C)");
    imuTemperatureCelsiusCharacteristics.addDescriptor(&imuTemperatureCelsiusDescriptor);
  #else
    bleService->addCharacteristic(&imuTemperatureFahrenheitCharacteristics);
    imuTemperatureFahrenheitDescriptor.setValue("IMU Temperature(F)");
    imuTemperatureFahrenheitCharacteristics.addDescriptor(&imuTemperatureFahrenheitDescriptor);
  #endif  

  // Battery
  bleService->addCharacteristic(&axpVoltageCharacteristics);
  axpVoltageDescriptor.setValue("AXP Battery(V)");
  axpVoltageCharacteristics.addDescriptor(&axpVoltageDescriptor);

  // LED Service
  bleService->addCharacteristic(&imuLEDCharacteristics);
  imuLEDDescriptor.setValue("IMU LED");
  imuLEDCharacteristics.addDescriptor(&imuLEDDescriptor);

  // Setting Pins
  pinMode(BUTTON_HOME, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Set Interrupt
  //attachInterrupt(digitalPinToInterrupt(BUTTON_HOME), btnPress, FALLING);

  // Start the service
  bleService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

float readTemperature(){
  float t;
  M5.IMU.getTempData(&t);
  t = (t-32.0)*5.0/9.0;
  return t;
}

float readVoltage() {
  float battery_voltage = M5.Axp.GetBatVoltage();
  return battery_voltage;
  
}

void onDemand() {
  M5.IMU.getTempData(&tempC);
      tempC = (tempC-32.0)*5.0/9.0;

      // Fahrenheit
      tempF = 1.8*tempC + 32;

      // Battery voltage
      // if (vBatt<1.0)
      //   vBatt = 5.0;
      // else
      //   vBatt -= 0.01;
      vBatt = readVoltage();
  
      //Notify temperature reading from IMU
      #ifdef temperatureCelsius
        static char temperatureCTemp[6];
        M5.update();
        dtostrf(tempC, 6, 2, temperatureCTemp);
        //Set temperature Characteristic value and notify connected client
        imuTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
        imuTemperatureCelsiusCharacteristics.notify();
        Serial.print("Temperature = ");
        Serial.print(tempC);
        Serial.print(" C");

        M5.Lcd.setCursor(0, 20, 2);
        M5.Lcd.print("Temperature = ");
        M5.Lcd.print(tempC);
        M5.Lcd.println(" C");
      #else
        static char temperatureFTemp[6];
        dtostrf(tempF, 6, 2, temperatureFTemp);
        //Set temperature Characteristic value and notify connected client
        imuTemperatureFahrenheitCharacteristics.setValue(temperatureFTemp);
        imuTemperatureFahrenheitCharacteristics.notify();
        Serial.print("Temperature = ");
        Serial.print(tempF);
        Serial.print(" F");

        M5.Lcd.setCursor(0, 20, 2);
        M5.Lcd.print("Temperature = ");
        M5.Lcd.print(tempF);
        M5.Lcd.println(" F");
      #endif
      
      //Notify battery status reading from AXP192
      static char voltageBatt[6];
      dtostrf(vBatt, 6, 2, voltageBatt);
      //Set voltage Characteristic value and notify connected client
      axpVoltageCharacteristics.setValue(voltageBatt);
      axpVoltageCharacteristics.notify();   
      Serial.print(" - Battery Voltage = ");
      Serial.print(vBatt);
      Serial.println(" V");

      M5.Lcd.setCursor(0, 40, 2);
      M5.Lcd.print("Battery Votage = ");
      M5.Lcd.print(vBatt);
      M5.Lcd.println(" V");
}

void btnPress() {
  int ledStatus = digitalRead(LED_PIN);

  if(ledStatus == LOW){
    
    digitalWrite(LED_PIN, HIGH);
    imuLEDCharacteristics.setValue("LED OFF");
    onDemand();
  }
  else {
    digitalWrite(LED_PIN, LOW);
    imuLEDCharacteristics.setValue("LED ON");
    onDemand();
  }

    imuLEDCharacteristics.notify();
}

void loop() {
  if (deviceConnected) {
    if ((millis() - lastTime) > timerDelay) {
      // Read temperature as Celsius (the default)

      if (digitalRead(BUTTON_HOME) == LOW) {
        btnPress();
      }
      // if (random(2)>0)
      //   tempC += random(10)/100.0;
      // else
      //   tempC -= random(10)/100.0;
      
      
      
      lastTime = millis();
    }
  }
}
