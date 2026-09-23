.syntax unified
    .cpu cortex-m4
    .thumb

    .section .text
    .align 2

    .global DisableInterrupts
    .thumb_func
    .type DisableInterrupts, %function
DisableInterrupts:
    CPSID I
    BX LR

    .global EnableInterrupts
    .thumb_func
    .type EnableInterrupts, %function
EnableInterrupts:
    CPSIE I
    BX LR

    .global StartCritical
    .thumb_func
    .type StartCritical, %function
StartCritical:
    MRS R0, PRIMASK
    CPSID I
    BX LR

    .global EndCritical
    .thumb_func
    .type EndCritical, %function
EndCritical:
    MSR PRIMASK, R0
    BX LR

    .global WaitForInterrupt
    .thumb_func
    .type WaitForInterrupt, %function
WaitForInterrupt:
    WFI
    BX LR

    .end
