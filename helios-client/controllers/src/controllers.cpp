#include <QtQml>

#include "controllers.h"
#include "qauthenticationcontroller.h"
#include "qsettingscontroller.h"
#include "qremotefilesystemcontroller.h"
#include "qkeystoragecontroller.h"

void Controllers::registerQmlTypes(const char* uri)
{
    qmlRegisterType<QAuthenticationController>(uri, 1, 0, "AuthenticationController");
    qmlRegisterType<QSettingsController>(uri, 1, 0, "SettingsController");
    qmlRegisterType<QRemoteFileSystemController>(uri, 1, 0, "RemoteFileSystemController");
    qmlRegisterType<QKeyStorageController>(uri, 1, 0, "KeyStorageController");
}
