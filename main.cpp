#include <QtCore/QCoreApplication>
#include <application.h>
#include <s60autostart/s60autostart.h>
#include <QDebug>
#include <QTranslator>
#include <QTime>
#include <QFile>

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
        if (!Application::confNote((Application::tr("Open4All patch is not activated (it is recomended apply it to autostart), MIF files can not be installed without patch. Continue?")))) return 0;
    }
    if (Application::isUninstalling()&&Application::isPatchInstalled()){
        if (Application::confNote((Application::tr("You're deleting MIFMaster, device will be reboot to remove mif-replacement patch..."))))
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
        Application::note((Application::tr("Patch deleted! Remove application one more time :)")));
        return 0;
    }
    if (Application::isPatchInstalled()&&(!Application::isUninstalling()))
    {
        if (!Application::isAvkonReplacementNeeded()&&(!wasAutostart)){
            if (Application::confNote((Application::tr("Install MIF-files? Phone will reboot."))))
            {
            int mifs=Application::copyMIFs();
            Application::switchCache();
            qDebug()<<mifs;
            if (mifs!=0) Application::reset();
            else if (mifs==0) Application::note(Application::tr("No *.mif files found!"));

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
                if (Application::getAutoStartReason()==EAllDone){ Application::note((Application::tr("All done!"))); Application::setAutoStartReason(EUndefined); return 0;}
                if (Application::getAutoStartReason()==EPatchInstalled) { Application::note((Application::tr("Patch installed! Copy MIF-files to E drive root folder and run an app"))); Application::setAutoStartReason(EUndefined); return 0;}
                else return 0;
            }
            if (Application::confNote((Application::tr("Install MIF-files? Device will reboot multiple times."))))
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
        if (Application::confNote((Application::tr("MIF-files replacement patch is not installed, install it (reboot required)?"))))
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
