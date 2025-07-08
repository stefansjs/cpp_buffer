#pragma once

#include<cassert>
#include<memory>


namespace CPPBuffer
{

/***
 *  The buffer class holds statically-sized contiguous memory of any data type. 
 *  Unlike a std::vector it cannot be resized, and does not carry around an allocator. This makes it safer to use in some cases.
 *  Unlike a std::array it is run-time sized rather than compile-time sized. This makes it a _lot_ more usable in most cases.
 * 
 *  The class can be "sliced" (using the python terminology) into views into the memory that allow the caller to view
 *  a subset of the underlying data, with a non-unitary stride. Strides are compile-time constants that indicate how to
 *  iterate over the memory. For example a Buffer<char,2u> would iterate over every other element in the underlying
 *  memory. A Buffer<> could be "sliced" to view only a few elements thusly:
 *      Buffer<char> buffer(10);
 *      auto slice = buffer.slice(2,4);// slices the half-open interval from the 2nd to 4th element
 *      auto slice2 = buffer.slice<2>();// slices the full range, but iterates over every other element.
 *      auto slice3 = buffer.slice<3>(2, 10); // the two strategies can be combined.
 * 
 *  Buffer is a good candidate to use with standard library math functions. For example you could implement a mean thusly:
 *      float mean(Buffer<float> buffer) {
 *          return std::accumulate(buffer.begin(), buffer.end(), 0.0f) / buffer.size();
 *      }
 * 
 *  The ptr_t allows you to provide a custom pointer class, but the Buffer class does not do any memory bookkeeping. It
 *  very much assumes that you provide a smart pointer type.
 * 
 *  Use bare pointers at your own risk.
 */
template< typename T, const uint8_t stride=1u, typename ptr_t=std::shared_ptr<T> >
class Buffer;

// slice is a subclass of the buffer that allow for offsetting into and limiting the extent of a Buffer
// This is a separate class so that we can keep the separate bookkeeping details separate from the Buffer class, which
// allows for exactly as much memory being occupied as the underlying ptr
template< typename T, const uint8_t stride=1u, typename ptr_t=std::shared_ptr<T> >
class Slice;


// And an iterator class is needed for the non-unit-strided buffer
template< typename T, const uint8_t stride=1u >
class BufferIterator;


/**
 *  This non-strided version of the buffer class provides the cleanest looking interface to the buffer class overall.
 *  In order to understand the API, this is the class to look at.
 * 
 *  The short version is that this class is a smart pointer with explicit access operators built in that allow you to do
 *  something like this
 * 
 *      Buffer<char> buffer(new char[10], 10);
 *      std::asign(buffer.begin(), buffer.end(), 1);
 *      buffer[1] = 0;
 *      std::cout << buffer[1] << buffer[2];
 *      // this will fail under certain conditions:
 *      // buffer[10]; // calls a removable assert macro that fails (usually used for unit testing)
 */
template< typename T, typename pointer_t >
class Buffer<T, 1u, pointer_t>
{
    public:
    //typedefs
    typedef pointer_t   ptr_t;
    typedef T*          Iterator;
    typedef const T *   ConstIterator;

    // constructors and destructors are all default for the trivial case
    Buffer() = default;
    virtual ~Buffer() = default;
    Buffer(const Buffer &) = default;
    Buffer(Buffer &&) = default;
    // assignment operators are also default
    Buffer &operator=(const Buffer &) = default;
    Buffer &operator=(Buffer &&) = default;

    // The actual constructors:
    Buffer(const ptr_t &, size_t);
    Buffer(ptr_t &&, size_t);

    template< typename allocator_t >
    Buffer(size_t, allocator_t &);
    Buffer(size_t); // new-allocated

    // accessors
    T &operator[](int);
    const T &operator[](int) const;

    // iterators
    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    size_t size() const; // size is the same convention used by other stl containers

    // finally slice operations
    template< uint8_t s >
    Slice<T, s, ptr_t> slice(size_t begin, size_t end);

    private:
    ptr_t mMemory = nullptr;
    size_t mSize = 0ul;
};

/** Return the number of bytes stored in a Buffer */
template< typename T, uint8_t s, typename ptr_t >
inline size_t size_of(const Buffer<T,s,ptr_t> &buffer) {
    return buffer.size() * sizeof(T);
}




// The actual constructors:
template< typename T, typename ptr_t >
Buffer<T, 1u, ptr_t>::Buffer(const ptr_t &p, size_t s) 
    : mMemory(p)
    , mSize(s) 
{}

template< typename T, typename ptr_t >
Buffer<T, 1u, ptr_t>::Buffer(ptr_t &&other, size_t s) 
    : mMemory(std::move(other))
    , mSize(s) 
{}

template< typename T, typename ptr_t >
template< typename allocator_t >
Buffer<T, 1u, ptr_t>::Buffer(size_t n, allocator_t &a) 
    : mMemory(a.allocate(n))
    , mSize(n) 
{}

template< typename T, typename ptr_t >
Buffer<T, 1u, ptr_t>::Buffer(size_t n) 
    : mMemory(std::shared_ptr<T>(new T[n], [](T* p){ delete[] p; }))
    , mSize(n) 
{}

template< typename T, typename ptr_t >
T &Buffer<T, 1u, ptr_t>::operator[](int i) {
    assert(i >= 0 && static_cast<size_t>(i) < mSize);
    return mMemory.get()[i];
}

template< typename T, typename ptr_t >
const T &Buffer<T, 1u, ptr_t>::operator[](int i) const 
{
    assert(i >= 0 && static_cast<size_t>(i) < mSize);
    return mMemory.get()[i];
}

// iterators
template< typename T, typename ptr_t >
typename Buffer<T, 1u, ptr_t>::Iterator Buffer<T, 1u, ptr_t>::begin() { 
    return mMemory.get(); 
}

template< typename T, typename ptr_t >
typename Buffer<T, 1u, ptr_t>::Iterator Buffer<T, 1u, ptr_t>::end() {
    return mMemory.get() + mSize; 
}

template< typename T, typename ptr_t >
typename Buffer<T, 1u, ptr_t>::ConstIterator Buffer<T, 1u, ptr_t>::begin() const { 
    return mMemory.get(); 
}

template< typename T, typename ptr_t >
typename Buffer<T, 1u, ptr_t>::ConstIterator Buffer<T, 1u, ptr_t>::end() const { 
    return mMemory.get() + mSize; 
}

template< typename T, typename ptr_t >
size_t Buffer<T, 1u, ptr_t>::size() const { 
    return mSize; 
}

// finally slice operations
template< typename T, typename ptr_t >
template< uint8_t s >
Slice<T, s, ptr_t> Buffer<T, 1u, ptr_t>::slice(size_t begin, size_t end) { 
    return Slice<T, s, ptr_t>(mMemory, mSize, begin, end, s); 
}

}// namespace CPPBuffer