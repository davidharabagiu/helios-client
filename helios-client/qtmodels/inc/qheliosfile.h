#ifndef QHELIOSFILE_H
#define QHELIOSFILE_H

#include <QMetaType>
#include <QString>
#include <memory>

// Forward declarations
class File;

/**
 * @class QHeliosFile
 * @brief Qt wrapper over File
 */
class QHeliosFile
{
    Q_GADGET

    /**
     * @property name
     * @brief File name
     */
    Q_PROPERTY(QString name READ name)

    /**
     * @property fullPath
     * @brief Full path of the file
     */
    Q_PROPERTY(QString fullPath READ fullPath)

    /**
     * @property isDirectory
     * @brief Specifies if the file is a directory
     */
    Q_PROPERTY(bool isDirectory READ isDirectory)

    /**
     * @property size
     * @brief File size in bytes. Valid only if isDirectory = false
     */
    Q_PROPERTY(quint64 size READ size)

    /**
     * @property isValid
     * @brief Validity check
     */
    Q_PROPERTY(bool isValid READ isValid)

public:
    /**
     * @brief Constructs an invalid QHeliosFile
     */
    QHeliosFile() = default;

    /**
     * @brief Constructor
     * @param data - Internal data pointer
     */
    QHeliosFile(const std::shared_ptr<const File>& data);

    /**
     * @brief Getter for isValid
     * @return bool
     */
    bool isValid() const;

public:  // Wrappers over File
    QString name() const;
    QString fullPath() const;
    bool    isDirectory() const;
    quint64 size() const;

private:
    /**
     * @brief Internal data
     */
    std::shared_ptr<const File> m_data;
};

Q_DECLARE_METATYPE(QHeliosFile)

#endif  // QHELIOSFILE_H
