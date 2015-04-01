/*
 * DMX_dosencontroller.h
 *
 * Author: @dreimalbe
 */
#include "avr/io.h"
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define DOSE_CHANNEL 	46

#define D1_POWER		0x03
#define D1_POWER_0		0x00
#define D1_POWER_30		0x01
#define	D1_POWER_60		0x02
#define D1_POWER_100	0x03

#define D1_STROBE		0x0C
#define D1_STROBE_0		0x00
#define D1_STROBE_1		0x04
#define D1_STROBE_4		0x08
#define D1_STROBE_10	0x0C

#define D2_POWER		( D1_POWER << DOSE_CHANNEL )
#define D2_POWER_0		( D1_POWER_0 << DOSE_CHANNEL )
#define D2_POWER_30		( D1_POWER_30 << DOSE_CHANNEL )
#define	D2_POWER_60		( D1_POWER_60 << DOSE_CHANNEL )
#define D2_POWER_100	( D1_POWER_100 << DOSE_CHANNEL )

#define D2_STROBE		( D1_STROBE << DOSE_CHANNEL )
#define D2_STROBE_0		( D1_STROBE_0 << DOSE_CHANNEL )
#define D2_STROBE_1		( D1_STROBE_1 << DOSE_CHANNEL )
#define D2_STROBE_4		( D1_STROBE_4 << DOSE_CHANNEL )
#define D2_STROBE_10	( D1_STROBE_10 << DOSE_CHANNEL )

#define STROBE_OFF		0
#define STROBE_SLOW		30
#define STROBE_MID		15
#define STROBE_FAST		5

// Relais
#define NUM_RELAIS	5
#define R_PORT PORTB
#define R1_PIN (1<<PORTB0)
#define R2_PIN (1<<PORTB1)
#define R3_PIN (1<<PORTB2)
#define R4_PIN (1<<PORTB3)
#define R5_PIN (1<<PORTB4)
#define R_MASK ( R1_PIN | R2_PIN | R3_PIN | R4_PIN | R5_PIN )
#define CLEAR_ALL 	(PORTB &= ~R_MASK)

#define R_P1_RED	0
#define R_P2_WHITE	1


// LEDs
#define LED_PIN 					(1<<PORTD6)
#define SET_LED 	(PORTD |= LED_PIN)
#define CLEAR_LED 	(PORTD &= ~LED_PIN)

// PWM Settings
#define F_PWM 				100   	// PWM-Frequenz in Hz
#define PWM_STEPS 			200		// PWM-Schritte pro Zyklus(1..256)

#define FLASH_CNT			255


// LED settings
#define PWM_OFF			0
#define PWM_FULL		PWM_STEPS
#define PWM_HALF		(PWM_STEPS>>2)
#define PWM_QUART		(PWM_STEPS>>3)
#define PWM_MIN			(PWM_STEPS>>4)

#define LED_LATENCY 10000

struct relais_data {
	uint8_t pwm;
	uint8_t strobe;
	uint8_t enable;
	uint8_t PIN;
};

typedef struct relais_data relais_data_t;

// UART
#define BAUD 250000UL
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)
 #if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif


// PWM
#define T_PWM 					(F_CPU/(F_PWM*PWM_STEPS))
#if (T_PWM<(93+5))
    #error T_PWM zu klein, F_CPU muss vergrösst werden oder F_PWM oder PWM_STEPS verkleinert werden
#endif
