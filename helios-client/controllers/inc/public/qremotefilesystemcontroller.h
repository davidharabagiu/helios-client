#ifndef QREMOTEFILESYSTEMCONTROLLER_H
#define QREMOTEFILESYSTEMCONTROLLER_H

#include <QObject>
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

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QRemoteFileSystemControllerImpl> m_privateImpl;
};

#endif  // QREMOTEFILESYSTEMCONTROLLER_H
