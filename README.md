# ESP32 boilerplate


## Build
Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v5.4/esp32/get-started/index.html)
on `~/esp/idf`.

Then:

```bash
. activate.sh
idf.py set-target esp32s3
make menu
make flash monitor
```

Press `ctrl+]` to exit the monitor mode.


# Debug (OpenOCD + GDB)
Install `openocd` and `gdb` then:

```bash
make ocd
```

And run `make gdb` on another terminal to debug the firmware.

## References
- https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/jtag-debugging/configure-builtin-jtag.html#configure-usb-drivers
