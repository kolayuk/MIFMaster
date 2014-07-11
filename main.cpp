#include <QtCore/QCoreApplication>
#include <application.h>
#include <s60autostart/s60autostart.h>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    S60AutoStart::SetAutoStart(ETrue);
    QTranslator myTranslator;
      myTranslator.load("mifmaster_" + QLocale::system().name());
      a.installTranslator(&myTranslator);
    bool wasAutostart= argc > 1;
    if (wasAutostart)  User::After(5000000);
    if (wasAutostart&&Application::getAutoStartReason()==EUndefined) return 0;

    if ((!Application::checkRoot())){
        if (!Application::confNote((Application::tr("Open4All patch is not activated (it is recomended apply it to autostart), MIF files can not be installed without patch. Continue?")))) return 0;
    }
    if (Application::isUninstalling()&&Application::isPatchInstalled()){
        Application::renameAknIcon(false);
        Application::setAutoStartReason(EUninstallingApp);
        Application::note((Application::tr("You're deleting MIFMaster, device will be reboot to remove mif-replacement patch...")));
        Application::reset();
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
                Application::renameAknIcon(false);
                Application::setAutoStartReason(EInstallingMifs);
                Application::reset();
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
