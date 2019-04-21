#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QDir(QCoreApplication::applicationDirPath()).filePath("qml/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
