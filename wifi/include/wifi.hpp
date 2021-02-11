#ifndef WiFi_hpp
#define WiFi_hpp
#include "esp_event.h"
#include <string>
#include <vector>
using namespace std;
#define WIFI_START 1
#define WIFI_GOT_IP 2
#define WIFI_DISCONNECT 3

class WiFi {
    public:
        void Connect();
        void Start(const char* ssid,const char* password);
        void AddNetwork(const char* ssid,const char* password);
        void (*onEvent)(WiFi* wifi, uint8_t ev);
        vector<string> ssid;
        vector<string> password;

    private:
        static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    
        
};
#endif 
