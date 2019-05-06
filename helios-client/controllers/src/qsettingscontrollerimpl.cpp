#include <QDebug>
#include <QMetaObject>
#include <map>

#include "qsettingscontroller.h"
#include "qsettingscontrollerimpl.h"
#include "single.h"
#include "dependencyinjector.h"
#include "settingsmanager.h"
#include "defaultsettingskeys.h"

namespace
{
const std::map<std::string, const char*> kSettingKeysToSignalNamesMap = {
    std::make_pair(SettingsKeys::kWindowWidth, "windowWidthChanged"),
    std::make_pair(SettingsKeys::kWindowHeight, "windowHeightChanged"),
    std::make_pair(SettingsKeys::kDarkMode, "darkModeChanged"),
    std::make_pair(SettingsKeys::kFullScreen, "fullScreenChanged")};
}

QSettingsControllerImpl::QSettingsControllerImpl(QSettingsController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_settingsManager = Single<DependencyInjector>::instance().getInstance<SettingsManager>();
    if (!m_settingsManager)
    {
        qFatal("SettingsManager instance not available");
    }
}

void QSettingsControllerImpl::registerForNotifications()
{
    m_settingsManager->registerListener(shared_from_this());
}

void QSettingsControllerImpl::unregisterFromNotifications()
{
    m_settingsManager->unregisterListener(shared_from_this());
}

void QSettingsControllerImpl::settingChanged(const std::string& key)
{
    auto it = kSettingKeysToSignalNamesMap.find(key);
    if (it != kSettingKeysToSignalNamesMap.end())
    {
        QMetaObject::invokeMethod(m_publicImpl, it->second, Qt::QueuedConnection);
    }
}

qreal QSettingsControllerImpl::windowWidth() const
{
    QVariant width = m_settingsManager->get(SettingsKeys::kWindowWidth);
    if (width.type() == QVariant::Double)
    {
        return static_cast<qreal>(width.toDouble());
    }
    return 0;
}

void QSettingsControllerImpl::setWindowWidth(qreal newValue)
{
    if (newValue > 0)
    {
        m_settingsManager->set(SettingsKeys::kWindowWidth, static_cast<double>(newValue));
    }
}

qreal QSettingsControllerImpl::windowHeight() const
{
    QVariant height = m_settingsManager->get(SettingsKeys::kWindowHeight);
    if (height.type() == QVariant::Double)
    {
        return static_cast<qreal>(height.toDouble());
    }
    return 0;
}

void QSettingsControllerImpl::setWindowHeight(qreal newValue)
{
    if (newValue > 0)
    {
        m_settingsManager->set(SettingsKeys::kWindowHeight, static_cast<double>(newValue));
    }
}

bool QSettingsControllerImpl::darkMode() const
{
    QVariant dm = m_settingsManager->get(SettingsKeys::kDarkMode);
    if (dm.type() == QVariant::Bool)
    {
        return dm.toBool();
    }
    return false;
}

void QSettingsControllerImpl::setDarkMode(bool newValue)
{
    m_settingsManager->set(SettingsKeys::kDarkMode, newValue);
}

bool QSettingsControllerImpl::fullScreen() const
{
    QVariant fs = m_settingsManager->get(SettingsKeys::kFullScreen);
    if (fs.type() == QVariant::Bool)
    {
        return fs.toBool();
    }
    return false;
}

void QSettingsControllerImpl::setFullScreen(bool newValue)
{
    m_settingsManager->set(SettingsKeys::kFullScreen, newValue);
}
