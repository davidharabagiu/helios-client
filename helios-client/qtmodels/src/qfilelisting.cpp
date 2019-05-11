#include <algorithm>

#include "qfilelisting.h"
#include "qheliosfile.h"
#include "typeconversions.h"

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
    auto it    = std::find_if(m_files.begin(), m_files.end(),
                           [&file](const QHeliosFile& el) { return file.name() < el.name(); });
    int  index = safe_integral_cast<int>(std::distance(m_files.begin(), it));

    beginInsertRows(QModelIndex(), index, index);
    m_files.insert(it, file);
    endInsertRows();
}

void QFileListing::removeFile(const QString& fileName)
{
    auto it = std::find_if(m_files.begin(), m_files.end(),
                           [&fileName](const QHeliosFile& el) { return el.name() == fileName; });
    if (it != m_files.end())
    {
        int index = safe_integral_cast<int>(std::distance(m_files.begin(), it));
        beginRemoveRows(QModelIndex(), index, index);
        m_files.removeAt(index);
        endRemoveRows();
    }
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
