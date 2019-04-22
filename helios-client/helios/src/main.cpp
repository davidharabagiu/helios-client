#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QDebug>

#include "paths.h"
#include "dependencyinjector.h"

int main(int argc, char* argv[])
{
    DependencyInjector depInjector;
    depInjector.registerInstance<int>(std::make_shared<int>(5));

    auto test = depInjector.getInstance<int>();

    qDebug() << *test;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QString::fromStdString(Paths::kQmlPath)).filePath(QString::fromStdString(Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
