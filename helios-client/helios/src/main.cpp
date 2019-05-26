#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <memory>

#include "paths.h"
#include "qtmodels.h"
#include "controllers.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userserviceimpl.h"
#include "fileserviceimpl.h"
#include "userapicallerimpl.h"
#include "fileapicallerimpl.h"
#include "settingsmanagerimpl.h"
#include "defaultsettingsproviderimpl.h"
#include "httprequestmanagerimpl.h"
#include "configimpl.h"
#include "httprequestfactoryimpl.h"
#include "cipherfactoryimpl.h"
#include "keymanagerimpl.h"
#include "configkeys.h"
#include "rsaimpl.h"
#include "randomfactoryimpl.h"
#include "notificationserviceimpl.h"
#include "notificationsapicallerimpl.h"

void registerInstances()
{
    std::shared_ptr<SettingsManager> settingsManager(
        new SettingsManagerImpl(std::make_unique<DefaultSettingsProviderImpl>()));
    Single<DependencyInjector>::instance().registerInstance<SettingsManager>(settingsManager);

    std::shared_ptr<Config> config(new ConfigImpl());
    Single<DependencyInjector>::instance().registerInstance<Config>(config);

    std::shared_ptr<HttpRequestFactory> httpRequestFactory(new HttpRequestFactoryImpl());
    Single<DependencyInjector>::instance().registerInstance<HttpRequestFactory>(httpRequestFactory);

    std::shared_ptr<HttpRequestManager> httpRequestManager(new HttpRequestManagerImpl());
    httpRequestManager->setIgnoreSslErrors(config->get(ConfigKeys::kIgnoreSslErrors).toBool());
    Single<DependencyInjector>::instance().registerInstance<HttpRequestManager>(httpRequestManager);

    std::shared_ptr<UserService> userService(new UserServiceImpl(
        std::make_unique<UserApiCallerImpl>(httpRequestManager), settingsManager, std::make_unique<RsaImpl>()));
    Single<DependencyInjector>::instance().registerInstance<UserService>(userService);

    std::shared_ptr<KeyManager> keyManager(new KeyManagerImpl(std::make_unique<RandomFactoryImpl>()));
    Single<DependencyInjector>::instance().registerInstance<KeyManager>(keyManager);

    std::shared_ptr<FileService> fileService(
        new FileServiceImpl(config, std::make_unique<FileApiCallerImpl>(httpRequestManager),
                            std::make_unique<CipherFactoryImpl>(), keyManager));
    Single<DependencyInjector>::instance().registerInstance<FileService>(fileService);

    std::shared_ptr<NotificationService> notificationService(
        new NotificationServiceImpl(std::make_unique<NotificationsApiCallerImpl>(httpRequestManager), config));
    Single<DependencyInjector>::instance().registerInstance<NotificationService>(notificationService);
}

int main(int argc, char* argv[])
{
    QtModels::registerQmlTypes("helios");
    Controllers::registerQmlTypes("helios");
    registerInstances();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath())
                    .filePath(QString::fromStdString(Paths::kQmlPath + Paths::kSeparator + Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
