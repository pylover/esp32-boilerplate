#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


void
app_main(void) {

    printf("ESP32 Boilerplate\n");
#ifdef DEBUG
    printf("ESP32 DEBUG build #2\n");
#endif

    printf("Rebooting in 5 seconds...\n");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}
