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
#include "WifiCredentials.h"
#include "HomeApi.h"



#define TEMPERATURE_SAMPLE_RATE 32
#define TSR TEMPERATURE_SAMPLE_RATE

#define HUMIDITY_SAMPLE_RATE 8
#define HSR HUMIDITY_SAMPLE_RATE

//#define DEBUG

#define TIME_PER_SAMPLE 900
#define TIME_LIGHT_SLEEP(r,t) (r*(t)+(1000*r))

#define ONE_WIRE_BUS D2
#define DHT_PIN D3
#define DHT_TYPE DHT11

//LED will turn on when it makes a single read, so it should blink TSR times and stay off for TIME_LIGHT_SLEEP(s)
#define LED_PIN D5



//Sensor declarations
DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//NTP declarations
WiFiUDP ntpUdp;
NTPClient ntpClient(ntpUdp, "europe.pool.ntp.org", 7200);
HomeApi homeApi("http://localhost:2100/api/insertTemperature/");

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
  #ifdef DEBUG
  Serial.printf("Trying to connect to %s .", AP_BSSID);
  #endif
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(450);
    #ifdef DEBUG
    Serial.print(".");
    #endif
  }
  #ifdef DEBUG
  Serial.println();
  Serial.println(F("WiFi connection Successful"));
  Serial.print(F("The IP Address of ESP8266 Module is: "));
  Serial.println(WiFi.localIP()); // Print the IP address
  #endif
  ntpClient.begin();
  ntpClient.update();
  delay(10);
  homeApi.begin();
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

  for (uint8_t i = 0; i < TSR; i++){
    digitalWrite(LED_PIN, 1);
    sensors.requestTemperatures();
    dt += sensors.getTempCByIndex(0);
    if(i % HSR == 0) {
      h += dht.readHumidity();
    }
    digitalWrite(LED_PIN, 0);
    delay(TIME_PER_SAMPLE+15);
  }
  ntpClient.forceUpdate(); 
  
  #ifdef DEBUG
  Serial.print(F("time:"));
  Serial.println(ntpClient.getFormattedTime());
  #endif

  unsigned long epoch = ntpClient.getEpochTime();
  float avgDallas = dt / TSR;
  float avgHum = h / (TSR / HSR);
  float dallasTic = dht.computeHeatIndex(avgDallas, avgHum, false);

  struct HomeApi::sensor_data_t data =
        {
            avgDallas,
            avgHum,
            dallasTic,
            epoch
        };
  homeApi.sendData(data);
  //Serial.printf("%.4fºC, %.1f %c, %.4fºC.\n", avgDallas, avgHum, '%', dallasTic);
  #ifdef DEBUG
  Serial.printf("Sleeping for %i secs...\n", (TIME_LIGHT_SLEEP(TEMPERATURE_SAMPLE_RATE, TIME_PER_SAMPLE) / 1000 ));
  #endif
  delay(TIME_LIGHT_SLEEP(TEMPERATURE_SAMPLE_RATE, TIME_PER_SAMPLE));
}
