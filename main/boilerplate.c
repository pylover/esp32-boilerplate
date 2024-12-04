#include <stdio.h>
#include <fcntl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/uart_vfs.h>
#include <driver/uart.h>

#include <uaio.h>
#include <elog.h>

#include "foo.h"


static int
_uart_init(int no) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    if (uart_driver_install(no, 1024, 0, 0, NULL, 0) != ESP_OK) {
        return -1;
    }

    uart_param_config(no, &uart_config);
    uart_vfs_dev_use_driver(no);

    char path[16];
    sprintf(path, "/dev/uart/%d", no);
    int fd = open(path, O_RDWR);
    if (fd == -1) {
        ERROR("open(%s)", path);
        return -1;
    }

    return fd;
}


void
app_main(void) {
    elog_verbosity = ELOG_DEBUG;
    elog_errfd = elog_outfd = _uart_init(UART_NUM_0);
    struct foo foo = {
        .interval = 1000000,
    };

    PRINT(ELOG_LF);
    INFO("ESP32 Boilerplate");
    DEBUG("DEBUG Mode: ON");

    if (uaio_init(CONFIG_BOILERPLATE_TASKS_MAX)) {
        goto terminate;
    }
    DEBUG("uaio initialized successfully");

//     if (cuart_init(&sh.console, (struct caio_fdmon*)fdmon, UART_NUM_0, 43, 44,
//                 0)) {
//         ERROR("uart driver installation failed");
//         goto terminate;
//     }
//
//     if (cuart_init(&sh.debug, (struct caio_fdmon*)fdmon, UART_NUM_1, 6, 7,
//                 0)) {
//         ERROR("Cannot initialize UART1 for shell debug port.");
//         goto terminate;
//     }
//     INFO("UART driver installed successfully.");
//
//     cshell_spawn(_caio, cshellA, &sh);
//     // oras_spawn(_caio, orasA, &_oras);
//
    foo_spawn(fooA, &foo);
    uaio_loop();

terminate:
    uaio_destroy();

    INFO("Rebooting in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}
