;/*****************************************************************************/
;/* OSasm.s: low-level OS commands, written in assembly                       */
;/* derived from uCOS-II                                                      */
;/*****************************************************************************/
;Jonathan Valvano, OS Lab2/3/4/5, 1/12/20
;Lab 6, ECE 455 / ECE 555, Fall 2026.
;StartOS, ContextSwitch and PendSV_Handler are GIVEN COMPLETE. Do not change this file.
;They need three things from your OS.c:
;  1) a global pointer RunPt to the TCB of the running thread;
;  2) the saved stack pointer as the FIRST field of the TCB (this file reads it at offset 0);
;  3) a C function void Scheduler(void) that sets RunPt to the next thread to run.
;StartOS launches the thread RunPt points to. ContextSwitch pends PendSV.
;PendSV_Handler saves R4-R11 and SP, calls Scheduler, and restores the new thread.
;Set PendSV to the lowest priority in OS_Launch.

        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8

        EXTERN  RunPt            ; currently running thread
        EXTERN  Scheduler        ; OS.c: picks the next RunPt

        EXPORT  StartOS
        EXPORT  ContextSwitch
        EXPORT  PendSV_Handler
        EXPORT  SVC_Handler


NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22                              ; PendSV priority register (position 14).
NVIC_SYSPRI15   EQU     0xE000ED23                              ; Systick priority register (position 15).
NVIC_LEVEL14    EQU           0xEF                              ; Systick priority value (second lowest).
NVIC_LEVEL15    EQU           0xFF                              ; PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.


;********************************************************************************************************
;                                         StartOS
; Load the first thread's stack pointer from RunPt->sp and pop the initial register
; frame that OS_AddThread built, so that BX LR "returns" into the thread's start
; address with interrupts enabled.
;********************************************************************************************************
StartOS
    LDR     R0, =RunPt         ; currently running thread
    LDR     R1, [R0]           ; R1 = value of RunPt
    LDR     SP, [R1]           ; new thread SP; SP = RunPt->sp
    POP     {R4-R11}           ; restore regs r4-11
    POP     {R0-R3}            ; restore regs r0-3
    POP     {R12}
    ADD     SP, SP, #4         ; discard LR from initial stack
    POP     {LR}               ; start location (the thread's PC slot)
    ADD     SP, SP, #4         ; discard PSR
    CPSIE   I                  ; enable interrupts at processor level
    BX      LR                 ; start first thread

OSStartHang
    B       OSStartHang        ; Should never get here


;********************************************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void ContextSwitch(void)
;
; Note(s) : 1) ContextSwitch() is called when OS wants to perform a task context switch.  This function
;              triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

ContextSwitch
    LDR     R0, =NVIC_INT_CTRL ; pend PendSV; it runs as soon as no higher priority exception is active
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR


;********************************************************************************************************
;                                         HANDLE PendSV EXCEPTION
;                                     void OS_CPU_PendSVHandler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
;              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCur      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdy  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;********************************************************************************************************
; This kernel runs every thread on the main stack, so the frame the
; processor pushed on entry sits on the outgoing thread's own stack.

PendSV_Handler
    CPSID   I                  ; 2) prevent interrupt during switch
    PUSH    {R4-R11}           ; 3) save remaining regs r4-11
    LDR     R0, =RunPt         ; 4) R0 = pointer to RunPt, old thread
    LDR     R1, [R0]           ;    R1 = RunPt
    STR     SP, [R1]           ; 5) save SP into TCB: RunPt->sp = SP
    PUSH    {R0, LR}           ;    keep R0 and the exception-return LR across the C call
    BL      Scheduler          ; 6) OS.c chooses the next thread: RunPt = ...
    POP     {R0, LR}
    LDR     R1, [R0]           ; 7) R1 = the new RunPt
    LDR     SP, [R1]           ; 8) new thread SP; SP = RunPt->sp
    POP     {R4-R11}           ; 9) restore regs r4-11
    CPSIE   I                  ; 10) tasks run with interrupts enabled
    BX      LR                 ; 11) Exception return will restore remaining context


;********************************************************************************************************
;                                         HANDLE SVC EXCEPTION
;                                     void OS_CPU_SVCHandler(void)
;
; Note(s) : SVC is a software-triggered exception to make OS kernel calls from user land.
;           The function ID to call is encoded in the instruction itself, the location of which can be
;           found relative to the return address saved on the stack on exception entry.
;           Function-call paramters in R0..R3 are also auto-saved on stack on exception entry.
;********************************************************************************************************

        IMPORT    OS_Id
        IMPORT    OS_Kill
        IMPORT    OS_Sleep
        IMPORT    OS_Time
        IMPORT    OS_AddThread

SVC_Handler
; not used in this lab (Lab 5 of the original sequence)


    BX      LR                   ; Return from exception



    ALIGN
    END
