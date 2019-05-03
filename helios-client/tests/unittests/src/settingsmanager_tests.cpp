#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QFile>

#include "settingsmanagerimpl.h"
#include "defaultsettingsprovider_mock.h"
#include "paths.h"

class SettingsManagerTests : public testing::Test
{
protected:
    void SetUp() override
    {
        m_defaultSettingsProvider = std::make_unique<testing::NiceMock<DefaultSettingsProviderMock>>();
    }

    std::unique_ptr<DefaultSettingsProviderMock> m_defaultSettingsProvider;
};

TEST_F(SettingsManagerTests, DefaultSettingsProvidedWhenNoSettingsFilePresent)
{
    QFile(Paths::kSettingsFile.c_str()).remove();
    ON_CALL(*m_defaultSettingsProvider, get("favorite_fruit"))
        .WillByDefault(testing::Return(QVariant(QString("banana"))));

    SettingsManagerImpl settingsManager(std::unique_ptr<DefaultSettingsProvider>(std::move(m_defaultSettingsProvider)));

    EXPECT_EQ(settingsManager.get("favorite_fruit"), QString("banana"));
}

TEST_F(SettingsManagerTests, InvalidKeyWillProvideAnInvalidValue)
{
    SettingsManagerImpl settingsManager(std::unique_ptr<DefaultSettingsProvider>(std::move(m_defaultSettingsProvider)));

    EXPECT_FALSE(settingsManager.get("0923rh02inc230").isValid());
}

TEST_F(SettingsManagerTests, SettingAStringValueWillProvideTheSameValue)
{
    auto settingsManager =
        new SettingsManagerImpl(std::unique_ptr<DefaultSettingsProvider>(std::move(m_defaultSettingsProvider)));

    settingsManager->set("favorite_fruit", QString("banana"));
    EXPECT_EQ(settingsManager->get("favorite_fruit").type(), QVariant::String);
    EXPECT_EQ(settingsManager->get("favorite_fruit").toString(), QString("banana"));

    delete settingsManager;
    QFile(Paths::kConfigFile.c_str()).remove();
}

TEST_F(SettingsManagerTests, SettingABooleanValueWillProvideTheSameValue)
{
    auto settingsManager =
        new SettingsManagerImpl(std::unique_ptr<DefaultSettingsProvider>(std::move(m_defaultSettingsProvider)));

    settingsManager->set("banana?", true);
    EXPECT_EQ(settingsManager->get("banana?").type(), QVariant::Bool);
    EXPECT_EQ(settingsManager->get("banana?").toBool(), true);

    delete settingsManager;
    QFile(Paths::kConfigFile.c_str()).remove();
}

TEST_F(SettingsManagerTests, SettingADoubleValueWillProvideTheSameValue)
{
    auto settingsManager =
        new SettingsManagerImpl(std::unique_ptr<DefaultSettingsProvider>(std::move(m_defaultSettingsProvider)));

    settingsManager->set("bananas_count", 9001.0);
    EXPECT_EQ(settingsManager->get("bananas_count").type(), QVariant::Double);
    EXPECT_EQ(settingsManager->get("bananas_count").toDouble(), 9001.0);

    delete settingsManager;
    QFile(Paths::kConfigFile.c_str()).remove();
}
