CC = gcc
CFLAGS = -Wall -g -pthread -DPROJ_LINUX_SIM

# Yollar
FREERTOS_DIR = ./FreeRTOS
FREERTOS_SRC = $(FREERTOS_DIR)
FREERTOS_INC = $(FREERTOS_DIR)/include
FREERTOS_PORT_DIR = $(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix
FREERTOS_MEMMANG_DIR = $(FREERTOS_DIR)/portable/MemMang

INCLUDES = -I. -I./include -I$(FREERTOS_INC) -I$(FREERTOS_PORT_DIR)

# Kaynak Dosyalar
MY_SOURCES = main.c scheduler.c

# FreeRTOS Kaynakları (+heap_3.c eklendi!)
FREERTOS_SOURCES = $(FREERTOS_SRC)/tasks.c \
                   $(FREERTOS_SRC)/list.c \
                   $(FREERTOS_SRC)/queue.c \
                   $(FREERTOS_SRC)/timers.c \
                   $(FREERTOS_SRC)/event_groups.c \
                   $(FREERTOS_SRC)/stream_buffer.c \
                   $(FREERTOS_PORT_DIR)/port.c \
                   $(FREERTOS_PORT_DIR)/utils/wait_for_event.c \
                   $(FREERTOS_MEMMANG_DIR)/heap_3.c

SOURCES = $(MY_SOURCES) $(FREERTOS_SOURCES)
OUTPUT = freertos_sim

all:
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
