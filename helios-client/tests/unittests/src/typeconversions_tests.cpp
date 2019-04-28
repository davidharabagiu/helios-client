#include <gtest/gtest.h>
#include <memory>

#include "typeconversions.h"

class TypeConversionsTests : public testing::Test
{
};

class MyTestClassA
{
public:
    virtual ~MyTestClassA() = default;
};

class MyTestClassB : public MyTestClassA
{
};

TEST_F(TypeConversionsTests, NarrowingSignedInteger)
{
    int   x = -1000;
    short y = safe_integral_cast<short>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, WideningSignedInteger)
{
    int       x = -1000;
    long long y = safe_integral_cast<long long>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, NarrowingUnsignedInteger)
{
    unsigned int   x = 1000;
    unsigned short y = safe_integral_cast<unsigned short>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, WideningUnsignedInteger)
{
    unsigned int       x = 1000;
    unsigned long long y = safe_integral_cast<unsigned long long>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, ConvertSignedToUnsigned)
{
    int          x = 1000;
    unsigned int y = safe_integral_cast<unsigned int>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, ConvertUnsignedToSigned)
{
    unsigned int x = 1000;
    int          y = safe_integral_cast<int>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, NarrowAndConvertSignedToUnsigned)
{
    int            x = 50000;
    unsigned short y = safe_integral_cast<unsigned short>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, NarrowAndConvertUnsignedToSigned)
{
    unsigned int x = 1000;
    short        y = safe_integral_cast<short>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, WidenAndConvertSignedToUnsigned)
{
    int                x = 1000;
    unsigned long long y = safe_integral_cast<unsigned long long>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, WidenAndConvertUnsignedToSigned)
{
    unsigned short x = 50000;
    int            y = safe_integral_cast<int>(x);
    EXPECT_EQ(x, y);
}

TEST_F(TypeConversionsTests, FailedNarrowingSignedInteger)
{
    int x = -50000;
    EXPECT_DEBUG_DEATH(safe_integral_cast<short>(x), "");
}

TEST_F(TypeConversionsTests, FailedNarrowingUnsignedInteger)
{
    unsigned int x = 100000;
    EXPECT_DEBUG_DEATH(safe_integral_cast<unsigned short>(x), "");
}

TEST_F(TypeConversionsTests, FailedConvertSignedToUnsigned)
{
    int x = -1;
    EXPECT_DEBUG_DEATH(safe_integral_cast<unsigned int>(x), "");
}

TEST_F(TypeConversionsTests, FailedConvertUnsignedToSigned)
{
    unsigned short x = 50000;
    EXPECT_DEBUG_DEATH(safe_integral_cast<short>(x), "");
}

TEST_F(TypeConversionsTests, FailedNarrowAndConvertSignedToUnsigned)
{
    int x = 100000;
    EXPECT_DEBUG_DEATH(safe_integral_cast<unsigned short>(x), "");
    x = -1;
    EXPECT_DEBUG_DEATH(safe_integral_cast<unsigned short>(x), "");
}

TEST_F(TypeConversionsTests, FailedNarrowAndConvertUnsignedToSigned)
{
    unsigned int x = 50000;
    EXPECT_DEBUG_DEATH(safe_integral_cast<short>(x), "");
}

TEST_F(TypeConversionsTests, FailedWidenAndConvertSignedToUnsigned)
{
    int x = -1;
    EXPECT_DEBUG_DEATH(safe_integral_cast<unsigned long long>(x), "");
}

TEST_F(TypeConversionsTests, StaticUniqueCastMovesSourceToDestination)
{
    std::unique_ptr<MyTestClassA> ptr(new MyTestClassB());
    auto                          rawPtr = ptr.get();
    auto                          ptr2   = static_unique_cast<MyTestClassB>(std::move(ptr));
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr2.get(), rawPtr);
}

TEST_F(TypeConversionsTests, DynamicUniqueCastMovesSourceToDestination)
{
    std::unique_ptr<MyTestClassA> ptr(new MyTestClassB());
    auto                          rawPtr = ptr.get();
    auto                          ptr2   = dynamic_unique_cast<MyTestClassB>(std::move(ptr));
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr2.get(), rawPtr);
}

TEST_F(TypeConversionsTests, DynamicUniqueCastSourceAndDestinationNotCompatible)
{
    std::unique_ptr<MyTestClassA> ptr(new MyTestClassA());
    auto                          rawPtr = ptr.get();
    auto                          ptr2   = dynamic_unique_cast<MyTestClassB>(std::move(ptr));
    EXPECT_EQ(ptr.get(), rawPtr);
    EXPECT_EQ(ptr2.get(), nullptr);
}
