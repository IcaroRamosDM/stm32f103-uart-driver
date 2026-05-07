.syntax unified
.cpu cortex-m3
.thumb

.global reset_handler
.global default_handler
.global sys_tick_handler

_estack = 0x20005000

.section .vectors, "a", %progbits
.type vector_table, %object

vector_table:
  .word _estack
  .word reset_handler
  .word default_handler
  .word default_handler
  .word default_handler
  .word default_handler
  .word default_handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word default_handler
  .word default_handler
  .word 0
  .word default_handler
  .word sys_tick_handler

.text

.type reset_handler, %function
reset_handler:
  bl main

reset_loop:
  b reset_loop

.type default_handler, %function
default_handler:
  b default_handler
