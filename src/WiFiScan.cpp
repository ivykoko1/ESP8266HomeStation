/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"
#include "DHT.h"
#include "Wire.h"
#include "SPI.h"
#include "NTPClient.h"
#include "DallasTemperature.h"
#include "WiFiUdp.h"

#define AP_BSSID "COWIFI-19"
#define AP_PASS "99F595CBC3B674E5"

#define ONE_WIRE_BUS D2
#define DHT_PIN D3
#define DHT_TYPE DHT11

#define LED_PIN D5

#define SAMPLES_PER_LOG 32
#define TSPL SAMPLES_PER_LOG
#define HSPL 4

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUdp;
NTPClient ntpClient(ntpUdp, "europe.pool.ntp.org", 7200);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  wifi_set_sleep_type(LIGHT_SLEEP_T);

  dht.begin();
  sensors.begin();
  WiFi.begin(AP_BSSID, AP_PASS);

  Serial.printf("Trying to connect to %s .", AP_BSSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(450);
    Serial.print(".");
  }

  Serial.println();
  Serial.println(F("WiFi connection Successful"));
  Serial.print(F("The IP Address of ESP8266 Module is: "));
  Serial.println(WiFi.localIP()); // Print the IP address

  ntpClient.begin();
  ntpClient.update();
  delay(10);
  Serial.print(F("Boot time: "));
  Serial.println(ntpClient.getFormattedTime());

}

void loop()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = 0.0f;
  // Read temperature as Celsius (the default)

  float dt = 0.0f;

  for (uint8_t i = 0; i < TSPL; i++){
    digitalWrite(LED_PIN, 1);
    sensors.requestTemperatures();
    dt += sensors.getTempCByIndex(0);
    if(i % HSPL == 0) {
      h += dht.readHumidity();
    }
    digitalWrite(LED_PIN, 0);
    delay(1200);
  }
  ntpClient.forceUpdate();
  Serial.print(F("Time:"));
  Serial.println(ntpClient.getFormattedTime());

  float avgDallas = dt/TSPL;
  float avgHum = h / (TSPL/HSPL);

  float dallasTic = dht.computeHeatIndex(avgDallas, avgHum, false);

  Serial.println(F("DS18B20:"));
  Serial.printf("%.3fºC, %.1f percent humidity, %.4fºC Perceived temperature.\n", avgDallas, avgHum, dallasTic);
  Serial.println(F("Sleeping for 1 minute..."));
  delay(60000);
  Serial.println(F("Reading now"));
}
