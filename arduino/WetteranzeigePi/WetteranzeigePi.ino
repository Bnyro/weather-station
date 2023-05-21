#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OpenWeather.h>
#include "config.h"

// Config file content:
// const char ssid[] = "";
// const char password[] = "";
// String host = "http://example.com/api/weather/latest?format=csv";
// String api_key = "";
// String latitude = "";
// String longitude = "";

String units = "metric";
String language = "de";

OW_Weather ow;
float OWTemperature, OWHumidity, OWWindSpeed, OWWindDeg, OWPressure, OWUVIndex;
String datetime, temperature, humidity, pressure;

const unsigned long HTTP_TIMEOUT = 10000;
int statusCode = 0;

WiFiClient client;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  pinMode(D0, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  lcd.init();
  lcd.backlight();
  connectWiFi();
}

void loop() {
  GetWeatherData();
  PrintWeatherData();
  delay(5000);
  OpenWeatherData();
  PrintOpenWeatherMapData();
  delay(5000);
}

void connectWiFi() {
  if (WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to ");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(5000);
    }
    lcd.setCursor(0, 2);
    lcd.print("Successfully connected.");
  }
}

void GetWeatherData() {

  HTTPClient http;

  http.begin(client, host);

  // get the JSON response
  int response = http.GET();
  String csv = http.getString();

  datetime = getValue(csv, ',', 0);
  temperature = getValue(csv, ',', 1);
  humidity = getValue(csv, ',', 2);
  pressure = getValue(csv, ',', 3);
}

void OpenWeatherData()
{
  OW_current *current = new OW_current;
  OW_hourly *hourly = new OW_hourly;
  OW_daily  *daily = new OW_daily;

  ow.getForecast(current, hourly, daily, api_key, latitude, longitude, units, language);

  if (current)
  {
    OWTemperature = current->temp;
    OWPressure = current->pressure;
    OWHumidity = current->humidity;
    OWWindSpeed = current->wind_speed;
  }

  delete current;
  delete hourly;
  delete daily;
}

void PrintWeatherData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(datetime);
  lcd.setCursor(0, 1);
  lcd.print("Temperatur: " + temperature + "C");
  lcd.setCursor(0, 2);
  lcd.print("Luftfeuchte: " + humidity + "%");
  lcd.setCursor(0, 3);
  lcd.print("Luftdruck: " + pressure + "hPa");
}

void PrintOpenWeatherMapData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: " + String(OWTemperature, 2) + "C");
  lcd.setCursor(0, 1);
  lcd.print("Luftdruck: " + String(OWPressure, 0) + "hPa");
  lcd.setCursor(0, 2);
  lcd.print("Luftfeuchte: " + String(OWHumidity, 2) + "%");
  lcd.setCursor(0, 3);
  lcd.print("Windstaerke: " + String(OWWindSpeed, 1) + "kmh");
}

String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
