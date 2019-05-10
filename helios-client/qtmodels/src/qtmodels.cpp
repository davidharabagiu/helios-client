#include <QMetaType>

#include "qtmodels.h"
#include "qheliosfile.h"
#include "qfiletransfer.h"

void QtModels::registerQmlTypes(const char* /*uri*/)
{
    qRegisterMetaType<QHeliosFile>();
    qRegisterMetaType<QFileTransfer>();
}
