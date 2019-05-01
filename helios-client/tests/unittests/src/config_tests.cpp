#include <gtest/gtest.h>
#include <memory>

#include "configimpl.h"
#include "configkeys.h"

class ConfigTests : public testing::Test
{
};

TEST_F(ConfigTests, ConfigurationsLoadedSuccessfully)
{
    std::unique_ptr<ConfigImpl> config;

    EXPECT_NO_FATAL_FAILURE(config = std::make_unique<ConfigImpl>());
    EXPECT_TRUE(config->get(ConfigKeys::kServerUrlKey).type() == QVariant::String);
}

TEST_F(ConfigTests, UsingInvalidKeyReturnsInvalidValue)
{
    std::unique_ptr<ConfigImpl> config;

    EXPECT_NO_FATAL_FAILURE(config = std::make_unique<ConfigImpl>());
    EXPECT_FALSE(config->get("0192jeoq239uen03p").isValid());
}
