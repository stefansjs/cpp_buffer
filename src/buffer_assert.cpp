#include "cpp_buffer/buffer_assert.h"



using namespace CPPBuffer;

static const char *OUT_OF_RANGE_STRING = "Buffer access out of bounds";

inline void assert_hook(const char *file, int line, const int condition, const char *conditionString)
{
    try {
        static const char *formatString = "%s:%d buffer assert condition failed. Got %d from \"%s\"";
        char buffer[1024];
        std::snprintf(buffer, sizeof(buffer), formatString, file, line, condition, conditionString);
        throw OutOfRangeError(buffer);
    } catch(...) {
        // If we fail to format a string for some reason, pass a non-formatted statically initialized string instead
        throw OutOfRangeError(OUT_OF_RANGE_STRING);
    } 
}