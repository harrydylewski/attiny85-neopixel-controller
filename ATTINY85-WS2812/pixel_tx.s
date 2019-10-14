#include <avr/io.h>


	.file "pixel_tx.s"

	.text
	.global pixel_tx

	.global pixel_tx_progmem
	.global pixel_tx_singlecolor

#define pixelport  PORTB
#define pixelpin   PB4
/*
pixel_tx:
	ldi R25,24

start:
	lsl R20 ;1
	rol R22 ;2
	rol R24 ;3
	brcs longexit ;4 FOR FALSE/5 FOR TRUE

quickexit:

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	dec R25    ;3
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	brne start
	rjmp end

longexit: 

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	dec R25  ;3
	rjmp delay ;5
delay:
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	brne start ;4

end:
	ret
*/

#define BITCOUNTER R22
#define RED        R26
#define GREEN      R27
#define BLUE       R23

pixel_tx_progmem:
	
	//r25:24 = amount of pixel
	//r23:22  = ptr+start offset
	//r21:20  = ptr
	//r19:18  = total bytes of array + ptr
	movw R30,R22

init1:
	lpm GREEN,z+ //g
	lpm RED,z+ //r
	lpm BLUE,z+ //b

	ldi BITCOUNTER,24

start1:
	lsl BLUE ;1
	rol RED ;2
	rol GREEN ;3
	brcs longexit1 ;4 FOR FALSE/5 FOR TRUE

quickexit1:

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	dec BITCOUNTER    ;3
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	brne start1
	rjmp nextpixel1

longexit1: 

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	dec BITCOUNTER ;3
	rjmp delay1 ;5

delay1:
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	brne start1 ;4

nextpixel1:
	cp R18,R30
	cpc R19,R31
	brne decrementpixel1
	movw R30,R20

decrementpixel1:
	sbiw R24,1
	brne init1

end1:
	ret

pixel_tx_singlecolor:
	//r25:24 = amount of pixels
	//r23:22  = green
	//r21:20  = red
	//r19:18  = blue

init2:
	ldi R23,24

start2:

	lsl r18 ;1
	rol r20 ;2
	rol r22 ;3
	brcs longexit1 ;4 FOR FALSE/5 FOR TRUE

quickexit2:

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	adc R18,R1
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	dec R23    ;3
	brne start2
	rjmp decrementpixel2
	

longexit2: 

	sbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	nop
	adc R18,R1
	dec R23;4
	

delay2:
	cbi _SFR_IO_ADDR(pixelport),pixelpin ;2
	brne start2 ;4

	
decrementpixel2:
	sbiw R24,1
	brne init2

end2:
	ret





/*

#include <avr/io.h>


	.file "pixel_tx.s"

	.text
	.global pixel_tx

.equ PIXEL_PORT PORTB 
.equ PIXEL_PIN  PB4

pixel_tx:
	ldi R25,24

start:
	lsl R20 ;1
	rol R22 ;2
	rol R24 ;3
	brcs longexit ;4 FOR FALSE/5 FOR TRUE

quickexit:

	sbi _SFR_IO_ADDR(PIXEL_PORT),PIXEL_PIN ;2
	dec R25    ;3
	cbi _SFR_IO_ADDR(PIXEL_PORT),PIXEL_PIN ;2
	brne start
	rjmp end

longexit: 

	sbi _SFR_IO_ADDR(PIXEL_PORT),PIXEL_PIN ;2

	dec R25    ;3
	cbi _SFR_IO_ADDR(PIXEL_PORT),PIXEL_PIN ;5
	breq start

end:
	ret
	*/