#include "utils.h"
#include "common.h"
#include <math.h>

f32 rand_between(f32 min, f32 max) {
    f32 r = (f32)rand() / (f32)RAND_MAX;
    f32 range = max - min;
    return r * range + min;
}
