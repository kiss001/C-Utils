#include "time_utils.h"

#include "types.h"
#include <sys/time.h>

long TimeUtils::current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}