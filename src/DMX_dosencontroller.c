/*
 * DMX_dosencontroller.c
 *
 * Author: @dreimalbe
 */
#include "DMX_dosencontroller.h"

#define DMX_CHANNEL 45
#define DMX_NUM_CHANNELS 1
#define LED_TIME 50000
#define SW_CNT_MAX 1000

volatile uint16_t _led_latency = LED_TIME;
volatile uint8_t _dmx_old_data[DMX_NUM_CHANNELS];
volatile uint8_t _dmx_data[DMX_NUM_CHANNELS];
volatile relais_data_t _relais_data[NUM_RELAIS];
volatile uint8_t _strobe_count[NUM_RELAIS];
volatile uint8_t _flag = 0;

ISR(TIMER1_COMPA_vect)
{
	static uint8_t pwm_cnt = 0;
	uint8_t i = 0;

	OCR1A += (uint16_t) T_PWM;

	for (i = 0; i < NUM_RELAIS; i++)
	{
		if (_relais_data[i].pwm > pwm_cnt)
		{
			if (_relais_data[i].enable)
			{
				if (_relais_data[i].strobe > STROBE_OFF)
				{
					_relais_data[i].enable--;
				}
				R_PORT |= _relais_data[i].PIN;
			}
			else
			{
				R_PORT &= ~(_relais_data[i].PIN);
			}
		}
		else
		{
			R_PORT &= ~(_relais_data[i].PIN);
		}
	}

	if (pwm_cnt >= (uint8_t)(PWM_STEPS - 1))
	{
		for (i = 0; i < NUM_RELAIS; i++)
		{
			if (_relais_data[i].strobe > 0)
			{
				if (_strobe_count[i] == 0 )
				{
					_strobe_count[i] = _relais_data[i].strobe;
					_relais_data[i].enable = FLASH_CNT;
				}
				else
				{
					_strobe_count[i]--;
				}
			}
			else
			{
				_relais_data[i].enable = 1;
				_strobe_count[i] = 0;
			}
		}
		pwm_cnt = 0;
	}
	else
	{
		pwm_cnt++;
	}
}

ISR(USART_RX_vect)
{
	static uint16_t channel;
	static uint8_t channel_num;
	uint8_t data = UDR;

	if (UCSRA & (1 << FE))
	{
		if (data == 0)
		{
			channel = 1;
		}
		else
		{
			channel++;
		}
		UCSRA &= ~(1 << FE);
	}
	else
	{
		if (channel == DMX_CHANNEL)
		{
			_led_latency = LED_TIME;
			channel_num = 0;
			_dmx_data[channel_num++] = data;
			_flag = 1;
		}
		else if (channel_num > DMX_NUM_CHANNELS)
		{
			_dmx_data[channel_num++] = data;
			_flag = 1;
		}
		channel++;
	}
}


