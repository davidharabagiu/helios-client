#include "qsettingscontroller.h"
#include "qsettingscontrollerimpl.h"

QSettingsController::QSettingsController(QObject* parent)
    : QObject(parent)
    , m_privateImpl(new QSettingsControllerImpl(this))
{
}

qreal QSettingsController::windowWidth() const
{
    return m_privateImpl->windowWidth();
}

void QSettingsController::setWindowWidth(qreal newValue)
{
    m_privateImpl->setWindowWidth(newValue);
}

qreal QSettingsController::windowHeight() const
{
    return m_privateImpl->windowHeight();
}

void QSettingsController::setWindowHeight(qreal newValue)
{
    m_privateImpl->setWindowHeight(newValue);
}

bool QSettingsController::darkMode() const
{
    return m_privateImpl->darkMode();
}

void QSettingsController::setDarkMode(bool newValue)
{
    m_privateImpl->setDarkMode(newValue);
}

bool QSettingsController::fullScreen() const
{
    return m_privateImpl->fullScreen();
}

void QSettingsController::setFullScreen(bool newValue)
{
    m_privateImpl->setFullScreen(newValue);
}
