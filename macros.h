#ifndef HEADER_TED_MACROS
#define HEADER_TED_MACROS

#define internal_func static
#define global_var static

// Return the number of elements in a static array
#define countArray(arr) (sizeof(arr) / sizeof((arr)[0]))

#if LOCAL_BUILD
// NOLINTBEGIN
#define assert(expression) \
    if (!(expression)) { \
        __pragma(warning(push)) \
        __pragma(warning(disable: 6011)) \
        int *address = 0x0; \
        *address = 0; \
        __pragma(warning(pop)) \
    }
// NOLINTEND
#else
#define assert(expression)
#endif

#define SCREEN_SIZE_X 1400
#define SCREEN_SIZE_Y 800

#endif