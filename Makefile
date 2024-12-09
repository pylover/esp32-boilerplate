PORT = /dev/ttyACM0
ifeq ("$(wildcard $(PORT))","")
  PORT = /dev/ttyUSB0
endif


HERE = $(shell readlink -f `dirname .`)
PRJ = $(shell basename $(HERE))
REMOTE = rpi5
REMOTE_PATH = "/tmp/$(PRJ)"
BUILDPATH = $(HERE)/build


BUILDTYPE ?= debug
IDF_FLAGS = \
	-D ESPIDF_BUILDTYPE=$(BUILDTYPE)


all: build


.PHONY: menu
menu:
	idf.py $(IDF_FLAGS) menuconfig


.PHONY: build
build:
	idf.py $(IDF_FLAGS) build


.PHONY: flash
flash:
	idf.py $(IDF_FLAGS) flash --port $(PORT)


ESPTOOL = /opt/esp32/pyenv/bin/esptool.py
ESPTOOL_FLAGS = \
	--chip esp32s3 \
	-p /dev/ttyACM0 \
	-b 460800 \
	--before=default_reset \
	--after=hard_reset

ESPTOOL_FLASHFLAGS = \
	--flash_mode dio \
	--flash_freq 80m \
	--flash_size 2MB \
	0x0 $(REMOTE_PATH)/bootloader.bin \
	0x10000 $(REMOTE_PATH)/boilerplate.bin \
	0x8000 $(REMOTE_PATH)/partition-table.bin


.PHONY: rempte-flash
rempte-flash:
	ssh $(REMOTE) "mkdir -p $(REMOTE_PATH)"
	scp $(BUILDPATH)/bootloader/bootloader.bin \
		$(BUILDPATH)/boilerplate.bin \
		$(BUILDPATH)/partition_table/partition-table.bin \
		$(REMOTE):$(REMOTE_PATH)
	ssh $(REMOTE) \
		"$(ESPTOOL) $(ESPTOOL_FLAGS) write_flash $(ESPTOOL_FLASHFLAGS)"


.PHONY: screen
screen:
	screen $(PORT) 115200


.PHONY: test
test:
	pytest --target esp32s3 tests/


.PHONY: clean
clean:
	-rm -rf tests/pytest-embedded
	idf.py clean


.PHONY: fullclean
fullclean:
	idf.py fullclean
