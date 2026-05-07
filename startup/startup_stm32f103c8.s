.syntax unified
.cpu cortex-m3
.thumb

.global reset_handler
.global default_handler
.global sys_tick_handler
.global usart1_isr

.weak sys_tick_handler
.thumb_set sys_tick_handler, default_handler

.weak usart1_isr
.thumb_set usart1_isr, default_handler

_estack = 0x20005000

.section .vectors, "a", %progbits
.type vector_table, %object

vector_table:
  .word _estack              /* Initial stack pointer */
  .word reset_handler        /* Reset */
  .word default_handler      /* NMI */
  .word default_handler      /* HardFault */
  .word default_handler      /* MemManage */
  .word default_handler      /* BusFault */
  .word default_handler      /* UsageFault */
  .word 0                    /* Reserved */
  .word 0                    /* Reserved */
  .word 0                    /* Reserved */
  .word 0                    /* Reserved */
  .word default_handler      /* SVCall */
  .word default_handler      /* Debug monitor */
  .word 0                    /* Reserved */
  .word default_handler      /* PendSV */
  .word sys_tick_handler     /* SysTick */

  .word default_handler      /* WWDG */
  .word default_handler      /* PVD */
  .word default_handler      /* TAMPER */
  .word default_handler      /* RTC */
  .word default_handler      /* FLASH */
  .word default_handler      /* RCC */
  .word default_handler      /* EXTI0 */
  .word default_handler      /* EXTI1 */
  .word default_handler      /* EXTI2 */
  .word default_handler      /* EXTI3 */
  .word default_handler      /* EXTI4 */
  .word default_handler      /* DMA1 Channel 1 */
  .word default_handler      /* DMA1 Channel 2 */
  .word default_handler      /* DMA1 Channel 3 */
  .word default_handler      /* DMA1 Channel 4 */
  .word default_handler      /* DMA1 Channel 5 */
  .word default_handler      /* DMA1 Channel 6 */
  .word default_handler      /* DMA1 Channel 7 */
  .word default_handler      /* ADC1/ADC2 */
  .word default_handler      /* USB HP / CAN TX */
  .word default_handler      /* USB LP / CAN RX0 */
  .word default_handler      /* CAN RX1 */
  .word default_handler      /* CAN SCE */
  .word default_handler      /* EXTI9_5 */
  .word default_handler      /* TIM1 Break */
  .word default_handler      /* TIM1 Update */
  .word default_handler      /* TIM1 Trigger/Commutation */
  .word default_handler      /* TIM1 Capture Compare */
  .word default_handler      /* TIM2 */
  .word default_handler      /* TIM3 */
  .word default_handler      /* TIM4 */
  .word default_handler      /* I2C1 Event */
  .word default_handler      /* I2C1 Error */
  .word default_handler      /* I2C2 Event */
  .word default_handler      /* I2C2 Error */
  .word default_handler      /* SPI1 */
  .word default_handler      /* SPI2 */
  .word usart1_isr           /* USART1 */

.text

.type reset_handler, %function
reset_handler:
  bl main

reset_loop:
  b reset_loop

.type default_handler, %function
default_handler:
  b default_handler
