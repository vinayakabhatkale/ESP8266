#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme;

// your wifi credentials
const char* ssid = "WIFI_SSID";
const char* password = "password";


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char* AWS_endpoint = "XXXXXXXXXXXXX-ats.iot.eu-central-1.amazonaws.com"; //ADD YOUR MQTT end point and region.

void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}
Serial.println();

}
WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); 
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

//wait for 10 seconds
delay(10);
// We start by connecting to a WiFi network
espClient.setBufferSizes(512, 512);
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid,password);

WiFi.begin(ssid);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

timeClient.begin();
while(!timeClient.update()){
timeClient.forceUpdate();
}

espClient.setX509Time(timeClient.getEpochTime());

}

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESPthing")) {
Serial.println("connected");
// Once connected, publish an announcement...

// ... and resubscribe
client.subscribe("inTopic");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");

char buf[256];
espClient.getLastSSLError(buf,256);
Serial.print("WiFiClientSecure SSL error: ");
Serial.println(buf);

// Wait 5 seconds before retrying
delay(5000);
}
}
}

void setup() {

Serial.begin(9600);  change the baud according to requirement
Serial.setDebugOutput(true);
setup_wifi();
delay(1000);
if (!SPIFFS.begin()) {
Serial.println("Failed to mount file system");
return;
}

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

// Load certificate file
File cert = SPIFFS.open("/Amazon.cert.der","r"); //replace Amazon.cert with your file name
if (!cert) {
Serial.println("Failed to open cert file");
}
else
Serial.println("Success to open cert file");

delay(1000);

if (espClient.loadCertificate(cert))
Serial.println("cert loaded");
else
Serial.println("cert not loaded");
delay(1000);
// Load private key file
File private_key = SPIFFS.open("/private.der", "r"); //change the file name according to yours
if (!private_key) {
Serial.println("Failed to open private cert file");
}
else
Serial.println("Success to open private cert file");

delay(1000);

if (espClient.loadPrivateKey(private_key))
Serial.println("private key loaded");
else
Serial.println("private key not loaded");






// Load CA file
File ca = SPIFFS.open("/ca.der", "r"); //replace ca with your uploaded file name
if (!ca) {
Serial.println("Failed to open ca ");
}
else
Serial.println("Success to open ca");

delay(1000);

if(espClient.loadCACert(ca))
Serial.println("ca loaded");
else
Serial.println("ca failed");

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); 

//test bme680  starts

while (!Serial);
Serial.println(F("BME680 test"));
if (!bme.begin(0x77))
{
Serial.println("Could not find a valid BME680 sensor, check wiring!");
while (1);
}
bme.setTemperatureOversampling(BME680_OS_8X);
bme.setHumidityOversampling(BME680_OS_2X);
bme.setPressureOversampling(BME680_OS_4X);
bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
bme.setGasHeater(320, 150); // 

//test bme680 ends

}

void loop() {

  if (! bme.performReading())
{
Serial.println("Failed to perform reading :(");
return;
}
Serial.print("Temperature = ");
Serial.print(bme.temperature);
Serial.println(" *C");
Serial.print("Pressure = ");
Serial.print(bme.pressure / 100.0);
Serial.println(" hPa");
Serial.print("Humidity = ");
Serial.print(bme.humidity);
Serial.println(" %");
Serial.print("Gas = ");
Serial.print(bme.gas_resistance / 1000.0);
Serial.println(" KOhms");
Serial.print("Approx. Altitude = ");
Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
Serial.println(" ft");
Serial.println();
delay(2000);

float t=bme.temperature;
float h=bme.humidity;
float a=bme.readAltitude(SEALEVELPRESSURE_HPA);
if (!client.connected()) {
reconnect();
}
client.loop();
String temp = "{\"temperature\":" + String(t) + ", \"humidity\":" + String(h) + ", \"altitude\":" + String(a) +"}";
    int len = temp.length();
    char msg[len+1];
    temp.toCharArray(msg, len+1);

Serial.print("Publish message: ");
Serial.println(msg);
client.publish("outTopic", msg);
Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
}
