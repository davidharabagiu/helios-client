#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QDebug>

#include "paths.h"
#include "typeconversions.h"
#include <cstdint>

int main(int argc, char* argv[])
{
    unsigned long long a = 1000000000000ULL;
    auto               b = safe_integral_cast<int>(a);
    (void)b;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath())
                    .filePath(QString::fromStdString(Paths::kQmlPath + Paths::kSeparator + Paths::kMainQmlFile)));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
