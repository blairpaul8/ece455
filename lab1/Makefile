TIVAWARE = ../ti_sdk
PART     = TM4C123GH6PM

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
         -DPART_$(PART) -DTARGET_IS_TM4C123_RB1 -c \
         -I$(TIVAWARE) \
         -ffunction-sections -fdata-sections -MD -std=c99 -Wall

LDFLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
          -nostartfiles -static -Wl,--gc-sections \
          -T tm4c123g.ld \
          -L$(TIVAWARE)/driverlib/gcc

LIBS = -ldriver -lm -lc -lgcc

OBJS = main.o startup_gcc.o IO.o PortF.o

all: blink.bin

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<


blink.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

blink.bin: blink.elf
	$(OBJCOPY) -O binary $< $@

flash: blink.elf
	openocd -f board/ti_ek-tm4c123gxl.cfg -c "program blink.elf verify reset exit"

clean:
	rm -f *.o *.d *.elf *.bin

.PHONY: all flash clean
