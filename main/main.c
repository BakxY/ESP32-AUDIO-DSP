#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "driver/ledc.h"

// Define the ADC channel used
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_6
#define ADC2_EXAMPLE_CHAN0          ADC1_CHANNEL_7

// Define the GPIO used for the LED
#define ADC_EXAMPLE_ATTEN           ADC_ATTEN_DB_11

// Define the resolution of the ADC
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT

// Define the frequency of the LEDC timer
#define LEDC_FREQUENCY          (250000)

// Initialize a variable to store the pwm duty value
int PWMDutyValue = 0;

// This is the function that will run on core 0 to write the pwm
void writePWMCore0(void *pvParameter)
{
    // Create the config for the ledc timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Create the config for the ledc channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = 5,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    
    while(true)
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, PWMDutyValue);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    }
}

void readADCCore1(void *pvParameter)
{
    int adc_raw_value = 0;

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_EXAMPLE_CHAN0, ADC_EXAMPLE_ATTEN));

    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC2_EXAMPLE_CHAN0, ADC_EXAMPLE_ATTEN));

    while(true)
    {
        adc_raw_value = adc1_get_raw(ADC1_EXAMPLE_CHAN0) / 256 * (adc1_get_raw(ADC2_EXAMPLE_CHAN0) / 16);

        PWMDutyValue = (int)(adc_raw_value) >> 4;
    }
}

void app_main(void)
{
    xTaskCreate(&readADCCore1, "core1_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreatePinnedToCore(&writePWMCore0, "core0_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL, 1);
}