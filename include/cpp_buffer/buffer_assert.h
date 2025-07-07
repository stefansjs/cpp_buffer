#pragma once
#include "buffer_definitions.h"


#if CPPBUFFER_USES_ASSERT && CPPBUFFER_ASSERT_USES_CASSERT
  #include <cassert>
  #define cpp_buffer_assert(cond_) cassert(cond_)
#elif CPPBUFFER_USES_ASSERT
  #define cpp_buffer_assert(cond_) do { if(!(cond_)) ::CPPBuffer::assert_hook(__FILE__, __LINE__, cond_, #cond_); } while(0)
#else
  #define cpp_buffer_assert(cond_) (void(0))
#endif


#if CPPBUFFER_ASSERT_USES_EXCEPTIONS
#include <stdexcept>
#endif


namespace CPPBuffer
{

#if CPPBUFFER_ASSERT_USES_EXCEPTIONS
typedef std::out_of_range OutOfRangeError;
#endif

inline void assert_hook(const char *file, int line, const int condition, const char *conditionString);

}
