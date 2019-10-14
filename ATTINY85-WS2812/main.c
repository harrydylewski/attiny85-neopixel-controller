/*
 * ATTINY85-WS2812.c
 *
 * Created: 02/11/2018 6:50:58 PM
 * Author : Harry
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include "output_progmem.h"

#define PIN(x) (1<<x)
#define BIT(x) (1<<x)

#define MAX_PIXELS 90

void peripheral_init(void);
extern void pixel_tx_progmem(unsigned int, const unsigned char*,const unsigned char*,const unsigned char*);
extern void pixel_tx_singlecolor(unsigned int, unsigned char,unsigned char,unsigned char);



typedef enum {
	MODE_RESET,
	MODE_CROSSFADE,
	MODE_RAINBOW,	
	MODE_BOUNDS
} mode_state;

typedef enum {
	INC_GREEN,
	DEC_RED,
	INC_BLUE,
	DEC_GREEN,
	INC_RED,
	DEC_BLUE,
	RGBSWITCH_RESET	
} rgbswitch_state;

typedef enum {
	MS16,
	MS32,
	MS64,
	MS128,
	MS256,
	SPEED_MAX
} wdt_state;

typedef enum {
	BTN3      = PB0,
	BTN2      = PB1,
	BTN_MODE  = PB2,
	BTN_SPEED = PB3
} button_state;

#define OUTPUT_PIN PB4

//---------------------------------------
typedef struct {
	uint8_t  green;
	uint8_t  red;
	uint8_t  blue;
} rgb;

typedef struct {
	uint8_t speed;
	uint8_t mode;
} control;

typedef struct {
	uint8_t    rgbswitch;
	uint16_t   color;
	uint16_t   ring;
} counters;
//---------------------------------------

control EEMEM save;
volatile uint8_t read_pinb;

ISR(WDT_vect)
{
}


ISR(PCINT0_vect)
{

	read_pinb = PINB;

}

int main(void)
{
	//peripherals setup for lowest power possible
	peripheral_init();
	
	//vars for controlling state of leds
	control setting={1};
	rgb neopixel={0,255,0};		
	counters cnt={0};
		
	eeprom_read_block(&save,&setting,sizeof(control));
	
	//if EEPROM settings are invalid then write a default value
	if(setting.speed >= SPEED_MAX)
	{
		setting.speed = MS16;
	}
	
	if(setting.mode>=MODE_BOUNDS)
	{
		setting.speed=MODE_CROSSFADE;
	}
	
	//save valid settings	
	eeprom_update_block(&save,&setting,sizeof(control));	
	 
    while (1) 
    {	
		
		cli();
			
		wdt_disable();
		wdt_reset();
		
		
		if( ! (read_pinb&PIN(BTN_SPEED)) )
		{
			//taking advantage of underflow
			if(--setting.speed >= SPEED_MAX )
			setting.speed = MS256;
			
		}
	
		if( ! (read_pinb&PIN(BTN_MODE)) )
		{
				if(--setting.mode==MODE_RESET)
				setting.speed=MODE_CROSSFADE;
		}
	
		
		eeprom_update_block(&save,&setting,sizeof(control));
		
		if(setting.mode)
		{
				pixel_tx_progmem(MAX_PIXELS,(output2+cnt.ring),output2, output2+sizeof(output2));
				
				cnt.ring+=3;
				if(cnt.ring>=sizeof(output2)-3)
				cnt.ring=0;
		}	
		//output colors to ws2812 using a ring buffer 
		
		else
		{
			pixel_tx_singlecolor(MAX_PIXELS,neopixel.green,neopixel.red,neopixel.blue);
			
			
			if(++cnt.color>=256)
			{
				cnt.color=0;
				
				cnt.rgbswitch++;
				if(cnt.rgbswitch>=RGBSWITCH_RESET)
				cnt.rgbswitch=0;
			}
	
			switch(cnt.rgbswitch){
			case INC_GREEN : neopixel.green++;  break;
			case DEC_RED   : neopixel.red--  ;  break;
			case INC_BLUE  : neopixel.blue++ ;  break;
			case DEC_GREEN : neopixel.green--;  break;
			case INC_RED   : neopixel.red++  ;  break;
			case DEC_BLUE  : neopixel.blue-- ;  break;
			}
		}
							
		
		wdt_enable(setting.speed);
		WDTCR |= BIT(WDIE);
			
		sei();			
		
		sleep_enable();
		sleep_cpu();
		sleep_disable();		
					
    }
	
}

void peripheral_init(void)
{

	DDRB = PIN(OUTPUT_PIN) | PIN(PB1) | PIN(PB0) ;
	PORTB = PIN(BTN_MODE) | PIN(BTN_SPEED);
	
	PCMSK = PIN(BTN_MODE) | PIN(BTN_SPEED);
	GIMSK = BIT(PCIE); //enable PCINT0 interrupt
	
	ACSR  = BIT(ACD); //disable analog comparator
	
	PRR = BIT(PRTIM1) | BIT(PRUSI) | BIT(PRADC) |BIT(PRTIM0);
	//disable all unused peripherals

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}
