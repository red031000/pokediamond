#include "OS_exception.h"
#include "function_target.h"
#include "OS_protectionUnit.h"

static OSiExContext OSi_ExContext;

static OSExceptionHandler OSi_UserExceptionHandler;
static void *OSi_UserExceptionHandlerArg;

static void *OSi_DebuggerHandler = NULL;

#define HW_EXCEP_VECTOR_BUF_FOR_DEBUGGER 0x027ffd9c

static void OSi_GetAndDisplayContext(void);
static void OSi_SetExContext(void);
static void OSi_DisplayExContext(void);

ARM_FUNC void OS_InitException(void)
{
    if (0x2600000 <= *(u32 *)HW_EXCEP_VECTOR_BUF_FOR_DEBUGGER
        && *(u32 *)HW_EXCEP_VECTOR_BUF_FOR_DEBUGGER < 0x2800000)
    {
        OSi_DebuggerHandler = *(void **)HW_EXCEP_VECTOR_BUF_FOR_DEBUGGER;
    }
    else
    {
        OSi_DebuggerHandler = NULL;
    }

    if (!OSi_DebuggerHandler)
    {
        *(u32 *)(HW_EXCEP_VECTOR_BUF_FOR_DEBUGGER) = (u32)OSi_ExceptionHandler;

        *(u32 *)(HW_EXCP_VECTOR_BUF) = (u32)OSi_ExceptionHandler;
    }

    OSi_UserExceptionHandler = NULL;
}

