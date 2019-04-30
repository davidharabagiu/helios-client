#include "testapplication.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactoryimpl.h"
#include "httprequestmanagerimpl.h"

int main(int argc, char** argv)
{
    Single<DependencyInjector>::instance().registerInstance<HttpRequestFactory>(
        std::make_shared<HttpRequestFactoryImpl>());
    Single<DependencyInjector>::instance().registerInstance<HttpRequestManager>(
        std::make_shared<HttpRequestManagerImpl>());

    TestApplication app(argc, argv);
    app.postEvent(&app, new QEvent(TestApplication::RunTests));
    return app.exec();
}
