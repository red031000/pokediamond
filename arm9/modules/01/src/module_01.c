#include "global.h"
#include "sinit.h"

THUMB_FUNC USED static void NitroStaticInit(void) {
    static u32 var[2];
    var[1]++;
}
