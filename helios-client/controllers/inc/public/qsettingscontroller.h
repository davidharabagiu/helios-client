#ifndef QSETTINGSCONTROLLER_H
#define QSETTINGSCONTROLLER_H

#include <QObject>
#include <memory>

// Forward declarations
class QSettingsControllerImpl;

/**
 * @class QSettingsController
 * @brief Provides a way to access application settings from QML domain
 */
class QSettingsController : public QObject
{
    Q_OBJECT

    /**
     * @property windowWidth
     * @brief Application window width in pixels
     */
    Q_PROPERTY(qreal windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)

    /**
     * @property windowWidth
     * @brief Application window height in pixels
     */
    Q_PROPERTY(qreal windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)

    /**
     * @property windowWidth
     * @brief UI dark mode active
     */
    Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged)

    /**
     * @property fullScreen
     * @brief Window full screen mode active
     */
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QSettingsController(QObject* parent = nullptr);

    /**
     * @brief Getter for windowWidth
     * @return qreal
     */
    qreal windowWidth() const;

    /**
     * @brief Setter for windowWidth
     * @param newValue - qreal
     */
    void setWindowWidth(qreal newValue);

    /**
     * @brief Getter for windowHeight
     * @return qreal
     */
    qreal windowHeight() const;

    /**
     * @brief Setter for windowHeight
     * @param newValue - qreal
     */
    void setWindowHeight(qreal newValue);

    /**
     * @brief Getter for darkMode
     * @return bool
     */
    bool darkMode() const;

    /**
     * @brief Setter for darkMode
     * @param newValue - bool
     */
    void setDarkMode(bool newValue);

    /**
     * @brief Getter for fullScreen
     * @return bool
     */
    bool fullScreen() const;

    /**
     * @brief Setter for fullScreen
     * @param newValue - bool
     */
    void setFullScreen(bool newValue);

signals:
    /**
     * @brief SIGNAL emitted when windowWidth changes
     */
    void windowWidthChanged();

    /**
     * @brief SIGNAL emitted when windowHeight changes
     */
    void windowHeightChanged();

    /**
     * @brief SIGNAL emitted when darkMode changes
     */
    void darkModeChanged();

    /**
     * @brief SIGNAL emitted when fullScreen changes
     */
    void fullScreenChanged();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QSettingsControllerImpl> m_privateImpl;
};

#endif  // QSETTINGSCONTROLLER_H
