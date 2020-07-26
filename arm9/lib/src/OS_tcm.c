#include "OS_tcm.h"
#include "function_target.h"

#ifdef __GNUC__
NAKED
ARM_FUNC u32 OS_GetDTCMAddress(void)
{
    asm("mrc p15, 0x0, r0, c9, c1, 0x0\n\
         ldr r1, =0xfffff000\n\
         and r0, r0, r1\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm u32 OS_GetDTCMAddress(void)
{
    mrc p15, 0x0, r0, c9, c1, 0x0 //Data TCM Base
    ldr r1, =OSi_TCM_REGION_BASE_MASK
    and r0, r0, r1
    bx lr
}
#endif
