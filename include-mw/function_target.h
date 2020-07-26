#ifndef GUARD_FUNCTION_TARGET_H
#define GUARD_FUNCTION_TARGET_H

#define ARM_FUNC _Pragma("thumb off")
#define THUMB_FUNC _Pragma("thumb on")

#define ENUMS_ALWAYS_INT_ON _Pragma("enumsalwaysint on")
#define ENUMS_ALWAYS_INT_OFF _Pragma("enumsalwaysint off")
#define ENUMS_ALWAYS_INT_RESET _Pragma("enumsalwaysint reset")

#define UNUSED
#define USED
//to stop it complaining
#define NAKED

#endif //GUARD_FUNCTION_TARGET_H