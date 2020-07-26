#ifndef GUARD_SINIT_H
#define GUARD_SINIT_H

static void NitroStaticInit(void);
#ifdef __GNUC__
#define SECTION_SINIT __attribute__((section("SINIT")))
#else
#define SECTION_SINIT
#pragma define_section SINIT ".sinit" abs32 RWX
#pragma section SINIT begin
#endif
SDK_FORCE_EXPORT SECTION_SINIT static void (*NitroStaticInit_[])(void) USED = { NitroStaticInit };
#ifndef __GNUC__
#pragma section SINIT end
#endif

#endif //GUARD_SINIT_H
