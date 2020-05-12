#ifndef HOME_API_H
#define HOME_API_H

class HomeApi {
    public:
        HomeApi();
        HomeApi(char* url);

    struct  sensor_data_t
        {
            float temp;
            float humidity;
            float temp_index;
            char* timestamp;
        };

        void begin(void);
        void sendData(sensor_data_t sensor_Data);
        void reload(void);  
};

#endif