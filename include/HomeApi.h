#ifndef HOME_API_H
#define HOME_API_H

#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"


class HomeApi {
    public:
        HomeApi();
        HomeApi(String url);

    struct  sensor_data_t
        {
            float temp;
            float humidity;
            float temp_index;
            unsigned long sensor_time;
        };

        void begin(void);
        bool sendData(sensor_data_t data);
        void reload(void);
        String  url;
        HTTPClient httpClient;
} ;

#endif