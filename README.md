Sensor station using the Wemos D1 Board containing the ESP8266 chip and two modules.

The modules are: DS18B20 to get the temperature (32 values with a 1.5sec delay within them) and the DHT11, which will only be used to measure humidity, at a rate of 1/4 the temperature.

Heat index is also calculated.

This will in later versions use a custom developed API to insert every read into a MySQL server.

Then, this data will be presented beautifully via web and (possibly) Android and or iOS apps.
