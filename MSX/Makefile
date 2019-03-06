CC = /Applications/sdcc-3.8.0/bin/sdcc
ASM = /Applications/sdcc-3.8.0/bin/sdasz80
OBJCOPY = /Applications/sdcc-3.8.0/bin/sdobjcopy
RM = rm -f

SRC_C = conio.c mem.c #heap.c
SRC_ASM = crt0msx_msxdos_advanced.s dos2.s getchar.s ioport.s putchar.s crt0msx_msxdos.s dos.s interrupt.s keyboard.s vdp.s
RELOCATE = putchar.rel
OBJ_C = $(SRC_C:%.c=%.rel)
OBJ_ASM = $(SRC_ASM:%.s=%.rel)

PARAMS=-mz80 --no-std-crt0

%.rel : %.c
	${CC} ${PARAMS} $< ${RELOCATE}

%.rel : %.s
	${ASM} -l -o $<

all: $(OBJ_ASM) $(OBJ_C)

clean:
	${RM} *.asm *.com *.ihx *.lk *.lst *.map *.noi *.rel *.sym
