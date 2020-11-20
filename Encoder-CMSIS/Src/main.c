#include "main.h"

/* Global Variables */
uint8_t Enc_Counter = 0;

/* Function prototypes */
void Enc_Trig_Int(void);
void GPIO_Initialization(void);

int main(void)
{

	GPIO_Initialization();

	/*
	 * PA6 - TIM3_CH1
	 * PA7 - TIM3_CH1
	 */

	/****** GPIO Initialization ******/

	/* GPIOA Clock */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	/* 10: Alternate function mode */
	GPIOA->MODER &= ~GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER6_1;

	/* 00: No pull-up, pull-down */
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR6_0;
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR6_1;

	/* 0001: AF1 */
	GPIOA->AFR[0] |= (1UL << GPIO_AFRL_AFSEL6_Pos);


	/* 10: Alternate function mode */
	GPIOA->MODER &= ~GPIO_MODER_MODER7_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_1;

	/* 00: No pull-up, pull-down */
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR7_0;
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR7_1;

	/* 0001: AF1 */
	GPIOA->AFR[0] |= (1UL << GPIO_AFRL_AFSEL7_Pos);


	/* Encoder Initialization */

	/* TIM3 Clock */
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	/* 01: CC1 channel is configured as input, IC1 is mapped on TI1
	 * 01: CC2 channel is configured as input, IC2 is mapped on TI2 */
	TIM3->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
	TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC2S_1);

	/* 00: noninverted/rising edge */
	TIM3->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
	TIM3->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2NP);

	/* 001: Encoder mode 1 - Counter counts up/down on TI2FP1 edge depending on TI1FP2 level */
	TIM3->SMCR |= TIM_SMCR_SMS_0;
	TIM3->SMCR &= ~TIM_SMCR_SMS_1;
	TIM3->SMCR &= ~TIM_SMCR_SMS_2;

	/* 1111: fSAMPLING = fDTS / 32, N = 8 */
	TIM3->CCMR1 |= (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
	TIM3->CCMR1 |= (TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);

	/* Auto-Reload Register (MAX counter number) */
	TIM3->ARR = 30;

	Enc_Trig_Int();

	/* 1: Counter enabled */
	TIM3->CR1 |= TIM_CR1_CEN;


while(1){

	Enc_Counter = TIM3->CNT;

	}
}

void Enc_Trig_Int(void){

	/* Trigger Edge Detector */

	/* 0000: No filter, sampling is done at fDTS */
	TIM3->SMCR &= ~(TIM_SMCR_ETF);

	/* 100: TI1 Edge Detector (TI1F_ED) */
	TIM3->SMCR &= ~(TIM_SMCR_TS_0 | TIM_SMCR_TS_1);
	TIM3->SMCR |= TIM_SMCR_TS_2;

	/* 1: Trigger interrupt enabled. */
	TIM3->DIER |= TIM_DIER_TIE;

	NVIC_EnableIRQ(TIM3_IRQn);
}

void GPIO_Initialization(void){

  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* GPIOA Clock */

  GPIOA->MODER |= GPIO_MODER_MODER4_0; /* 01: General purpose output mode */
  GPIOA->MODER &= ~GPIO_MODER_MODER4_1;

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4; /* 0: Output push-pull */

  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_0; /* 01: Medium speed */
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4_1;

  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_0; /* 00: No pull-up, pull-down */
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4_1;

  GPIOA->BSRR |= GPIO_BSRR_BS_4; /* Low Level Out */
}


void TIM3_IRQHandler(void){

	if(TIM3->SR & TIM_SR_TIF){

		/* Toggle LED (PA4 pin) */
		GPIOA->ODR ^= GPIO_ODR_4;

		/* Interrupt enabled */
		TIM3->SR &= ~TIM_SR_TIF;
	}

}
