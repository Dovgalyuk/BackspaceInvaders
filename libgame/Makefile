# try: make help
#

ifeq ($(OS),Windows_NT)
include Makefile-win32
else

include /usr/share/arduino/Arduino.mk
ARDUINO_DIR = /usr/share/arduino
# list boards with: make show_boards
BOARD_TAG = uno
ARDUINO_LIBS = EEPROM 
# MONITOR_CMD = picocom -b 9600
MONITOR_CMD = moni -config ACM0-9600.cfg
#ARDUINO_PORT = /dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00
# ARDUINO_PORT = /dev/ttyACM0
# ARDUINO_PORT = /dev/ttyUSB0
#OPTIMIZATION_FLAGS = -Os -fexpensive-optimizations -fstrength-reduce
OPTIMIZATION_FLAGS = -O3 -fexec-charset=windows-1251

endif
