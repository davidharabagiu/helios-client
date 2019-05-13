#include <QMetaType>
#include <QtQml>

#include "qtmodels.h"
#include "qheliosfile.h"
#include "qfiletransfer.h"
#include "qfilelisting.h"
#include "qtransferslist.h"

void QtModels::registerQmlTypes(const char* uri)
{
    qRegisterMetaType<QHeliosFile>();
    qRegisterMetaType<QFileTransfer>();
    qmlRegisterType<QFileListing>(uri, 1, 0, "FileListing");
    qmlRegisterType<QTransfersList>(uri, 1, 0, "TransfersList");
}
