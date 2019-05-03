#ifndef DEFAULTSETTINGSPROVIDER_MOCK_H
#define DEFAULTSETTINGSPROVIDER_MOCK_H

#include <gmock/gmock.h>

#include "defaultsettingsprovider.h"

class DefaultSettingsProviderMock : public DefaultSettingsProvider
{
public:
    MOCK_CONST_METHOD1(get, QVariant(const std::string&));
};

#endif  // DEFAULTSETTINGSPROVIDER_MOCK_H
