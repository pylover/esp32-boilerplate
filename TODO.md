- readme
- lint

[1/5] cd /home/vahid/workspace/esp32-boilerplate/build/esp-idf/esptool_py && 
/home/vahid/.espressif/python_env/idf5.4_py3.12_env/bin/python /home/vahid/esp/idf/components/partition_table/check_sizes.py --offset 0x8000 partition 
--type app /home/vahid/workspace/esp32-boilerplate/build/partition_table/partition-table.bin /home/vahid/workspace/esp32-boilerplate/build/boilerplate.bin
boilerplate.bin binary size 0x3e910 bytes. Smallest app partition is 0x100000 bytes. 0xc16f0 bytes (76%) free.
[2/5] Performing build step for 'bootloader'
[1/1] cd /home/vahid/workspace/esp32-boilerplate/build/bootloader/esp-idf/esptool_py && /home/vahid/.espressif/python_env/idf5.4_py3.12_env/bin/python /home/vahid/esp/idf/components/partition_table/check_sizes.py --offset 0x8000 bootloader 0x0 /home/vahid/workspace/esp32-boilerplate/build/bootloader/bootloader.bin
Bootloader binary size 0x62e0 bytes. 0x1d20 bytes (23%) free.
[3/5] No install step for 'bootloader'
[4/5] Completed 'bootloader'
[4/5] cd /home/vahid/esp/idf/components/esptool_py && /usr/bin/cmake -D IDF_PATH=/home/vahid/esp/idf -D "SERIAL_TOOL=/home/vahid/.espressif/python_env/idf5.4_py3.12_env/bin/python;;/home/vahid/esp/idf/components/esptool_py/esptool/esptool.py;--chip;esp32s3" -D "SERIAL_TOOL_ARGS=--before=default_reset;--after=hard_reset;write_flash;@flash_args" -D WORKING_DIRECTORY=/home/vahid/workspace/esp32-boilerplate/build -P /home/vahid/esp/idf/components/esptool_py/run_serial_tool.cmake
esptool.py --chip esp32s3 -p /dev/ttyACM0 -b 460800 
  --before=default_reset 
  --after=hard_reset 
  write_flash 
    --flash_mode dio 
    --flash_freq 80m 
    --flash_size 8MB 0x0 bootloader/bootloader.bin 0x10000 boilerplate.bin 0x8000 partition_table/partition-table.bin
esptool.py v4.8.0
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-S3 (QFN56) (revision v0.2)
Features: WiFi, BLE, Embedded PSRAM 2MB (AP_3v3)
Crystal is 40MHz
MAC: cc:8d:a2:0f:6c:c8
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00006fff...
Flash will be erased from 0x00010000 to 0x0004efff...
Flash will be erased from 0x00008000 to 0x00008fff...
SHA digest in image updated
Compressed 25312 bytes to 15021...
Writing at 0x00000000... (100 %)
Wrote 25312 bytes (15021 compressed) at 0x00000000 in 0.4 seconds (effective 493.9 kbit/s)...
Hash of data verified.
Compressed 256272 bytes to 132257...
Writing at 0x00010000... (11 %)
Writing at 0x0001c1a5... (22 %)
Writing at 0x0002427e... (33 %)
Writing at 0x0002aa38... (44 %)
Writing at 0x00030fbb... (55 %)
Writing at 0x0003ad43... (66 %)
Writing at 0x000419c7... (77 %)
Writing at 0x00047888... (88 %)
Writing at 0x0004e25b... (100 %)
Wrote 256272 bytes (132257 compressed) at 0x00010000 in 2.5 seconds (effective 827.6 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 103...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (103 compressed) at 0x00008000 in 0.0 seconds (effective 603.7 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Done

