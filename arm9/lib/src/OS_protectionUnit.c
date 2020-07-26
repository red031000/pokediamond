#include "function_target.h"
#include "OS_protectionUnit.h"

#ifdef __GNUC__
NAKED
ARM_FUNC void OS_EnableProtectionUnit(void)
{
    asm("mrc p15, 0x0, r0, c1, c0, 0x0\n\
         orr r0, r0, #0x1\n\
         mcr p15, 0x0, r0, c1, c0, 0x0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void OS_EnableProtectionUnit(void)
{
    mrc p15, 0x0, r0, c1, c0, 0x0 //Control Register
    orr r0, r0, #0x1
    mcr p15, 0x0, r0, c1, c0, 0x0 //Control Register
    bx lr
}
#endif

#ifdef __GNUC__
NAKED
ARM_FUNC void OS_DisableProtectionUnit(void)
{
    asm("mrc p15, 0x0, r0, c1, c0, 0x0\n\
         bic r0, r0, #0x1\n\
         mcr p15, 0x0, r0, c1, c0, 0x0\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void OS_DisableProtectionUnit(void)
{
    mrc p15, 0x0, r0, c1, c0, 0x0 //Control Register
    bic r0, r0, #0x1
    mcr p15, 0x0, r0, c1, c0, 0x0 //Control Register
    bx lr
}
#endif
