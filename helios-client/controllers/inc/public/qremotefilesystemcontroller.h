#ifndef QREMOTEFILESYSTEMCONTROLLER_H
#define QREMOTEFILESYSTEMCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <memory>

// Forward declarations
class QRemoteFileSystemControllerImpl;
class QHeliosFile;
class QFileTransfer;

/**
 * @class QRemoteFileSystemController
 * @brief Qt interface for managing a user's remote file system
 */
class QRemoteFileSystemController : public QObject
{
    Q_OBJECT

    /**
     * @property authenticationToken
     * @brief Authentication token of the current user session used to access the user's remote files
     */
    Q_PROPERTY(QString authenticationToken WRITE setAuthenticationToken RESET resetAuthenticationToken)

    /**
     * @property cwd
     * @brief Current working directory
     */
    Q_PROPERTY(QString cwd READ cwd WRITE setCwd NOTIFY cwdChanged)

    /**
     * @property files
     * @brief List of files in the current directory. QVariantList of QHeliosFile.
     */
    Q_PROPERTY(QVariantList files READ files NOTIFY filesChanged)

    /**
     * @property transfers
     * @brief List of active file transfers. QVariantList of QFileTransfer.
     */
    Q_PROPERTY(QVariantList transfers READ transfers NOTIFY transfersChanged)

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    QRemoteFileSystemController(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QRemoteFileSystemController();

    /**
     * @brief Setter for authenticationToken
     * @param newValue - QString
     */
    void setAuthenticationToken(const QString& newValue);

    /**
     * @brief Remove the current authentication token
     */
    void resetAuthenticationToken();

    /**
     * @brief Getter for cwd
     * @return QString
     */
    QString cwd() const;

    /**
     * @brief Setter for cwd
     * @param newValue - QString
     */
    void setCwd(const QString& newValue);

    /**
     * @brief Getter for files
     * @return QVariantList
     */
    QVariantList files() const;

    /**
     * @brief Getter for transfers
     * @return QVariantList
     */
    QVariantList transfers() const;

    /**
     * @brief Navigate to a directory with the given name from cwd
     * @param dirName - Directory name
     */
    Q_INVOKABLE void openDirectory(const QString& dirName);

    /**
     * @brief Navigate to the parent directory of the cwd
     */
    Q_INVOKABLE void goBack();

    /**
     * @brief Create a new directory with the given name cwd
     * @param dirName - Directory name / relative path
     */
    Q_INVOKABLE void createDirectory(const QString& dirName);

    /**
     * @brief Remove a file or directory with the given name from cwd
     * @param fileName - File name / relative path
     */
    Q_INVOKABLE void remove(const QString& fileName);

    /**
     * @brief Move a file or directory with the given name from cwd to another directory
     * @param fileName - File name / relative path
     * @param destinationDirName - Destination dir absolute path
     */
    Q_INVOKABLE void move(const QString& fileName, const QString& destinationDirPath);

    /**
     * @brief Rename a file or directory from cwd
     * @param fileName - Name of the file
     * @param newName - New name of the file
     */
    Q_INVOKABLE void rename(const QString& fileName, const QString& newName);

    /**
     * @brief Upload a file in the cwd under the same name
     * @param localPath - Local file path
     * @param encryptionKeyName - Identifier name of the key to use for file encryption
     */
    Q_INVOKABLE void upload(const QUrl& localPath, const QString& encryptionKeyName);

    /**
     * @brief Download a file from the cwd with the given name
     * @param fileName - Name of the file to download
     * @param destinationDir - Local path of the destination directory
     * @param decryptionKeyName - Identifier name of the key to use for file decryption
     */
    Q_INVOKABLE void download(const QString& fileName, const QUrl& destinationDir, const QString& decryptionKeyName);

    /**
     * @brief Cancel a file transfer
     * @param transfer - File transfer
     */
    Q_INVOKABLE void cancelTransfer(const QFileTransfer& transfer);

signals:
    /**
     * @brief SIGNAL emitted when files changes
     */
    void cwdChanged();

    /**
     * @brief SIGNAL emitted when files changes
     */
    void filesChanged();

    /**
     * @brief SIGNAL emitted when transfers changes
     */
    void transfersChanged();

    /**
     * @brief SIGNAL emitted when a file was added in cwd
     * @param directory - New file
     */
    void fileAddedInCwd(const QHeliosFile& file);

    /**
     * @brief SIGNAL emitted when a file or directory from cwd was removed
     * @param file - Removed file / directory name
     */
    void fileRemovedFromCwd(const QString& fileName);

    /**
     * @brief SIGNAL emitted when a transfer was added in the list
     * @param transfer - File transfer
     */
    void transferAdded(const QFileTransfer& transfer);

    /**
     * @brief SIGNAL emitted when a transfer was updated (progress changed)
     * @param transfer - File transfer
     */
    void transferUpdated(const QFileTransfer& transfer);

    /**
     * @brief SIGNAL emitted when a transfer was removed from the list
     * @param transfer - File transfer
     */
    void transferRemoved(const QFileTransfer& transfer);

    /**
     * @brief SIGNAL emitted when an error occured
     * @param message - Error message
     */
    void error(const QString& message);

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QRemoteFileSystemControllerImpl> m_privateImpl;
};

#endif  // QREMOTEFILESYSTEMCONTROLLER_H
