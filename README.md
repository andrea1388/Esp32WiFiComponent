# Esp32WiFiComponent
A component written in C++ for handling wifi

Minimal usage

#include "wifi.hpp"

static const char *TAG = "main";

void WiFiEvent(WiFi* wifi, uint8_t ev)
{
    switch(ev)
    {
        case WIFI_DISCONNECT: // disconnected
            ESP_LOGI(TAG,"Disconnected.");
            wifi->Connect();
            break;
        case WIFI_GOT_IP: // connected
            ESP_LOGI(TAG,"Connected.");
            break;
    }
}


extern "C" {

    void app_main(void)
    {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }

        WiFi wifi;
        wifi.onEvent=&WiFiEvent;
        wifi.Start("ssid","password");
        for(;;) {
            vTaskDelay(1);
        }
    }

}
