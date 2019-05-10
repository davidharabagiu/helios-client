#include "qtransferslist.h"

QTransfersList::QTransfersList(QObject* parent)
    : QAbstractListModel(parent)
{
}

void QTransfersList::setTransfers(const QVariantList& transfers)
{
    beginResetModel();

    m_transfers.clear();
    for (const auto& transfer : transfers)
    {
        m_transfers.push_back(qvariant_cast<QFileTransfer>(transfer));
    }

    endResetModel();
}

int QTransfersList::rowCount(const QModelIndex& /*parent*/) const
{
    return m_transfers.length();
}

QVariant QTransfersList::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    if (role == static_cast<int>(Roles::TRANSFER_DATA))
    {
        return QVariant::fromValue(m_transfers.at(index.row()));
    }

    return QVariant();
}

QHash<int, QByteArray> QTransfersList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(Roles::TRANSFER_DATA)] = "transferData";
    return roles;
}