#ifdef __GNUC__
NAKED
ARM_FUNC void OSi_ExceptionHandler(void)
{
    asm("ldr r12, =OSi_DebuggerHandler\n\
         ldr r12, [r12]\n\
         cmp r12, #0\n\
         movne lr, pc\n\
         bxne r12\n\
         ldr r12, =0x02000000\n\
         stmfd r12!, {r0-r3, sp, lr}\n\
         and r0, sp, #1\n\
         mov sp, r12\n\
         mrs r1, cpsr\n\
         and r1, r1, #0x1f\n\
         teq r1, #0x17\n\
         bne _020CCA38\n\
         bl OSi_GetAndDisplayContext\n\
         b _020CCA44\n\
    _020CCA38:\n\
         teq r1, #0x1b\n\
         bne _020CCA44\n\
         bl OSi_GetAndDisplayContext\n\
    _020CCA44:\n\
         ldr r12, =OSi_DebuggerHandler\n\
         ldr r12, [r12]\n\
         cmp r12, #0\n\
    _020CCA50:\n\
         beq _020CCA50\n\
    _020CCA54:\n\
         mov r0, r0\n\
         b _020CCA54\n\
         ldmfd sp!, {r0-r3, r12, lr}\n\
         mov sp, r12\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void OSi_ExceptionHandler(void)
{
    ldr r12, =OSi_DebuggerHandler
    ldr r12, [r12]
    cmp r12, #0
    movne lr, pc
    bxne r12

    ldr r12, =HW_ITCM_END
    stmfd r12!, {r0-r3, sp, lr}

    and r0, sp, #1
    mov sp, r12

    mrs r1, cpsr
    and r1, r1, #0x1f

    teq r1, #0x17
    bne _020CCA38
    bl OSi_GetAndDisplayContext
    b _020CCA44

_020CCA38:
    teq r1, #0x1b
    bne _020CCA44
    bl OSi_GetAndDisplayContext

_020CCA44:
    ldr r12, =OSi_DebuggerHandler
    ldr r12, [r12]
    cmp r12, #0

_020CCA50:
    beq _020CCA50

_020CCA54:
    mov r0, r0
    b _020CCA54

    ldmfd sp!, {r0-r3, r12, lr}
    mov sp, r12
    bx lr
}
#endif

#ifdef __GNUC__
NAKED USED
ARM_FUNC static void OSi_GetAndDisplayContext(void)
{
    asm("stmfd sp!, {r0, lr}\n\
         bl OSi_SetExContext\n\
         bl OSi_DisplayExContext\n\
         ldmfd sp!, {r0, lr}\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC static asm void OSi_GetAndDisplayContext(void)
{
    stmfd sp!, {r0, lr}

    bl OSi_SetExContext
    bl OSi_DisplayExContext

    ldmfd sp!, {r0, lr}
    bx lr
}
#endif

#ifdef __GNUC__
NAKED USED
ARM_FUNC static void OSi_SetExContext(void)
{
    asm("ldr r1, =OSi_ExContext\n\
         mrs r2, cpsr\n\
         str r2, [r1, #0x74]\n\
         str r0, [r1, #0x6c]\n\
         ldr r0, [r12, #0]\n\
         str r0, [r1, #4]\n\
         ldr r0, [r12, #4]\n\
         str r0, [r1, #8]\n\
         ldr r0, [r12, #8]\n\
         str r0, [r1, #12]\n\
         ldr r0, [r12, #12]\n\
         str r0, [r1, #16]\n\
         ldr r2, [r12, #16]\n\
         bic r2, r2, #1\n\
         add r0, r1, #20\n\
         stmia r0, {r4-r11}\n\
         str r12, [r1, #0x70]\n\
         ldr r0, [r2, #0]\n\
         str r0, [r1, #0x64]\n\
         ldr r3, [r2, #4]\n\
         str r3, [r1, #0]\n\
         ldr r0, [r2, #8]\n\
         str r0, [r1, #52]\n\
         ldr r0, [r2, #12]\n\
         str r0, [r1, #64]\n\
         mrs r0, cpsr\n\
         orr r3, r3, #0x80\n\
         bic r3, r3, #0x20\n\
         msr cpsr_cxsf, r3\n\
         str sp, [r1, #56]\n\
         str lr, [r1, #60]\n\
         mrs r2, spsr\n\
         str r2, [r1, #0x7c]\n\
         msr cpsr_cxsf, r0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC static asm void OSi_SetExContext(void)
{
    ldr r1, =OSi_ExContext;

    mrs r2, cpsr
    str r2, [r1, #OSiExContext.debug[1]]

    str r0, [r1, #OSiExContext.exinfo]

    ldr r0, [r12, #0]
    str r0, [r1, #4]
    ldr r0, [r12, #4]
    str r0, [r1, #8]
    ldr r0, [r12, #8]
    str r0, [r1, #12]
    ldr r0, [r12, #12]
    str r0, [r1, #16]
    ldr r2, [r12, #16]
    bic r2, r2, #1

    add r0, r1, #20
    stmia r0, {r4-r11}

    str r12, [r1, #OSiExContext.debug[0]]

    ldr r0, [r2, #0]
    str r0, [r1, #OSiExContext.cp15]
    ldr r3, [r2, #4]
    str r3, [r1, #0]
    ldr r0, [r2, #8]
    str r0, [r1, #52]
    ldr r0, [r2, #12]
    str r0, [r1, #64]

    mrs r0, cpsr
    orr r3, r3, #0x80
    bic r3, r3, #0x20
    msr cpsr_cxsf, r3

    str sp, [r1, #56]
    str lr, [r1, #60]
    mrs r2, spsr

    str r2, [r1, #OSiExContext.debug[3]]

    msr cpsr_cxsf, r0
    bx lr
}
#endif

USED
ARM_FUNC static void OSi_DisplayExContext(void)
{
    if (OSi_UserExceptionHandler)
    {
#ifdef __GNUC__
        asm("mov r0, sp\n\
             ldr r1, =0x9f\n\
             msr cpsr_cxsf, r1\n\
             mov sp, r0");
#else
        asm
        {
            mov r0, sp
            ldr r1, =0x9f
            msr CPSR_cxsf, r1
            mov sp, r0
        }
#endif

        OS_EnableProtectionUnit();

        ((void (*)(u32, void *))OSi_UserExceptionHandler)((u32)&OSi_ExContext, OSi_UserExceptionHandlerArg);

        OS_DisableProtectionUnit();
    }
}
