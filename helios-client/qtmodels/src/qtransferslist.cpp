#include <algorithm>

#include "qtransferslist.h"
#include "typeconversions.h"

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

void QTransfersList::addTranfer(const QFileTransfer& transfer)
{
    beginInsertRows(QModelIndex(), m_transfers.length(), m_transfers.length());
    m_transfers.push_back(transfer);
    endInsertRows();
}

void QTransfersList::updateTransfer(const QFileTransfer& transfer)
{
    auto it = std::find_if(m_transfers.begin(), m_transfers.end(),
                           [&transfer](const QFileTransfer& el) { return el.data() == transfer.data(); });
    assert(it != m_transfers.end());
    if (it != m_transfers.end())
    {
        int  idx = safe_integral_cast<int>(std::distance(m_transfers.begin(), it));
        emit dataChanged(index(idx), index(idx), {static_cast<int>(Roles::TRANSFER_DATA)});
    }
}

void QTransfersList::removeTransfer(const QFileTransfer& transfer)
{
    auto it = std::find_if(m_transfers.begin(), m_transfers.end(),
                           [&transfer](const QFileTransfer& el) { return el.data() == transfer.data(); });
    assert(it != m_transfers.end());
    if (it != m_transfers.end())
    {
        int idx = safe_integral_cast<int>(std::distance(m_transfers.begin(), it));
        beginRemoveRows(QModelIndex(), idx, idx);
        m_transfers.erase(it);
        endRemoveRows();
    }
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
