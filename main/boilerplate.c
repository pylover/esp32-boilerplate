#include <stdio.h>
#include <fcntl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/uart_vfs.h>
#include <driver/uart.h>

#include <elog.h>


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

    PRINT(ELOG_LF);
    INFO("ESP32 Boilerplate");
#ifdef ESPIDF_DEBUG
    DEBUG("ESP32 DEBUG build #2");
#endif
    // printf("%s:%d\n", basename(__FILE__), __LINE__);

    INFO("Rebooting in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}
