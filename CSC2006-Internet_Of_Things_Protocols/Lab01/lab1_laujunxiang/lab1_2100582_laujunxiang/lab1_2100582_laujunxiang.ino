// https://docs.m5stack.com/en/quick_start/m5stickc_plus/arduino
// 2100582 Lau Jun Xiang P4 CSC2006
#include <WiFi.h>
#include <WebServer.h>
#include <M5StickCPlus.h>

/* Put your SSID & Password */
const char* ssid = "Pew pew";
const char* password =  "pewpew123";

// read new set of sensors value on button push
float pitch, roll, yaw, temperature, accelX, accelY, accelZ;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  M5.begin();

  int x = M5.IMU.Init(); //return 0 is ok, return -1 is unknown
  if(x!=0)
    Serial.println("IMU initialisation fail!");  

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.printf("RESTful API", 0);

  WiFi.begin(ssid, password);

  // Setting the hostname
  WiFi.setHostname("group01-stick");

  Serial.print("Start WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting ..");
  }
  M5.Lcd.setCursor(0, 20, 2);
  M5.Lcd.print("IP: ");
  M5.Lcd.println(WiFi.localIP());

  // SET PINS
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_LED, OUTPUT);
  delay(100);

  // APIs
  server.on("/", handle_JsonResponse);
  server.on("/gyro", getGyro);
  server.on("/accel", getAccelerometer);
  server.on("/temp", getTemperature);
  server.on("/led/1", postLEDOff);
  server.on("/led/0", postLEDOn);
  server.on("/buzzer/0", postBuzzerOff);
  server.on("/buzzer/1", postBuzzerOn);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.print("Connected to the WiFi network. IP: ");
  Serial.println(WiFi.localIP());
  //digitalWrite(M5_LED, LOW);
}
 
void handle_JsonResponse(){
  String response;
  response += "{ \"imu\": { \"pitch\": ";
  response += String(pitch, 6);
  response += ", \"roll\": ";
  response += String(roll, 6);
  response += ", \"yaw\": ";
  response += String(yaw, 6);
  response += ", \"temperature\": ";
  response += String(temperature, 6);
  response += " } }";
  Serial.println(response);
  //digitalWrite(M5_LED, 1);

  server.send(200, "application/json", response);
}

void postLEDOff() {
  String message = "LED Turn Off!";

  String response;
  response += "{ \"imu\": { \"response\": ";
  response += message;
  response += " } }";
  int ledData = digitalRead(10);
  digitalWrite(M5_LED, HIGH);
  M5.Lcd.println("\nLED Turn Off!");
  Serial.println(("Turning Off LED!"));

  server.send(200, "application/json", response);

}

void postLEDOn() {
  String message = "LED Turn On!";
  
  String response;
  response += "{ \"imu\": { \"response\": ";
  response += message;
  response += " } }";

  digitalWrite(M5_LED, LOW);
  M5.Lcd.println("\nLED Turn On!");
  Serial.println(("Turning On LED!"));

  server.send(200, "application/json", response);
}

void postBuzzerOff() {
  String response = "Buzzer Off!";

  M5.Beep.tone(0);
  M5.Beep.update();

  Serial.println(response);
  M5.Lcd.println(response);
  server.send(200, "application/json", response);
}

void postBuzzerOn() {
  String response = "Buzzer On!";

  M5.Beep.tone(2500);
  M5.Beep.update();
  
  Serial.println(response);
  M5.Lcd.print(response);
  server.send(200, "application/json", response);
}

void getTemperature(){
  String response;
  response += "{ \"imu\": { \"temperature\": ";
  response += String(temperature, 6);
  response += " } }";
  Serial.println(response);

  server.send(200, "application/json", response);
}

// Display Accelerometer data in JSON
void getAccelerometer(){
  String response;
  response += "{ \"imu\": { \"X\": ";
  response += String(accelX, 6);
  response += ", \"Y\": ";
  response += String(accelY, 6);
  response += ", \"Z\": ";
  response += String(accelZ, 6);
  response += " } }";
  Serial.println(response);

  server.send(200, "application/json", response);
}

// Display Gyroscope data in JSON
void getGyro(){
  String response;
  response += "{ \"imu\": { \"pitch\": ";
  response += String(pitch, 6);
  response += ", \"roll\": ";
  response += String(roll, 6);
  response += ", \"yaw\": ";
  response += String(yaw, 6);
  response += " } }";
  Serial.println(response);

  server.send(200, "application/json", response);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


bool readGyro(){
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  return true;
}


float readTemperature(){
  float t;
  M5.IMU.getTempData(&t);
  t = (t-32.0)*5.0/9.0;
  return t;
}

float readAccelerometer() {
  M5.IMU.getAccelData(&accelX, &accelY, &accelZ);
  return true;
}

uint8_t setup_flag = 1;

bool readSensors() {
  bool status = readGyro();
  if (status==true) {
    M5.Lcd.setCursor(0, 40, 2);
    M5.Lcd.printf("                                        ");
  }
  Serial.println("Gyro:");
  Serial.print("Pitch[X]: ");
  Serial.println(pitch);
  Serial.print("Roll[Y]: ");
  Serial.println(roll);
  Serial.print("Yaw[Z]: ");
  Serial.println(yaw);

  temperature = readTemperature();
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.printf("Temperature = %2.1f", temperature);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  bool statusAccel = readAccelerometer();
  if (status == true) {
    M5.Lcd.setCursor(0, 80, 2);
    M5.Lcd.printf("                                        ");
  }
  Serial.println("Accelerometer:");
  Serial.print("Accel[X]: ");
  Serial.println(accelX);
  Serial.print("Accel[Y]: ");
  Serial.println(accelY);
  Serial.print("Accel[Z]: ");
  Serial.println(accelZ);

  return status;
}
 
void loop() {
  server.handleClient();
  M5.update();

  if (setup_flag == 1) {
    M5.Lcd.setCursor(0, 40, 2);
    M5.Lcd.printf("X = %3.2f, Y = %3.2f, Z = %3.2f", pitch, roll, yaw);
    M5.Lcd.setCursor(0, 60, 2);
    M5.Lcd.printf("Temperature = %2.1f", temperature);
    M5.Lcd.setCursor(0, 80, 2);
    M5.Lcd.printf("X = %3.2f, Y = %3.2f, Z = %3.2f", accelX, accelY, accelZ);
  }

  if(!setup_flag){
    setup_flag = 1;
    bool status = readSensors();
    if (status)
      Serial.print("\n\rRead Sensors success...\n");
    else
      Serial.print("\n\rRead Sensors failed...\n");
  }

  if(digitalRead(M5_BUTTON_HOME) == LOW){
    setup_flag = 0;
    while(digitalRead(M5_BUTTON_HOME) == LOW);
  }
}
