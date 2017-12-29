; Z80 calling convention description.
; Parameters are passed right to left.  As the stack grows downwards,
; the parameters are arranged in left to right in memory.
;
; Everything is caller saves. i.e. the caller must save any registers
; that it wants to preserve over the call, except for ix, which is callee saves.
; GB: The return value is returned in DEHL.  DE is normally used as a
; working register pair.  Caller saves allows it to be used for a
; return value.
; va args functions do not use register parameters.  All arguments
; are passed on the stack.
; IX is used as an index register to the top of the local variable
; area.  ix-0 is the top most local variable.

; On a MSX @ 3.58Mhz (Timing Z80+M1)
; 3580 cycles = 1ms


.area _CODE

_msx_wait_jiffies::
    push ix

    ld ix, #0
    add ix, sp

    ld c, 4(ix)
    ld b, 5(ix)

    ld hl, #0xFC9E

$1:
    ld a, (hl)
 $2:
    ld e, (hl)
    cp e
    jp z, $2    
    dec bc
    ld a, b
    or c
    jp nz, $1

    pop ix

    ret

