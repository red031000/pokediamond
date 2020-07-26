#include "global.h"
#include "MI_swap.h"

/*
 * Exchange 32 bits of data between register and memory.
 *
 * The SDK also defines an 8-bit version of this routine,
 * but it is not linked in pokediamond.
 */

#ifdef __GNUC__
NAKED
ARM_FUNC u32 MI_SwapWord(register u32 setData, register vu32 * destp)
{
    asm("swp r0, r0, [r1]\n\
        bx lr\n\
        .pool");
}
#else
ARM_FUNC asm u32 MI_SwapWord(register u32 setData, register vu32 * destp)
{
    swp r0, r0, [r1]
    bx lr
}
#endif
