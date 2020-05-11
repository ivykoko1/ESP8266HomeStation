Sensor station using the Wemos D1 Board containing the ESP8266 chip and two modules.

The modules are: DS18B20 to get the temperature (32 values with a 1.5sec delay within them) and the DHT11, which will only be used to measure humidity, at a rate of 1/4 the temperature.

Heat index is also calculated.

SAMPLE log for a single 32 read result:

```
Trying to connect to WIFI-19 ....
WiFi connection Successful
The IP Address of ESP8266 Module is: 192.168.0.85
Boot time: 05:58:59
Time:05:59:54
DS18B20:
21.643ºC, 62.8 percent humidity, 21.5006ºC Perceived temperature.
Sleeping for 1 minute...
```
This will in later versions use a custom developed API to insert every read into a MySQL server.

Then, this data will be presented beautifully via web and (possibly) Android and or iOS apps.
