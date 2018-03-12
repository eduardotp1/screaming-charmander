/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

#include "asf.h"


/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO_ID 12
#define LED_PIO_PIN 17
#define LED_PIO_PIN_MASK (1<< LED_PIO_PIN)
#define LED_PIO PIOC

#define BUT_PIO PIOC
#define BUT_PIO_ID 12
#define BUT_PIO_PIN 19
#define BUT_PIO_PIN_MASK (1 << BUT_PIO_PIN)

#define BUZZ_PIO PIOC
#define BUZZ_PIO_ID 12
#define BUZZ_PIO_PIN 13
#define BUZZ_PIO_PIN_MASK (1 << BUZZ_PIO_PIN)

#define CHAMA_PIO PIOB
#define CHAMA_PIO_ID 11
#define CHAMA_PIO_PIN 2
#define CHAMA_PIO_PIN_MASK (1 << CHAMA_PIO_PIN)

volatile Bool but_flag = false;
volatile Bool alarm = false;


void but_callback(void){
	but_flag = true;
}

void alarm_callback(void){
	alarm = true;
}


// Funcao principal chamada na inicalizacao do uC.
int main(void){
	
	sysclk_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(CHAMA_PIO_ID);
	pio_configure(PIOC,PIO_OUTPUT_1, LED_PIO_PIN_MASK, PIO_PULLUP);
	pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_PIN_MASK, PIO_PULLUP);
	pio_configure(BUZZ_PIO, PIO_OUTPUT_0, BUZZ_PIO_PIN_MASK, PIO_DEFAULT);
	pio_configure(CHAMA_PIO, PIO_INPUT, CHAMA_PIO_PIN_MASK, PIO_DEFAULT);
	
	// super loop
	// aplicacoes embarcadas não devem sair do while(1). 
	
	pio_enable_interrupt(BUT_PIO, BUT_PIO_PIN_MASK);
	pio_handler_set(BUT_PIO,BUT_PIO_ID,BUT_PIO_PIN_MASK, PIO_IT_FALL_EDGE, but_callback);
	
	pio_enable_interrupt(CHAMA_PIO, CHAMA_PIO_PIN_MASK);
	pio_handler_set(CHAMA_PIO, CHAMA_PIO_ID, CHAMA_PIO_PIN_MASK, PIO_IT_FALL_EDGE, alarm_callback);
	
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID,1);
	
	NVIC_EnableIRQ(CHAMA_PIO_ID);
	NVIC_SetPriority(CHAMA_PIO_ID,0);
	
	
	
//pio_clear(PIOC, LED_PIO_PIN_MASK);
//delay_ms(2000);

//pio_set(PIOC, LED_PIO_PIN_MASK);
//delay_ms(2000);
	
	alarm = false;
	but_flag = false;
	while (1) {
		pio_set(LED_PIO, LED_PIO_PIN_MASK);
		pio_set(BUZZ_PIO, BUZZ_PIO_PIN_MASK);
		if(alarm){
			pio_clear(LED_PIO, LED_PIO_PIN_MASK);
			
			pio_clear(BUZZ_PIO, BUZZ_PIO_PIN_MASK);
			delay_ms(1);
			pio_set(BUZZ_PIO, BUZZ_PIO_PIN_MASK);
			delay_ms(1);
			
			if(but_flag){
				alarm = false;
				but_flag = false;
			}
		}

	}
	return 0;
}
