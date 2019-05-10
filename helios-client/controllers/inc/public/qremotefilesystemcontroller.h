#ifndef QREMOTEFILESYSTEMCONTROLLER_H
#define QREMOTEFILESYSTEMCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <memory>

// Forward declarations
class QRemoteFileSystemControllerImpl;

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
    Q_PROPERTY(QString cwd READ cwd NOTIFY cwdChanged)

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
     * @brief SIGNAL emitted when a directory was created in the current working directory
     * @param directory - Created directory (QHeliosFile)
     */
    void directoryCreatedInCwd(const QVariant& directory);

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QRemoteFileSystemControllerImpl> m_privateImpl;
};

#endif  // QREMOTEFILESYSTEMCONTROLLER_H
