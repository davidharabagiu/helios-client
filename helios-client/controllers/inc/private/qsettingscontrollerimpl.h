#ifndef QSETTINGSCONTROLLERIMPL_H
#define QSETTINGSCONTROLLERIMPL_H

#include <QtGlobal>
#include <memory>

#include "settingslistener.h"

// Forward declarations
class QSettingsController;
class SettingsManager;

/**
 * @class QSettingsControllerImpl
 * @brief Private implementation of QSettingsController
 */
class QSettingsControllerImpl : public SettingsListener
{
public:
    /**
     * @brief Constructor
     * @param publicImpl - Public implementation
     */
    QSettingsControllerImpl(QSettingsController* publicImpl);

public:  // from SettingsListener
    void settingChanged(const std::string& key) override;

public:  // forwarded from QSettingsController
    qreal windowWidth() const;
    void  setWindowWidth(qreal newValue);
    qreal windowHeight() const;
    void  setWindowHeight(qreal newValue);
    bool  darkMode() const;
    void  setDarkMode(bool newValue);
    bool  fullScreen() const;
    void  setFullScreen(bool newValue);

private:
    /**
     * @brief Public implementation
     */
    QSettingsController* m_publicImpl;

    /**
     * @brief Settings manager instance
     */
    std::shared_ptr<SettingsManager> m_settingsManager;
};

#endif  // QSETTINGSCONTROLLERIMPL_H
