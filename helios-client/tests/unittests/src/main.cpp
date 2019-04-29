#include "testapplication.h"

int main(int argc, char** argv)
{
    TestApplication app(argc, argv);
    app.postEvent(&app, new QEvent(TestApplication::RunTests));
    return app.exec();
}
