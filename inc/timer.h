#ifndef TIMER_H
#define TIMER_H

#include <time.h>

#define BENCHMARK_FUNC(tag, function) do { \
    clock_t begin = clock(); \
    function; \
    clock_t end = clock(); \
    printf("%s - %.9f sec.\n", tag, (double)(end - begin) / CLOCKS_PER_SEC); \
} while(0);

#define HANDLE_TIMER(tag, code) {\
    if (benchmark) { BENCHMARK_FUNC(tag, code); }\
    else { code; }\
}

#endif
