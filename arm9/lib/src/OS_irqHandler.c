#include "function_target.h"
#include "OS_irqHandler.h"
#include "OS_system.h"
#include "OS_thread.h"
#include "sections.h"
#include "CP_context.h"

OSThreadQueue OSi_IrqThreadQueue = { NULL, NULL };

#ifndef __GNUC__
#pragma section ITCM begin
#endif
#ifdef __GNUC__
SECTION_ITCM NAKED
ARM_FUNC void OS_IrqHandler(void)
{
    asm("stmfd sp!, {lr}\n\
         mov ip, #0x04000000\n\
         add ip, ip, #0x210\n\
         ldr r1, [ip, #-8]\n\
         cmp r1, #0\n\
         ldmeqfd sp!, {pc}\n\
         ldmia ip, {r1, r2}\n\
         ands r1, r1, r2\n\
         ldmeqfd sp!, {pc}\n\
         mov r3, #0x80000000\n\
    _01FF8028:\n\
         clz r0, r1\n\
         bics r1, r1, r3, lsr r0\n\
         bne _01FF8028\n\
         mov r1, r3, lsr r0\n\
         str r1, [ip, #0x4]\n\
         rsbs r0, r0, #0x1f\n\
         ldr r1, =OS_IRQTable\n\
         ldr r0, [r1, r0, lsl #2]\n\
         ldr lr, =OS_IrqHandler_ThreadSwitch\n\
         bx r0\n\
         .pool");
}
#else
ARM_FUNC asm void OS_IrqHandler(void)
{
    stmfd sp!, {lr}
    mov ip, #0x04000000
    add ip, ip, #0x210
    ldr r1, [ip, #-8]
    cmp r1, #0
    ldmeqfd sp!, {pc}
    ldmia ip, {r1, r2}
    ands r1, r1, r2
    ldmeqfd sp!, {pc}
    mov r3, #0x80000000
_01FF8028:
    clz r0, r1
    bics r1, r1, r3, lsr r0
    bne _01FF8028
    mov r1, r3, lsr r0
    str r1, [ip, #0x4]
    rsbs r0, r0, #0x1f
    ldr r1, =OS_IRQTable
    ldr r0, [r1, r0, lsl #2]
    ldr lr, =OS_IrqHandler_ThreadSwitch
    bx r0
}
#endif

#ifdef __GNUC__
NAKED SECTION_ITCM
ARM_FUNC void OS_IrqHandler_ThreadSwitch(void)
{
    asm("ldr ip, =OSi_IrqThreadQueue\n\
         mov r3, #0x0\n\
         ldr ip, [ip]\n\
         mov r2, #0x1\n\
         cmp ip, #0x0\n\
         beq _01FF80A8\n\
    _01FF8070:\n\
         str r2, [ip, #0x64]\n\
         str r3, [ip, #0x78]\n\
         str r3, [ip, #0x7c]\n\
         ldr r0, [ip, #0x80]\n\
         str r3, [ip, #0x80]\n\
         mov ip, r0\n\
         cmp ip, #0x0\n\
         bne _01FF8070\n\
         ldr ip, =OSi_IrqThreadQueue\n\
         str r3, [ip]\n\
         str r3, [ip, #0x4]\n\
         ldr ip, =OSi_ThreadInfo\n\
         mov r1, #0x1\n\
         strh r1, [ip]\n\
    _01FF80A8:\n\
         ldr ip, =OSi_ThreadInfo\n\
         ldrh r1, [ip]\n\
         cmp r1, #0x0\n\
         ldreq pc, [sp], #0x4\n\
         mov r1, #0x0\n\
         strh r1, [ip]\n\
         mov r3, #0xd2\n\
         msr CPSR_c, r3\n\
         add r2, ip, #0x8\n\
         ldr r1, [r2]\n\
    _01FF80D0:\n\
         cmp r1, #0x0\n\
         ldrneh r0, [r1, #0x64]\n\
         cmpne r0, #0x1\n\
         ldrne r1, [r1, #0x68]\n\
         bne _01FF80D0\n\
         cmp r1, #0x0\n\
         bne _01FF80F8\n\
    _01FF80EC:\n\
         mov r3, #0x92\n\
         msr CPSR_c, r3\n\
         ldr pc, [sp], #0x4\n\
    _01FF80F8:\n\
         ldr r0, [ip, #0x4]\n\
         cmp r1, r0\n\
         beq _01FF80EC\n\
         ldr r3, [ip, #0xC]\n\
         cmp r3, #0x0\n\
         beq _01FF8120\n\
         stmdb sp!, {r0, r1, ip}\n\
         mov lr, pc\n\
         bx r3\n\
         ldmia sp!, {r0, r1, ip}\n\
    _01FF8120:\n\
         str r1, [ip, #0x4]\n\
         mrs r2, SPSR\n\
         str r2, [r0, #0x0]!\n\
         stmdb sp!, {r0, r1}\n\
         add r0, r0, #0x0\n\
         add r0, r0, #0x48\n\
         ldr r1, =CP_SaveContext\n\
         blx r1\n\
         ldmia sp!, {r0, r1}\n\
         ldmib sp!, {r2, r3}\n\
         stmib r0!, {r2, r3}\n\
         ldmib sp!, {r2, r3, ip, lr}\n\
         stmib r0!, {r2-r14}^\n\
         stmib r0!, {lr}\n\
         mov r3, #0xd3\n\
         msr CPSR_c, r3\n\
         stmib r0!, {sp}\n\
         stmfd sp!, {r1}\n\
         add r0, r1, #0x0\n\
         add r0, r0, #0x48\n\
         ldr r1, =CPi_RestoreContext\n\
         blx r1\n\
         ldmfd sp!, {r1}\n\
         ldr sp, [r1, #0x44]\n\
         mov r3, #0xd2\n\
         msr CPSR_c, r3\n\
         ldr r2, [r1, #0x0]!\n\
         msr SPSR_fc, r2\n\
         ldr lr, [r1, #0x40]\n\
         ldmib r1!, {r0-r14}^\n\
         nop\n\
         stmda sp!, {r0, r1, r2, r3, ip, lr}\n\
         ldmfd sp!, {pc}\n\
         .pool");
}
#else
ARM_FUNC asm void OS_IrqHandler_ThreadSwitch(void)
{
    ldr ip, =OSi_IrqThreadQueue
    mov r3, #0x0
    ldr ip, [ip]
    mov r2, #0x1
    cmp ip, #0x0
    beq _01FF80A8
_01FF8070:
    str r2, [ip, #0x64]
    str r3, [ip, #0x78]
    str r3, [ip, #0x7c]
    ldr r0, [ip, #0x80]
    str r3, [ip, #0x80]
    mov ip, r0
    cmp ip, #0x0
    bne _01FF8070
    ldr ip, =OSi_IrqThreadQueue
    str r3, [ip]
    str r3, [ip, #0x4]
    ldr ip, =OSi_ThreadInfo
    mov r1, #0x1
    strh r1, [ip]
_01FF80A8:
    ldr ip, =OSi_ThreadInfo
    ldrh r1, [ip]
    cmp r1, #0x0
    ldreq pc, [sp], #0x4
    mov r1, #0x0
    strh r1, [ip]
    mov r3, #0xd2
    msr CPSR_c, r3
    add r2, ip, #0x8
    ldr r1, [r2]
_01FF80D0:
    cmp r1, #0x0
    ldrneh r0, [r1, #0x64]
    cmpne r0, #0x1
    ldrne r1, [r1, #0x68]
    bne _01FF80D0
    cmp r1, #0x0
    bne _01FF80F8
_01FF80EC:
    mov r3, #0x92
    msr CPSR_c, r3
    ldr pc, [sp], #0x4
_01FF80F8:
    ldr r0, [ip, #0x4]
    cmp r1, r0
    beq _01FF80EC
    ldr r3, [ip, #0xC]
    cmp r3, #0x0
    beq _01FF8120
    stmdb sp!, {r0, r1, ip}
    mov lr, pc
    bx r3
    ldmia sp!, {r0, r1, ip}
_01FF8120:
    str r1, [ip, #0x4]
    mrs r2, SPSR
    str r2, [r0, #0x0]!
    stmdb sp!, {r0, r1}
    add r0, r0, #0x0
    add r0, r0, #0x48
    ldr r1, =CP_SaveContext
    blx r1
    ldmia sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, ip, lr}
    stmib r0!, {r2-r14}^
    stmib r0!, {lr}
    mov r3, #0xd3
    msr CPSR_c, r3
    stmib r0!, {sp}
    stmfd sp!, {r1}
    add r0, r1, #0x0
    add r0, r0, #0x48
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmfd sp!, {r1}
    ldr sp, [r1, #0x44]
    mov r3, #0xd2
    msr CPSR_c, r3
    ldr r2, [r1, #0x0]!
    msr SPSR_fc, r2
    ldr lr, [r1, #0x40]
    ldmib r1!, {r0-r14}^
    nop
    stmda sp!, {r0, r1, r2, r3, ip, lr}
    ldmfd sp!, {pc}
}
#endif
#ifndef __GNUC__
#pragma section ITCM end
#endif

ARM_FUNC void OS_WaitIrq(BOOL clear, OSIrqMask irqFlags)
{
    OSIntrMode lastIntrMode = OS_DisableInterrupts();
    if (clear)
    {
        OS_ClearIrqCheckFlag(irqFlags);
    }
    (void)OS_RestoreInterrupts(lastIntrMode);

    while (!(OS_GetIrqCheckFlag() & irqFlags))
    {
        OS_SleepThread(&OSi_IrqThreadQueue);
    }
}
