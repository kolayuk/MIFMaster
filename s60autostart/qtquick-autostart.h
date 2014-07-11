#ifndef QTQUICKAUTOSTART_H
#define QTQUICKAUTOSTART_H

#include <QtDeclarative>

/**
 * class AutoStart
 * Description: class that defines functionality for AutoStart QML element
 */
class QmlAutoStart : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY toggled)
    Q_PROPERTY(QString launchType READ launchType)

public:
    QmlAutoStart(QObject *parent = 0, bool autoLaunch = false);
    ~QmlAutoStart() {}

    QString launchType();
    bool enabled();
    void setEnabled(bool enabled);

signals:
    void toggled();

private:
        QString m_launchType;
};


class QmlAutoStartAuto : public QmlAutoStart
{
    Q_OBJECT

public:
    QmlAutoStartAuto(QObject *parent = 0) : QmlAutoStart(parent, true) {}
    ~QmlAutoStartAuto() {}
};


extern void qmlReqisterAutoStart(bool autoLaunch);

#define qmlInitAutoStart() qmlReqisterAutoStart(argc > 1)

#endif // QTQUICKAUTOSTART_H
