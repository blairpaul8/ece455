# .gdbinit
target extended-remote localhost:3333
monitor halt
load
# ICDI probe's hardware reset (SRST) is currently broken - "monitor reset halt"
# fails with "Error: SRST error" and wedges the debug link. Work around it by
# loading without a reset and manually starting execution at the reset vector.
set $sp = *(unsigned int *)0x00000000
set $pc = *(unsigned int *)0x00000004

break main
