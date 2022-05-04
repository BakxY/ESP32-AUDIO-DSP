# ESP32-AUDIO-DSP
This program is used to convert an analog signal to digital pwm signal. This digital signal can be passed onto a d aplifier to produce a sound. The d aplifier can be connected to a speaker or headphones. 

The program is written for an ESP32 in the ESP-IDF. It uses the analog inputs to read the signal and writes it with the `ledc.h` library. The frequency of the signal is defined in the program with the `LEDC_FREQUENCY` define. The resolution of the pwm signal is set to 8 bits, this could be increased up to 20 bits. The pwm signal is written to with the use of a task running on core 2 of the ESP32.

### Refrence
- [General ESP32 refrence](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [ledc.h](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)
- [adc.h](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
- [task.h](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#task-api)