#include <WiFiUdp.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

int bottomSensor = 0; //Bottom Sensor
int middleSensor = D13; // Middle Sensor
int topSensor = D14; // Top Sensor

WiFiUDP udp;
// Set IP address of influx server
byte influx[] = {10, 10, 10, 10};
// Set outgoing UDP port
int port = 8888;

void setup() {
    Serial.begin(9600);
    // Set up our WiFi
    WiFiManager wifiManager;
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    wifiManager.autoConnect("IoT-AP");
    Serial.println("connected... :)");
    pinMode(middleSensor, INPUT);
    pinMode(topSensor, INPUT);
}

void loop() {

    String bottomSensorValue = String(analogRead(bottomSensor));
    String middleSensorValue = String(digitalRead(middleSensor));
    String topSensorValue = String(digitalRead(topSensor));
    // Create our string for Influx
    String bottomstring = "sump_pump_monitor_bottom value=" + bottomSensorValue;
    String middlestring = "sump_pump_monitor_middle value=" + middleSensorValue;
    String topstring = "sump_pump_monitor_top value=" + topSensorValue;
    // Send our strings via UDP to our influx server
    sendudp(bottomstring);
    sendudp(middlestring);
    sendudp(topstring);
    delay(1000);
}

//Let's send to influxdb
void sendudp(String poststring) {
  udp.beginPacket(influx, port);
  udp.print(poststring);
  udp.endPacket();
}
