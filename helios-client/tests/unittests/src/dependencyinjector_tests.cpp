#include <gtest/gtest.h>
#include <memory>

#include "dependencyinjector.h"

class DependencyInjectorTests : public testing::Test
{
};

class MyTestClass
{
public:
    MyTestClass(int a)
        : m_a(a)
    {
    }

    int m_a;
};

TEST_F(DependencyInjectorTests, GetInstanceAfterRegister)
{
    DependencyInjector depInjector;

    auto test = std::make_shared<MyTestClass>(666);
    EXPECT_TRUE(depInjector.registerInstance<MyTestClass>(test));

    auto test2 = depInjector.getInstance<MyTestClass>();
    EXPECT_NE(test2.get(), nullptr);
    EXPECT_EQ(test2->m_a, test->m_a);
    EXPECT_EQ(test.get(), test2.get());
}

TEST_F(DependencyInjectorTests, GetInstanceNoRegister)
{
    DependencyInjector depInjector;

    auto test = depInjector.getInstance<MyTestClass>();
    EXPECT_EQ(test.get(), nullptr);
}

TEST_F(DependencyInjectorTests, GetInstanceAfterRegisterAndUnregisterAll)
{
    DependencyInjector depInjector;

    auto test = std::make_shared<MyTestClass>(666);
    EXPECT_TRUE(depInjector.registerInstance<MyTestClass>(test));

    depInjector.unregisterAllInstances();
    auto test2 = depInjector.getInstance<MyTestClass>();
    EXPECT_EQ(test2.get(), nullptr);
}

TEST_F(DependencyInjectorTests, RegisteringSecondTimeFails)
{
    DependencyInjector depInjector;

    auto test = std::make_shared<MyTestClass>(666);
    EXPECT_TRUE(depInjector.registerInstance<MyTestClass>(test));

    auto test2 = std::make_shared<MyTestClass>(667);
    EXPECT_FALSE(depInjector.registerInstance<MyTestClass>(test2));

    auto test3 = depInjector.getInstance<MyTestClass>();
    EXPECT_NE(test3.get(), nullptr);
    EXPECT_EQ(test3->m_a, test->m_a);
    EXPECT_EQ(test.get(), test3.get());
}

TEST_F(DependencyInjectorTests, GetInstanceAfterReplace)
{
    DependencyInjector depInjector;

    auto test = std::make_shared<MyTestClass>(666);
    EXPECT_TRUE(depInjector.registerInstance<MyTestClass>(test));

    auto test2 = std::make_shared<MyTestClass>(667);
    EXPECT_TRUE(depInjector.replaceInstance<MyTestClass>(test2));

    auto test3 = depInjector.getInstance<MyTestClass>();
    EXPECT_NE(test3.get(), nullptr);
    EXPECT_EQ(test3->m_a, test2->m_a);
    EXPECT_EQ(test2.get(), test3.get());
}

TEST_F(DependencyInjectorTests, DestructorUnregistersAll)
{
    DependencyInjector depInjector;

    auto test = std::make_shared<MyTestClass>(666);
    EXPECT_TRUE(depInjector.registerInstance<MyTestClass>(test));

    depInjector.~DependencyInjector();
    auto test2 = depInjector.getInstance<MyTestClass>();
    EXPECT_EQ(test2.get(), nullptr);
}