void processData()
{
	uint8_t tmp = 0;
	uint8_t i = 0;
	for (i = 0; i < DMX_NUM_CHANNELS; i++)
	{
		if (_dmx_data[i] != _dmx_old_data[i])
		{
			if (i == 0)
			{
				if (_dmx_data[i] & D1_POWER)
				{
					if ((_dmx_data[i] & D1_POWER_100) == D1_POWER_100)
					{
						tmp = PWM_FULL;
					}
					else if ((_dmx_data[i] & D1_POWER_60) == D1_POWER_60)
					{
						tmp = PWM_HALF;
					}
					else if ((_dmx_data[i] & D1_POWER_30) == D1_POWER_30)
					{
						tmp = PWM_MIN;
					}
					_relais_data[R_P1_RED].pwm = tmp;

					if (_dmx_data[i] & D1_STROBE)
					{
						if ((_dmx_data[i] & D1_STROBE_10) == D1_STROBE_10)
						{
							tmp = STROBE_FAST;
						}
						else if ((_dmx_data[i] & D1_STROBE_4) == D1_STROBE_4)
						{
							tmp = STROBE_MID;
						}
						else if ((_dmx_data[i] & D1_STROBE_1) == D1_STROBE_1)
						{
							tmp = STROBE_SLOW;
						}
						_strobe_count[R_P1_RED] = 0;
						_relais_data[R_P1_RED].strobe = tmp;
					}
					else
					{
						_relais_data[R_P1_RED].strobe = STROBE_OFF;
					}
				}
				else
				{
					_relais_data[R_P1_RED].pwm = PWM_OFF;
				}

				if (_dmx_data[i] & D2_POWER)
				{
					if ((_dmx_data[i] & D2_POWER_100) == D2_POWER_100)
					{
						tmp = PWM_FULL;
					}
					else if ((_dmx_data[i] & D2_POWER_60) == D2_POWER_60)
					{
						tmp = PWM_HALF;
					} else if ((_dmx_data[i] & D2_POWER_30) == D2_POWER_30)
					{
						tmp = PWM_MIN;
					}
					_relais_data[R_P2_WHITE].pwm = tmp;

					if (_dmx_data[i] & D2_STROBE)
					{
						if ((_dmx_data[i] & D2_STROBE_10) == D2_STROBE_10)
						{
							tmp = STROBE_FAST;
						}
						else if ((_dmx_data[i] & D2_STROBE_4) == D2_STROBE_4)
						{
							tmp = STROBE_MID;
						}
						else if ((_dmx_data[i] & D2_STROBE_1) == D2_STROBE_1)
						{
							tmp = STROBE_SLOW;
						}
						_strobe_count[R_P2_WHITE] = 0;
						_relais_data[R_P2_WHITE].strobe = tmp;
					}
					else
					{
						_relais_data[R_P2_WHITE].strobe = STROBE_OFF;
					}
				}
				else
				{
					_relais_data[R_P2_WHITE].pwm = PWM_OFF;
				}
			}
			_dmx_old_data[i] = _dmx_data[i];
		}
	}
}

void init_uart()
{
	uint8_t tmp;

	UBRRH = UBRR_VAL >> 8;
	UBRRL = UBRR_VAL & 0xFF;

	do
	{
		tmp = UDR;
	}
	while (UCSRA & (1 << RXC));
	UDR = 0;
	UCSRA = 0;
	UCSRB = (1 << RXCIE) | (1 << RXEN);
	UCSRC = (3 << UCSZ0) | (1 << USBS);
}

void init_timer()
{
	TCCR1B = 1;
	OCR1A = 0;

	TIMSK |= (1 << OCIE1A);
}

void init_gpio() {
	DDRB |= (R_MASK);
	DDRD |= (LED_PIN);
}

void init_relais()
{
	uint8_t i = 0;
	_relais_data[R_P1_RED] = (relais_data_t) {0, 0, 0, R1_PIN};
	_relais_data[R_P2_WHITE] = (relais_data_t) {0, 0, 0, R2_PIN};
	for(i = 2; i < NUM_RELAIS; i++ )
	{
		_relais_data[i] = (relais_data_t) {0,0,0,0};
	}
}

void init_dmx()
{
	uint8_t i = 0;
	for (i = 0; i < DMX_NUM_CHANNELS; i++)
	{
		_dmx_data[i] = 0;
		_dmx_old_data[i] = 0;
	}
}

void standalone_fallback()
{
	_relais_data[R_P1_RED].pwm = PWM_FULL;
	_relais_data[R_P1_RED].strobe = STROBE_OFF;
	_relais_data[R_P2_WHITE].pwm = PWM_FULL;
	_relais_data[R_P2_WHITE].strobe = STROBE_OFF;
}

int main(void)
{
	init_gpio();
	init_uart();
	init_relais();
	init_dmx();
	init_timer();

	standalone_fallback();

	sei();

	for (;;)
	{
		if (_flag)
		{
			processData();
			_flag = 0;
		}
		if (_led_latency > 0)
		{
			_led_latency--;
			SET_LED;
		}
		else
		{
			CLEAR_LED;
		}
	}

	return 0;
}
