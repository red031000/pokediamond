#include "global.h"
#include "main.h"
#include "gx.h"

//looks like asm and says asm on the tin...
#ifdef __GNUC__
NAKED
ARM_FUNC void GX_SendFifo48B(void *src, void *dst){
    asm("ldmia r0!, {r2-r3,r12}\n\
         stmia r1, {r2-r3,r12}\n\
         ldmia r0!, {r2-r3,r12}\n\
         stmia r1, {r2-r3,r12}\n\
         ldmia r0!, {r2-r3,r12}\n\
         stmia r1, {r2-r3,r12}\n\
         ldmia r0!, {r2-r3,r12}\n\
         stmia r1, {r2-r3,r12}\n\
         bx lr\n\
         .pool");
}
#else
ARM_FUNC asm void GX_SendFifo48B(void *src, void *dst){
    ldmia r0!, {r2-r3,r12}
    stmia r1, {r2-r3,r12}
    ldmia r0!, {r2-r3,r12}
    stmia r1, {r2-r3,r12}
    ldmia r0!, {r2-r3,r12}
    stmia r1, {r2-r3,r12}
    ldmia r0!, {r2-r3,r12}
    stmia r1, {r2-r3,r12}
    bx lr
}
#endif
