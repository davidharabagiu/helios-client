#include "qfilelisting.h"

QFileListing::QFileListing(QObject* parent)
    : QAbstractListModel(parent)
{
}

void QFileListing::setFiles(const QVariantList& files)
{
    beginResetModel();

    m_files.clear();
    for (const auto& file : files)
    {
        m_files.push_back(qvariant_cast<QHeliosFile>(file));
    }

    endResetModel();
}

int QFileListing::rowCount(const QModelIndex& /*parent*/) const
{
    return m_files.length();
}

QVariant QFileListing::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    if (role == static_cast<int>(Roles::FILE_DATA))
    {
        return QVariant::fromValue(m_files.at(index.row()));
    }

    return QVariant();
}

QHash<int, QByteArray> QFileListing::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(Roles::FILE_DATA)] = "fileData";
    return roles;
}
