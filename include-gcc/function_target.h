#ifndef POKEDIAMOND_FUNCTION_TARGET_H
#define POKEDIAMOND_FUNCTION_TARGET_H

#define ARM_FUNC __attribute__((target("arm")))
#define THUMB_FUNC __attribute__((target("thumb")))

//todo find alternatives
#define ENUMS_ALWAYS_INT_ON
#define ENUMS_ALWAYS_INT_OFF
#define ENUMS_ALWAYS_INT_RESET

#define NAKED __attribute__((naked))

#define UNUSED __attribute__((unused))
#define USED __attribute__((used))

#endif //POKEDIAMOND_FUNCTION_TARGET_H
