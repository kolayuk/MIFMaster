#include "qtquick-autostart.h"

#if defined(Q_OS_SYMBIAN)
#include "s60autostart.h"
#endif

const char *autostartQmlUri = "com.nokia.autostart.symbian";

QmlAutoStart::QmlAutoStart(QObject *parent, bool autoLaunch)
    : QObject(parent)
{
    m_launchType = autoLaunch ? "auto" : "normal";
}


QString QmlAutoStart::launchType()
{
    return m_launchType;
}


bool QmlAutoStart::enabled()
{
#if defined(Q_OS_SYMBIAN)
        return S60AutoStart::IsAutoStartEnabled();
#else
        return false;
#endif
}


void QmlAutoStart::setEnabled(bool enabled)
{
#if defined(Q_OS_SYMBIAN)
        S60AutoStart::SetAutoStart(enabled);
#else
    Q_UNUSED(enabled)
#endif
        emit toggled();
}

void qmlReqisterAutoStart(bool autoLaunch)
{
    if (autoLaunch)
        // @uri com.nokia.autostart.symbian
        qmlRegisterType<QmlAutoStartAuto>(autostartQmlUri, 1, 0, "AutoStart");
    else
        // @uri com.nokia.autostart.symbian
        qmlRegisterType<QmlAutoStart>(autostartQmlUri, 1, 0, "AutoStart");
}

// eof
