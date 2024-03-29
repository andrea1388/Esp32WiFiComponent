#include "wifi.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "string.h"
#include <string>
#include <vector>
using namespace std;


void WiFi::AddNetwork(const char* _ssid,const char* _password) 
{
    ssid.push_back(_ssid);
    password.push_back(_password);
}

void WiFi::event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    WiFi *obj=(WiFi*)arg;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        // call event handler if set or connect
        if (obj->onEvent!=NULL) (*obj->onEvent)(obj,WIFI_START);
        else obj->Connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (obj->onEvent!=NULL) (*obj->onEvent)(obj,WIFI_DISCONNECT);
        else obj->Connect();
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        if (obj->onEvent!=NULL) (*obj->onEvent)(obj,WIFI_GOT_IP);
    }
}
void WiFi::Connect()
{
    esp_wifi_connect();
}
void WiFi::Start(const char* _ssid,const char* _password)
{
    AddNetwork(_ssid,_password);

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &WiFi::event_handler,
                                                        this,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &WiFi::event_handler,
                                                        this,
                                                        NULL));

    wifi_config_t wifi_config = {};
    strcpy((char*)wifi_config.sta.ssid,ssid[0].c_str());
    strcpy((char*)wifi_config.sta.password,password[0].c_str());
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

}




