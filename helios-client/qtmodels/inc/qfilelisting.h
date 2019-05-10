#ifndef QFILELISTING_H
#define QFILELISTING_H

#include <QAbstractListModel>

#include "qheliosfile.h"

/**
 * @class QFileListing
 * @brief Provides a list model of QHeliosFile objects
 */
class QFileListing : public QAbstractListModel
{
    Q_OBJECT

    /**
     * @property files
     * @brief Files data
     */
    Q_PROPERTY(QVariantList files WRITE setFiles)

public:  // Enums
    enum class Roles
    {
        FILE_DATA = Qt::UserRole
    };

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QFileListing(QObject* parent = nullptr);

    /**
     * @brief Setter for files property
     * @param files - QVariantList
     */
    void setFiles(const QVariantList& files);

public:  // from QAbstractListModel
    int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    /**
     * @brief Files
     */
    QList<QHeliosFile> m_files;
};

#endif  // QFILELISTING_H
