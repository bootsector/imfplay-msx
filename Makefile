CC = /Applications/sdcc-3.6.0/bin/sdcc
ASM = /Applications/sdcc-3.6.0/bin/sdasz80
MAKE = make
OBJCOPY = gobjcopy
RM = rm -f

SRC_MAIN = imfplay.c
SRC_OTHERS = delay.c
SRC_ASM = jiffy.s

TARGET = $(SRC_MAIN:%.c=%)
OBJ_OTHERS = $(SRC_OTHERS:%.c=%.rel)
OBJ_ASM = $(SRC_ASM:%.s=%.rel)

INCLUDE=-I MSX
PARAMS=-mz80 --no-std-crt0
NORMAL=--data-loc 0 --code-loc 0x107 MSX/crt0msx_msxdos.rel
ADVANCED=--data-loc 0 --code-loc 0x178 MSX/crt0msx_msxdos_advanced.rel
RELOCATE=MSX/putchar.rel MSX/getchar.rel MSX/dos2.rel MSX/conio.rel MSX/ioport.rel

all: build_msx_lib build_main_advanced

%.rel : %.c
	${CC} ${INCLUDE} ${PARAMS} $< ${RELOCATE} $(OBJ_ASM)

%.rel : %.s
	${ASM} -l -o $<

build_msx_lib:
	cd MSX && $(MAKE) all

build_main_advanced: $(OBJ_ASM) $(OBJ_OTHERS) 
	${CC} ${SRC_MAIN} ${INCLUDE} ${PARAMS} ${ADVANCED} ${RELOCATE} $^
	${OBJCOPY} -I ihex ${TARGET}.ihx -O binary ${TARGET}.com

build_main_normal: $(OBJ_ASM) $(OBJ_OTHERS) 
	${CC} ${SRC_MAIN} ${INCLUDE} ${PARAMS} ${NORMAL} ${RELOCATE} $^
	${OBJCOPY} -I ihex ${TARGET}.ihx -O binary ${TARGET}.com
	
clean:
	${RM} *.asm *.com *.ihx *.lk *.lst *.map *.noi *.rel *.sym
	cd MSX && $(MAKE) clean
