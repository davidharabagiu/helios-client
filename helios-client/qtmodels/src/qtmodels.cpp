#include <QMetaType>
#include <QtQml>

#include "qtmodels.h"
#include "qheliosfile.h"
#include "qfiletransfer.h"
#include "qfilelisting.h"
#include "qtransferslist.h"
#include "qnotification.h"
#include "qusersession.h"

void QtModels::registerQmlTypes(const char* uri)
{
    qRegisterMetaType<QHeliosFile>();
    qRegisterMetaType<QFileTransfer>();
    qRegisterMetaType<QNotification>();
    qRegisterMetaType<QUserSession>();
    qmlRegisterType<QFileListing>(uri, 1, 0, "FileListing");
    qmlRegisterType<QTransfersList>(uri, 1, 0, "TransfersList");
}
