#include "rsaimpl.h"

#include <iostream>
#include <string>
#include <vector>
#include <QByteArray>
#include <QDebug>
#include <QCryptographicHash>

int main()
{
    auto test = QByteArray::fromStdString("pula");
    auto kek  = QCryptographicHash::hash(test, QCryptographicHash::Algorithm::Sha256);

    qDebug() << kek;

    return 0;
}
