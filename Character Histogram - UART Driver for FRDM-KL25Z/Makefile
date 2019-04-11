
CC := gcc

COMMON_CFLAGS := -Wall \
    -g \
    -O0 \
    -DUNIT_TEST \
    -Iutil/inc/

LINUX_CFLAGS := -DUNIT_TEST

KL25Z_CFLAGS := -mcpu=cortex-m0plus \
	-march=armv6-m \
    -mthumb \
    -mfloat-abi=soft \
    -mfpu=fpv4-sp-d16 \
    -specs=nosys.specs \
    -fmessage-length=0 \
    -fsigned-char \
    -ffunction-sections \
    -fdata-sections\
	-ISDK/platform/CMSIS/Include \
	-ISDK/platform/devices \
	-ISDK/platform/devices/MKL25Z4/include \
	-IProject_Settings/Startup_Code/ \
	-D"CPU_MKL25Z128VLK4"

KL25Z_LDFLAGS = -TProject_Settings/Linker_Files/MKL25Z128xxx4_flash.ld

COMMON_SRC := circular_buffer.c
TEST_SRC := cunit.c
KL25Z_C_SRC := system_MKL25Z4.c main.c startup.c
KL25Z_S_SRC := startup_MKL25Z4.S

vpath %.c util/src/
vpath %.c Project_Settings/Startup_Code/
vpath %.c Sources/
vpath %.h util/inc/
vpath %.h Project_Settings/Startup_Code/
vpath %.h SDK/platform/CMSIS/Include/
vpath %.h SDK/platform/devices/
vpath %.h SDK/platform/devices/MKL25Z4/include/


ifeq ($(PLATFORM), LINUX)
CC = gcc
CFLAGS = $(COMMON_CFLAGS) $(LINUX_CFLAGS)
SRC = $(COMMON_SRC) $(TEST_SRC)
LIBS := -lcunit
LDFLAGS =

else ifeq ($(PLATFORM), KL25Z)
CC = arm-none-eabi-gcc
CFLAGS = $(COMMON_CFLAGS) $(KL25Z_CFLAGS)
LDFLAGS = $(KL25Z_LDFLAGS)

else ifeq ($(PLATFORM), BB)
CC = arm-linux-gnueabi-gcc
CFLAGS = $(COMMON_CFLAGS) $(LINUX_CFLAGS) -DBB
SRC = $(COMMON_SRC) main.c
LIBS =
LDFLAGS =
else
    #-@echo 'PLATFORM not compatible '
    #-@echo ' '
endif

ifeq ($(PLATFORM), KL25Z)

OBJ := $(COMMON_SRC:.c=.o) $(KL25Z_C_SRC:.c=.o) 

test.elf: $(OBJ) startup_MKL25Z4.o
	$(CC) $(CFLAGS) $(LDFLAGS) -Xlinker -Map=test.map -o test.elf $(OBJ) startup_MKL25Z4.o

startup_MKL25Z4.o: Project_Settings/Startup_Code/startup_MKL25Z4.S
	$(CC) $(CFLAGS) -c $< -o $@

else

OBJ := $(SRC:.c=.o)

test: $(OBJ)
	$(CC) -o test $(OBJ) $(CFLAGS) $(LIBS)
endif

clean:
	rm -f test *.o *.map *.elf
