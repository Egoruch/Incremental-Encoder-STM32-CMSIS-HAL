# Incremental-Encoder-STM32-CMSIS-HAL
 Connection rotary (incremental encoder) to STM32F030F4P6 with CMSIS and HAL and compare memory usage.
* Article (rus): https://cxemka.com/71-podklyuchenie-enkodera-dupa-k-stm32-cmsis-i-hal.html
* Youtube (rus): https://youtu.be/1CPk9UD4440
___
* STM32F030F4P6
* STM32CubeIDE
* CMSIS/HAL
 ## Connection diagram
  ![alt text](https://cxemka.com/upload/art/enc/stm32_encoder_connection_diagram.svg)
 ## Debug
   ![alt text]( https://cxemka.com/upload/art/enc/le_enc_counter.png)
 ## Encoder interrupt on change value
 
```
/* Trigger Edge Detector */

/* 0000: No filter, sampling is done at fDTS */
TIM3->SMCR &= ~(TIM_SMCR_ETF);

/* 100: TI1 Edge Detector (TI1F_ED) */
TIM3->SMCR &= ~(TIM_SMCR_TS_0 | TIM_SMCR_TS_1);

TIM3->SMCR |= TIM_SMCR_TS_2;

/* 1: Trigger interrupt enabled. */
TIM3->DIER |= TIM_DIER_TIE;

NVIC_EnableIRQ(TIM3_IRQn);
```
```
void TIM3_IRQHandler(void){
	if(TIM3->SR & TIM_SR_TIF){
		/* Here code to execute */
  
  
		/* Interrupt enabled */
		TIM3->SR &= ~TIM_SR_TIF;
	}
}
```
