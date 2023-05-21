#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <DHT.h>
#inclue "config.h"

// Config file content:
// const char* ssid      = "";
// const char* password  = "";
// const char* server    = "http://example.com/api/weather";

// static char temperatureTemp[7], humidityTemp[7], ds18Temp[7];
float ds18_t;
double bmp180_t, bmp180_p;
double dht_h, dht_t, dht_f;

#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
ADC_MODE(ADC_VCC);

#define DHTTYPE DHT22
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

SFE_BMP180 pressure;
#define ALTITUDE 280.0

void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);

  digitalWrite(12, HIGH);
  initWifi();
  readSensors();
  makeHTTPRequest();

  digitalWrite(12, LOW);
  ESP.deepSleep(3600e6);
}

void loop() {}

void initWifi() {
  Serial.print("\nConnecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect, going back to sleep.");
    ESP.deepSleep(60e6);
  }
  else {
    Serial.println("\nConnected to WiFi.");
  }
}

void readSensors() {
  // ds18
  sensors.begin();
  sensors.requestTemperatures();
  ds18_t = sensors.getTempCByIndex(0);

  // dht
  dht.begin();
  dht_t = dht.readTemperature();
  dht_h = dht.readHumidity();

  // bmp180
  char status;
  pressure.begin();
  status = pressure.startTemperature();
  delay(status);

  status = pressure.getTemperature(bmp180_t);
  status = pressure.startPressure(3);
  delay(status);

  status = pressure.getPressure(bmp180_p, bmp180_t);

  Serial.println(ds18_t);
  Serial.println(dht_t);
  Serial.println(dht_h);
  Serial.println(bmp180_t);
  Serial.println(bmp180_p);
}

void makeHTTPRequest() {
  Serial.print("Connecting to ");
  Serial.println(server);

  WiFiClient client;
  HTTPClient http;

  http.begin(client, server);

  http.addHeader("Content-Type", "application/json");
  // build the JSON formatted string
  String form = String("{")
                + "\"ds18_t\":" + String(ds18_t)
                + ",\"dht_t\":" + String(dht_t)
                + ",\"dht_h\":" + String(dht_h)
                + ",\"bmp_t\":" + String(bmp180_t)
                + ",\"bmp_p\":" + String(bmp180_p)
                + "}";

  Serial.println("JSON: " + form);

  // post the data to the api
  int count = 0;
  int httpResponseCode = 0;

  // try 5 times if not successful
  while (httpResponseCode != 200 && count < 5) {
    httpResponseCode = http.POST(form);
    Serial.println("http response code: " + String(httpResponseCode));
    count++;
  }

  Serial.println("\nclosing connection");
  client.stop();
}
