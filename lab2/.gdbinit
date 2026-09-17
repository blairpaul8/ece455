# .gdbinit
target extended-remote localhost:3333
monitor cortex_m maskisr on
monitor reset halt
load

break main
