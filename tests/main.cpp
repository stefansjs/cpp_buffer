#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cpp_buffer/buffer.h>

using namespace CPPBuffer;

TEST_GROUP(BufferConstructors) {};

// TEST(BufferConstructors, nullptr)
// {
//     Buffer<float> buffer;
//     CHECK_TRUE(buffer == nullptr);

//     Buffer<float> buffer2;
//     CHECK_TRUE(buffer2 == buffer1);
// }

TEST(BufferConstructors, copies)
{
    Buffer<int> buffer(10);
    CHECK_TRUE(buffer.size() == 10);
    CHECK_TRUE(size_of(buffer) == sizeof(int[10]));
}

int main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
