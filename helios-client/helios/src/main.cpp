#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>

#include "paths.h"
#include "qtmodels.h"

int main(int argc, char* argv[])
{
    QtModels::registerQmlTypes("helios");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath())
                    .filePath(QString::fromStdString(Paths::kQmlPath + Paths::kSeparator + Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
