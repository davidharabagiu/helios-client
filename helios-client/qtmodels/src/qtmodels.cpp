#include <QtQml>

#include "qtmodels.h"
#include "quseraccount.h"

void QtModels::registerQmlTypes(const char* uri)
{
    qmlRegisterType<QUserAccount>(uri, 1, 0, "UserAccount");
}
