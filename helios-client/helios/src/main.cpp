#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>

#include "paths.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QString::fromStdString(Paths::kQmlPath)).filePath(QString::fromStdString(Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
