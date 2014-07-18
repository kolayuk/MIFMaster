#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
enum S60Version{
    EBelleRefresh,
    EFeaturePack1,
    EFeaturePack2
};
enum TReason{
    EAllDone,
    EPatchInstalled,
    EUndefined,
    EInstallingMifs,
    EUninstallingApp
};

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);
    static void renameAknIcon(bool enable);
    static bool checkRoot();
    static bool isAknIconRenamed();
    static int copyMIFs();
    static bool isAvkonReplacementNeeded();
    static void switchCache();
    static void prepareMIFsForPatch(QString drive);
    static void reset();
    static void getPathForMIF(QString name,QStringList& paths);
    //static bool checkResourceFile(QString drives, QString miffile);
    static void clearAttribs(QString file);
    static void note(QString text);
    static bool confNote(QString text);
    static bool isPatchInstalled();
    static void installPatch(S60Version version);
    static void setAutoStartReason(TReason reason);
    static bool isUninstalling();
    static void removePatchAndPreparedMIFs();
    static TReason getAutoStartReason();
    static S60Version getS60Version();
    static void killProcess(QString uid);
signals:
    
public slots:
    
};

#endif // APPLICATION_H
