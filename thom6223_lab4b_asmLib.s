.equ __P24FJ64GA002,1		;BP
.include "p24Fxxxx.inc"		;BP
#include "xc.inc"		;BP
.text				;Put the following data in ROM

.global _write_0, _write_1, _jat_wait_50us, _jat_wait_1ms
    
_write_0:		;20 cycle period
    bset LATA, #0		; Set LATA high
    repeat #3		
    nop
    clr LATA		; Set LATA low
    repeat #6		
    nop
    return
    
_write_1:		; 20 cycle period
    bset LATA, #0	; Set LATA high
    repeat #8
    nop
    clr LATA		; Set LATA low
    repeat #1		
    nop
    return
    
_jat_wait_50us:		
    repeat #793	
    nop
    return

_jat_wait_1ms:
    repeat #15993
    nop
    return
    