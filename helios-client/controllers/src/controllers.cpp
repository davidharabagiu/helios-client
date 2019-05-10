#include <QtQml>

#include "controllers.h"
#include "qauthenticationcontroller.h"
#include "qsettingscontroller.h"
#include "qremotefilesystemcontroller.h"

void Controllers::registerQmlTypes(const char* uri)
{
    qmlRegisterType<QAuthenticationController>(uri, 1, 0, "AuthenticationController");
    qmlRegisterType<QSettingsController>(uri, 1, 0, "SettingsController");
    qmlRegisterType<QRemoteFileSystemController>(uri, 1, 0, "RemoteFileSystemController");
}
