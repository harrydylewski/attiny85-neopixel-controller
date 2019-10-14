#include <avr/io.h>


	.file "pixel_tx.s"

	.text
	.global pixel_tx

pixel_tx:
	ldi R25,24

start:
	lsl R20 ;1
	rol R22 ;2
	rol R24 ;3
	brcs longexit ;4 FOR FALSE/5 FOR TRUE

quickexit:

	sbi _SFR_IO_ADDR(PORTB),4 ;2
	dec R25    ;3
	cbi _SFR_IO_ADDR(PORTB),4 ;2
	brne start
	rjmp end

longexit: 

	sbi _SFR_IO_ADDR(PORTB),4 ;2

	dec R25    ;3
	cbi _SFR_IO_ADDR(PORTB),4 ;5
	breq start

end:
	ret