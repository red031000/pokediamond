#include "OS_cache.h"
#include "nitro/types.h"
#include "function_target.h"

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_InvalidateAll(void)
{
    asm("mov r0, #0\n\
         mcr p15, 0, r0, c7, c6, 0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_InvalidateAll(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c6, 0 //Invalidate Entire Data Cache
    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_StoreAll(void)
{
    asm("mov r1, #0\n\
    _020CC0C8:\n\
         mov r0, #0\n\
    _020CC0CC:\n\
         orr r2, r1, r0\n\
         mcr p15, 0, r2, c7, c10, 2\n\
         add r0, r0, #32\n\
         cmp r0, #0x400\n\
         blt _020CC0CC\n\
         add r1, r1, #0x40000000\n\
         cmp r1, #0\n\
         bne _020CC0C8\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_StoreAll(void)
{
    mov r1, #0

_020CC0C8:
    mov r0, #0

_020CC0CC:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c10, 2 //Clean Data Cache Line Set/Index
    add r0, r0, #32
    cmp r0, #0x400
    blt _020CC0CC

    add r1, r1, #0x40000000
    cmp r1, #0
    bne _020CC0C8

    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_FlushAll(void)
{
    asm("mov r12, #0\n\
         mov r1, #0\n\
    _020CC0F8:\n\
         mov r0, #0\n\
    _020CC0FC:\n\
         orr r2, r1, r0\n\
         mcr p15, 0, r12, c7, c10, 4\n\
         mcr p15, 0, r2, c7, c14, 2\n\
         add r0, r0, #32\n\
         cmp r0, #0x400\n\
         blt _020CC0FC\n\
         add r1, r1, #0x40000000\n\
         cmp r1, #0\n\
         bne _020CC0F8\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_FlushAll(void)
{
    mov r12, #0
    mov r1, #0

_020CC0F8:
    mov r0, #0

_020CC0FC:
    orr r2, r1, r0
    mcr p15, 0, r12, c7, c10, 4 //Drain Write Buffer
    mcr p15, 0, r2, c7, c14, 2 //Clean and Invalidate Data Cache Line Set/Index
    add r0, r0, #32
    cmp r0, #0x400
    blt _020CC0FC

    add r1, r1, #0x40000000
    cmp r1, #0
    bne _020CC0F8

    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_InvalidateRange(register void *startAddr, register u32 nBytes)
{
    asm("add r1, r1, r0\n\
         bic r0, r0, #31\n\
    _020CC12C:\n\
         mcr p15, 0, r0, c7, c6, 1\n\
         add r0, r0, #32\n\
         cmp r0, r1\n\
         blt _020CC12C\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_InvalidateRange(register void *startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #31

_020CC12C:
    mcr p15, 0, r0, c7, c6, 1 //Invalidated Data Cache Line Virtual Address
    add r0, r0, #32
    cmp r0, r1
    blt _020CC12C

    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_StoreRange(register void *startAddr, register u32 nBytes)
{
    asm("add r1, r1, r0\n\
         bic r0, r0, #31\n\
    _020CC148:\n\
         mcr p15, 0, r0, c7, c10, 1\n\
         add r0, r0, #32\n\
         cmp r0, r1\n\
         blt _020CC148\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_StoreRange(register void *startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #31

_020CC148:
    mcr p15, 0, r0, c7, c10, 1 //Clean Data Cache Line Virtual Address
    add r0, r0, #32
    cmp r0, r1
    blt _020CC148

    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_FlushRange(register const void *startAddr, register u32 nBytes)
{
    asm("mov r12, #0\n\
         add r1, r1, r0\n\
         bic r0, r0, #31\n\
    _020CC168:\n\
         mcr p15, 0, r12, c7, c10, 4\n\
         mcr p15, 0, r0, c7, c14, 1\n\
         add r0, r0, #32\n\
         cmp r0, r1\n\
         blt _020CC168\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_FlushRange(register const void *startAddr, register u32 nBytes)
{
    mov r12, #0
    add r1, r1, r0
    bic r0, r0, #31

_020CC168:
    mcr p15, 0, r12, c7, c10, 4 //Drain Write Buffer
    mcr p15, 0, r0, c7, c14, 1 //Clean and Invalidate Data Cache Line Virtual Address
    add r0, r0, #32
    cmp r0, r1
    blt _020CC168

    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void DC_WaitWriteBufferEmpty(void)
{
    asm("mov r0, #0\n\
         mcr p15, 0, r0, c7, c10, 4\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void DC_WaitWriteBufferEmpty(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c10, 4 //Drain Write Buffer
    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void IC_InvalidateAll(void)
{
    asm("mov r0, #0\n\
         mcr p15, 0, r0, c7, c5, 0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void IC_InvalidateAll(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0 //Invalidate Entire Instruction Cache
    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void IC_InvalidateRange(register void *startAddr, register u32 nBytes)
{
    asm("add r1, r1, r0\n\
         bic r0, r0, #31\n\
    _020CC1A0:\n\
         mcr p15, 0, r0, c7, c5, 1\n\
         add r0, r0, #32\n\
         cmp r0, r1\n\
         blt _020CC1A0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void IC_InvalidateRange(register void *startAddr, register u32 nBytes)
{
    add r1, r1, r0
    bic r0, r0, #31

_020CC1A0:
    mcr p15, 0, r0, c7, c5, 1 //Invalidate Instruction Cache Line Virtual Address
    add r0, r0, #32
    cmp r0, r1
    blt _020CC1A0

    bx lr
}
#endif
