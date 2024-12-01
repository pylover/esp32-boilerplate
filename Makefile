PORT = /dev/ttyACM0
ifeq ("$(wildcard $(PORT))","")
  PORT = /dev/ttyUSB0
endif


all: build


.PHONY: menu
menu:
	idf.py menuconfig


.PHONY: build
build:
	idf.py build


.PHONY: flash
flash:
	idf.py flash --port $(PORT)


.PHONY: screen
screen:
	screen $(PORT) 115200


.PHONY: test
test:
	pytest --target esp32 tests/


.PHONY: clean
clean:
	-rm -rf tests/pytest-embedded
	idf.py clean


.PHONY: fullclean
fullclean:
	idf.py fullclean
