#include <QtCore/QCoreApplication>
#include <application.h>
#include <s60autostart/s60autostart.h>
#include <QDebug>
#include <QTranslator>
#include <QTime>
#include <QFile>

const QString INSTALL_REBOOT_MULTIPLE=Application::tr("Install MIF-files? Device will reboot multiple times.");
const QString OPEN4ALL_NOT_ACTIVE=Application::tr("Open4All patch is not activated (it is recomended apply it to autostart), MIF files can not be installed without patch. Continue?");
const QString DELETE_MIFMASTER_CONF=Application::tr("You're deleting MIFMaster, remove mif replacement patch (device will reboot)?");
const QString PATCH_DELETED=Application::tr("Patch deleted! Remove application one more time :)");
const QString INSTALL_REBOOT_ONCE=Application::tr("Install MIF-files? Phone will reboot.");
const QString NO_MIF_FOUND=Application::tr("No *.mif files found!");
const QString INSTALL_PATCH=Application::tr("MIF-files replacement patch is not installed, install it (reboot required)?");
const QString ALL_DONE=Application::tr("All done!");
const QString PATCH_INSTALLED=Application::tr("Patch installed! Copy MIF-files to E drive root folder and run an app");

void myMessageHandler(QtMsgType type, const char *msg)
{
        QString txt;
        QTime t=QTime::currentTime();
        QString st=t.toString("hh:mm:ss");
        switch (type) {
        case QtDebugMsg:
                txt = QString(" %1").arg(msg);
                break;
        case QtWarningMsg:
                txt = QString(" Warning: %1").arg(msg);
        break;
        case QtCriticalMsg:
                txt = QString(" Critical: %1").arg(msg);
        break;
        case QtFatalMsg:
                txt = QString(" Fatal: %1").arg(msg);
                abort();
        }
        txt=st+txt;
        QFile outFile("C:\\mifmaster.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
#ifdef Q_OS_SYMBIAN
        TPtrC des (reinterpret_cast<const TText*>(txt.constData()),txt.length());
        RDebug::Print(des);
#endif
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"launch";
    QFile file("D:\\mifmaster.txt");
    //if (file.exists()){file.remove();}
    qInstallMsgHandler(myMessageHandler);
    S60AutoStart::SetAutoStart(ETrue);
    QTranslator myTranslator;
      myTranslator.load("mifmaster_" + QLocale::system().name());
      a.installTranslator(&myTranslator);
    bool wasAutostart= argc > 1;
    qDebug()<<"autostart"<<wasAutostart;
    if (wasAutostart)  User::After(5000000);
    if (wasAutostart&&Application::getAutoStartReason()==EUndefined) return 0;

    if ((!Application::checkRoot())){
        if (!Application::confNote((OPEN4ALL_NOT_ACTIVE))) return 0;
    }
    if (Application::isUninstalling()&&Application::isPatchInstalled()){
        if (Application::confNote((DELETE_MIFMASTER_CONF)))
        {
            Application::renameAknIcon(false);
            Application::setAutoStartReason(EUninstallingApp);
            Application::reset();
        }
    }
    qDebug()<<"main";
    if (wasAutostart&&Application::getAutoStartReason()==EUninstallingApp){

        Application::removePatchAndPreparedMIFs();
        Application::setAutoStartReason(EUndefined);
        Application::note((PATCH_DELETED));
        return 0;
    }
    if (Application::isPatchInstalled()&&(!Application::isUninstalling()))
    {
        if (!Application::isAvkonReplacementNeeded()&&(!wasAutostart)){
            if (Application::confNote((INSTALL_REBOOT_ONCE)))
            {
            int mifs=Application::copyMIFs();
            Application::switchCache();
            qDebug()<<mifs;
            if (mifs!=0) Application::reset();
            else if (mifs==0) Application::note(NO_MIF_FOUND);

            }
            return 0;
        }
        if (Application::isAknIconRenamed())
        {

           Application::copyMIFs();
           Application::renameAknIcon(true);
           Application::setAutoStartReason(EAllDone);
           Application::reset();
        }
        else
        {

            if (wasAutostart)
            {
                if (Application::getAutoStartReason()==EAllDone){ Application::note((ALL_DONE)); Application::setAutoStartReason(EUndefined); return 0;}
                if (Application::getAutoStartReason()==EPatchInstalled) { Application::note((PATCH_INSTALLED)); Application::setAutoStartReason(EUndefined); return 0;}
                else return 0;
            }
            if (Application::getMifFilesCountReadyToReplace()==0) {Application::note(NO_MIF_FOUND); return 0;}
            QString reqString=(Application::getS60Version()==EBelleRefresh||
                               Application::getS60Version()==EFeaturePack1)?INSTALL_REBOOT_ONCE:INSTALL_REBOOT_MULTIPLE;
            if (Application::confNote((reqString)))
            {
                if (Application::getS60Version()==EBelleRefresh||
                    Application::getS60Version()==EFeaturePack1){
                    // one reboot way for refresh/FP1 devices
                    Application::copyMIFs(); // and kill aknicon on copying
                    Application::setAutoStartReason(EAllDone);
                    Application::reset();
                }
                else{
                    Application::renameAknIcon(false);
                    Application::setAutoStartReason(EInstallingMifs);
                    Application::reset();
                }
            }
        }
    }
    else if (!Application::isUninstalling())
    {
        if (Application::confNote((INSTALL_PATCH)))
        {
        Application::installPatch(Application::getS60Version());
        Application::prepareMIFsForPatch("C");
        Application::prepareMIFsForPatch("Z");
        Application::copyMIFs();
        Application::setAutoStartReason(EPatchInstalled);
        Application::reset();
        }
    }
    return 0;
}
