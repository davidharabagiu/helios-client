#include <algorithm>

#include "qfilelisting.h"
#include "qheliosfile.h"

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

    std::sort(m_files.begin(), m_files.end(),
              [](const QHeliosFile& file1, const QHeliosFile& file2) { return file1.name() < file2.name(); });

    endResetModel();
}

void QFileListing::insertFile(const QHeliosFile& file)
{
    int  idx = 0;
    auto it  = std::find_if(m_files.begin(), m_files.end(), [&file, &idx](const QHeliosFile& el) {
        if (file.name() < el.name())
        {
            return true;
        }
        ++idx;
        return false;
    });

    beginInsertRows(QModelIndex(), idx, idx);
    m_files.insert(it, file);
    endInsertRows();
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
