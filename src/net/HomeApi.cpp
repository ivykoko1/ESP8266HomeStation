#include "HomeApi.h"


    HomeApi::HomeApi(String url)
    {
        this->url  = url;
    }

    HomeApi::HomeApi()
    {
    }

    void HomeApi::begin(){
        this->httpClient.begin(this->url);
        this->httpClient.addHeader("Content-type", "application/json");
    }

    bool HomeApi::sendData(HomeApi::sensor_data_t data){
        char  buffer[128];
        int json = sprintf(buffer, "{\"sensorTime\" %lu, \"temperature\": %.4f, \"humidity\": %.2f, \"temp_index\": %.4f}", data.sensor_time, data.temp, data.humidity, data.temp_index);
        Serial.println(buffer);
        int responseCode  = this->httpClient.POST(buffer);
        Serial.println(responseCode);
        httpClient.end();
        return (responseCode == 200);
    }

    void HomeApi::reload(){
        this->begin();
    }