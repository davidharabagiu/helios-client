#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QDebug>

#include "paths.h"
#include "dependencyinjector.h"
#include "single.h"

int main(int argc, char* argv[])
{
    Single<DependencyInjector>::instance().registerInstance<int>(std::make_shared<int>(5));

    auto test = Single<DependencyInjector>::instance().getInstance<int>();

    qDebug() << *test;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath())
                    .filePath(QString::fromStdString(Paths::kQmlPath + Paths::kSeparator + Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
