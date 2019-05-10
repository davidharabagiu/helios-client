#ifndef QTRANSFERSLIST_H
#define QTRANSFERSLIST_H

#include <QAbstractListModel>

#include "qfiletransfer.h"

/**
 * @class QTransfersList
 * @brief Provides a list model of QFileTransfer objects
 */
class QTransfersList : public QAbstractListModel
{
    Q_OBJECT

    /**
     * @property transfers
     * @brief Transfers data
     */
    Q_PROPERTY(QVariantList transfers WRITE setTransfers)

public:  // Enums
    enum class Roles
    {
        TRANSFER_DATA = Qt::UserRole
    };

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QTransfersList(QObject* parent = nullptr);

    /**
     * @brief Setter for transfers property
     * @param transfers - QVariantList
     */
    void setTransfers(const QVariantList& transfers);

public:  // from QAbstractListModel
    int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    /**
     * @brief File transfers
     */
    QList<QFileTransfer> m_transfers;
};

#endif  // QTRANSFERSLIST_H
