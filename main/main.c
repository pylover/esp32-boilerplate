#include <stdio.h>
#include <fcntl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/uart_vfs.h>
#include <driver/uart.h>

#include <uaio.h>
#include <elog.h>
#include <euart.h>
#include <ush.h>

#include "foo.h"


struct ush ush = {

};



void
app_main(void) {
    elog_verbosity = ELOG_DEBUG;
    // elog_errfd = elog_outfd = _uart_init(UART_NUM_0);
    PRINT(ELOG_LF);
    INFO("ESP32 Boilerplate");
    DEBUG("DEBUG Mode: ON");

    if (euart_init(&ush.console, UART_NUM_0, 43, 44, EUIF_STDIO)) {
        ERROR("Cannot init UART #0");
        goto terminate;
    }
    INFO("UART #0 installed successfully.");

    if (euart_init(&ush.debug, UART_NUM_1, 6, 7, 0)) {
        ERROR("Cannot init UART #1");
        goto terminate;
    }
    INFO("UART #1 installed successfully.");

    if (uaio_init(CONFIG_BOILERPLATE_TASKS_MAX)) {
        ERROR("Canno init uaio");
        goto terminate;
    }
    DEBUG("uaio initialized successfully");

    ush_spawn(ushA, &ush);
    uaio_loop();

terminate:
    uaio_destroy();

    INFO("Rebooting in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}
