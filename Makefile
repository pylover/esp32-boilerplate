PORT = /dev/ttyACM0
ifeq ("$(wildcard $(PORT))","")
  PORT = /dev/ttyUSB0
endif


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
