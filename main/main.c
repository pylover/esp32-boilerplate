#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/uart_vfs.h>
#include <driver/uart.h>
#include <esp_psram.h>
#include <esp_vfs_dev.h>

#include <uaio.h>
#include <elog.h>
#include <euart.h>
#include <ush.h>

#include "adc.h"
#include "gpio.h"


// static struct euart_device debug;
static struct euart_device console;
static struct ush *ush;


ASYNC
helloA(struct uaio_task *self, struct ush_process *p) {
    UAIO_BEGIN(self);
    ush_printf(p, "Hello World!\n");
    UAIO_FINALLY(self);
}


struct ush_executable commands[] = {
    {"hello", helloA},
    {"adc", adcA},
    {"gpio", gpioA},
    {NULL}
};


void
app_main(void) {
    /* Register vfs uart driver: /dev/uartN */
    uart_vfs_dev_register();
    elog_verbosity = ELOG_DEBUG;

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    if (euart_device_init(&console, &uart_config, UART_NUM_0, 43, 44,
                EUIF_NONBLOCK)) {
        ERROR("Cannot init UART #0");
        goto terminate;
    }
    INFO("UART #0 initialized successfully.");
    dprintf(console.outfd, "\033[m"ELOG_LF);

    /* psram */
    size_t psram_size = esp_psram_get_size();
    INFO("PSRAM size: %d bytes", psram_size);

    PRINT("\033[m"ELOG_LF);
    INFO("ESP32 Boilerplate!");

#ifdef ESPIDF_DEBUG
    DEBUG("DEBUG Mode: ON");
#endif

    ush = ush_create(&console, commands);
    if (ush == NULL) {
        ERROR("ush_create");
        goto terminate;
    }

    if (uaio_init(CONFIG_BOILERPLATE_TASKS_MAX)) {
        ERROR("Canno init uaio");
        goto terminate;
    }
    DEBUG("uaio initialized successfully");
    fflush(stdout);

    /* replace standard output files */
    stderr = stdout = fdopen(console.outfd, "a");

    ush_spawn(ushA, ush);
    uaio_loop();

terminate:
    ush_destroy(ush);
    uaio_destroy();

    INFO("Rebooting in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}
