#include <gtest/gtest.h>
#include <memory>

#include "defaultsettingsproviderimpl.h"
#include "defaultsettingskeys.h"

class DefaultSettingsTests : public testing::Test
{
};

TEST_F(DefaultSettingsTests, SettingsLoadedSuccessfully)
{
    std::unique_ptr<DefaultSettingsProviderImpl> loader;

    EXPECT_NO_FATAL_FAILURE(loader = std::make_unique<DefaultSettingsProviderImpl>());
    EXPECT_EQ(loader->get(SettingsKeys::kDarkMode).type(), QVariant::Bool);
    EXPECT_EQ(loader->get(SettingsKeys::kFullScreen).type(), QVariant::Bool);
    EXPECT_EQ(loader->get(SettingsKeys::kWindowWidth).type(), QVariant::Double);
    EXPECT_EQ(loader->get(SettingsKeys::kWindowHeight).type(), QVariant::Double);
}

TEST_F(DefaultSettingsTests, InvalidSettingKeyReturnsInvalidValue)
{
    std::unique_ptr<DefaultSettingsProviderImpl> loader;

    EXPECT_NO_FATAL_FAILURE(loader = std::make_unique<DefaultSettingsProviderImpl>());
    EXPECT_FALSE(loader->get("askndo0noinf09293").isValid());
}
