/**
 * esp32-dallas-temperature example
 *
 * main.c
 *
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#include "esp_log.h"
#include "esp_event.h"

#include "dallas_temperature.h"

static const char *TAG = "main";

void event_handler(void * handler_arg, esp_event_base_t base, int32_t id, void * event_data)
{
    if(base != DALLAS_TEMPERATURE_EVENT_BASE) {
        return;
    }

    dallas_temperature_sensor_t * sensor;

    switch(id) {
        case DALLAS_TEMPERATURE_SEARCH_STARTED_EVENT:
            ESP_LOGI(TAG, "SEARCH_STARTED");
            break;
        case DALLAS_TEMPERATURE_SEARCH_FINISHED_EVENT:
            ESP_LOGI(TAG, "SEARCH_FINISHED");
            break;
        case DALLAS_TEMPERATURE_SEARCH_ERROR_EVENT:
            ESP_LOGI(TAG, "SEARCH_ERROR");
            break;
        case DALLAS_TEMPERATURE_SENSOR_REGISTERED_EVENT:
            sensor = (dallas_temperature_sensor_t *) event_data;
            ESP_LOGI(TAG, "SENSOR_REGISTERED with ROM code: %s", sensor->rom_code_string);
            break;
        case DALLAS_TEMPERATURE_SENSOR_SAMPLE_EVENT:
            sensor = (dallas_temperature_sensor_t *) event_data;
            ESP_LOGI(TAG, "SENSOR_SAMPLE %f with ROM code %s", sensor->temperature, sensor->rom_code_string);
            break;
        case DALLAS_TEMPERATURE_SENSOR_TEMPERATURE_CHANGE_EVENT:
            sensor = (dallas_temperature_sensor_t *) event_data;
            ESP_LOGI(TAG, "SENSOR_TEMPERATURE_CHANGE %f with ROM code %s", sensor->temperature, sensor->rom_code_string);
            break;
        case DALLAS_TEMPERATURE_SENSOR_ERROR_EVENT:
            sensor = (dallas_temperature_sensor_t *) event_data;
            ESP_LOGI(TAG, "SENSOR_ERROR with ROM code: %s", sensor->rom_code_string);
            break;
        case DALLAS_TEMPERATURE_SENSOR_LOST_EVENT:
            sensor = (dallas_temperature_sensor_t *) event_data;
            ESP_LOGI(TAG, "SENSOR_LOST with ROM code: %s", sensor->rom_code_string);
            break;
        default:
            ESP_LOGE(TAG, "Unknown event %d", id);
            break;
    }
}

// Create configuration structure and populate with default values
dallas_temperature_t dallas_temperature = DALLAS_TEMPERATURE_DEFAULT();

void app_main(void)
{
    esp_log_level_set("event",              ESP_LOG_WARN);
    esp_log_level_set("rmt",                ESP_LOG_WARN);
    esp_log_level_set("owb_rmt",            ESP_LOG_WARN);
    esp_log_level_set("owb",                ESP_LOG_WARN);
    esp_log_level_set("ds18b20",            ESP_LOG_WARN);
    esp_log_level_set("dallas_temperature", ESP_LOG_WARN);

    ESP_LOGI(TAG, "[INFO] Dallas Temperature example started");

    // Customize configuration to your application
    dallas_temperature.config.bus_gpio = (gpio_num_t) 32;
    dallas_temperature.config.pullup_helper = (gpio_num_t) 33;
    dallas_temperature.config.enable_crc = true;
    dallas_temperature.config.search_period = 60;
    dallas_temperature.config.sampling_period = 5;
    dallas_temperature.config.resolution = DS18B20_RESOLUTION_11_BIT;

    // Start dallas_temperature
    dallas_temperature_start(&dallas_temperature);

    // Register for desired events (see all events in dallas_temperature.h)
    esp_event_handler_register_with(dallas_temperature.event_loop, DALLAS_TEMPERATURE_EVENT_BASE, ESP_EVENT_ANY_ID, event_handler, NULL);
}

