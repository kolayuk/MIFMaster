#include "application.h"
#include <QFile>
//#include <e32base.h>
//#include <f32file.h>
#include <QDir>
#include <QStringList>
#include <starterclient.h>
#include <QSettings>
#include <QDebug>
#include <hal.h>
#include <hal_data.h>
#include <QDebug>

#include <AknGlobalNote.h>
#include <aknglobalmsgquery.h>
#include <akniconconfig.h>
#include <akniconsrvclient.h>
Application::Application(QObject *parent) :
    QObject(parent)
{


}

void Application::renameAknIcon(bool enable)
{
    if (enable) QFile::rename("C:/sys/bin/AknIcon1.dll","C:/sys/bin/AknIcon.dll");
    else QFile::rename("C:/sys/bin/AknIcon.dll","C:/sys/bin/AknIcon1.dll");

}
bool Application::checkRoot()
{
    const QString checkPath=QString("C:\\sys\\bin\\test.ini");
    QSettings* test=new QSettings(checkPath,QSettings::IniFormat);
    test->setValue("1","test");
    test->sync();
    QFile f(checkPath);
    bool ex=f.exists();
    if (ex) f.remove();
    qDebug()<<"root access"<<ex;
    if (ex) {return true;}
    else return false;

}

bool Application::isAknIconRenamed()
{
    QFile f("C:/sys/bin/AknIcon1.dll");
    if (f.exists()) return true;
    else return false;

}

int Application::copyMIFs()
{
    QDir d("E:/");
    QString target=QString("E:/resource/apps/");
    QStringList mifs=d.entryList(QStringList(QString("*.mif")));
    foreach(QString file, mifs){
        clearAttribs(target+file);
        if (QFile(target+file).exists()) QFile::remove(target+file);
        QFile::copy(d.canonicalPath()+"/"+file,target+file);
        QFile::remove(d.canonicalPath()+"/"+file);

    }
    return mifs.count();
}

bool Application::isAvkonReplacementNeeded()
{
    return QFile::exists("E:/avkon2.mif");
}

void Application::switchCache()
{
    RAknIconSrvClient::Connect();
    AknIconConfig::EnableAknIconSrvCache(EFalse);
    AknIconConfig::EnableAknIconSrvCache(ETrue);
    RAknIconSrvClient::Disconnect();
}

void Application::prepareMIFsForPatch(QString drive)
{
    QDir d(drive+":/resource/apps/");
    QString target=QString("E:/resource/apps/");
    QStringList mifs=d.entryList(QStringList(QString("*.mif")));
    QSettings* pc=new QSettings("C:/system/apps/ChangeMIF/reason.ini",QSettings::IniFormat);
    pc->setValue("cat999999/localName",QString::fromUtf8("Joshlog патч"));
    pc->setValue("cat999999/checked",0);
    int i=0;
    foreach(QString file, mifs){
        if (QFile(target+file).exists()) QFile::remove(target+file);
        QFile::copy(d.canonicalPath()+"/"+file,target+file);
        clearAttribs(target+file);
        pc->setValue("cat999999/"+drive+QString::number(i),target+file);
        i++;
    }
    pc->sync();
    delete pc;
}

void Application::reset()
{
        RStarterSession starter;
        TInt err=starter.Connect();
        starter.Reset(starter.ELanguageSwitchReset);
        starter.Close();
}

void Application::clearAttribs(QString file)
{
    file=file.replace("/","\\");
    TPtrC aPath (static_cast<const TUint16*>(file.utf16()), file.length());
    TInt err;
    RFs session;
    session.Connect();
    CFileMan* manager = CFileMan::NewL(session);
    TRequestStatus status=KRequestPending;
    TTime now;
    now.HomeTime();
    int errAt=manager->Attribs(aPath,KEntryAttNormal,KEntryAttReadOnly|KEntryAttHidden|KEntryAttSystem,now,CFileMan::CFileMan::ERecurse);
    delete manager;
    session.Close();
}

void Application::note(QString text)
{
    TPtrC t (static_cast<const TUint16*>(text.utf16()), text.length());
    CAknGlobalNote* Note=CAknGlobalNote::NewL();
    TRequestStatus status=KRequestPending;
    Note->ShowNoteL(status,EAknGlobalInformationNote,t);
    User::WaitForRequest(status);
    delete Note;
}

bool Application::confNote(QString text)
{
     TPtrC aText (static_cast<const TUint16*>(text.utf16()), text.length());
     TBuf<255> aHeader(_L("MIFMaster"));
            TInt queryAnswer = 1;
            char* querystr = NULL;
            int querystr_l = 0;
            char* headerstr = NULL;
            int headerstr_l = 0;
            TRequestStatus msgQueryStatus = KRequestPending;
            TRequestStatus timerStatus = KRequestPending;
            int delayInSeconds = 0;
            TInt timeout = 0;

            TTimeIntervalMicroSeconds32 timeoutValue = TTimeIntervalMicroSeconds32(timeout);

            CAknGlobalMsgQuery *msgQueryDialog = NULL;

            TRAPD(err,
                {
                    msgQueryDialog = CAknGlobalMsgQuery::NewL();
                });
            CleanupStack::PushL(msgQueryDialog);
            TRAPD(err1,
                {
                    msgQueryDialog->ShowMsgQueryL(msgQueryStatus,aText,R_AVKON_SOFTKEYS_OK_CANCEL,aHeader, KNullDesC);
                });
            if(err1 != KErrNone||err!=KErrNone){
               CleanupStack::PopAndDestroy(msgQueryDialog);
            }
            User::WaitForRequest(msgQueryStatus);

            CleanupStack::PopAndDestroy(msgQueryDialog);
            return msgQueryStatus.Int() == EAknSoftkeyOk ? 1 : 0;
}

bool Application::isPatchInstalled()
{
    QFile f1("C:/sys/bin/AknIcon1.dll");
    QFile f2("C:/sys/bin/AknIcon.dll");
    if (f1.exists()||f2.exists()) return true;
    else return false;
}

void Application::installPatch(S60Version version)
{
    if (version==EFeaturePack1){
        TInt id;
        HAL::Get(HAL::EMachineUid,id);
        if (id==0x2002BF94||id==0x2002C12C) QFile::copy(QDir::currentPath()+"/patches/1_700_701.dll1","C:/sys/bin/AknIcon.dll");
        else if (id==0x2003AB64) QFile::copy(QDir::currentPath()+"/patches/1_808.dll1","C:/sys/bin/AknIcon.dll");
        else if (id==0x20035565) QFile::copy(QDir::currentPath()+"/patches/1_603.dll1","C:/sys/bin/AknIcon.dll");
    }
    else qDebug()<<QFile::copy(QDir::currentPath()+"/patches/"+QString::number(version)+".dll1","C:/sys/bin/AknIcon.dll");
    qDebug()<<QDir::currentPath()+"/patches/"+QString::number(version)+".dll1";
}

void Application::setAutoStartReason(TReason reason)
{
    QSettings* test=new QSettings("C:/system/apps/ChangeMIF/reason.ini",QSettings::IniFormat);
    test->setValue("reason",reason);
    test->sync();
    test->deleteLater();
}

bool Application::isUninstalling()
{
    qDebug()<<"is uninstalling";
    RWsSession* ws=new RWsSession();
    ws->Connect();
    TApaTaskList tlist(*ws);
    TApaTask t=tlist.FindApp(TUid::Uid(0x101f875a));
    bool ex=t.Exists();
    ws->Close();
    qDebug()<<1;
    delete ws;
    qDebug()<<2;
    return ex;
}

void Application::removePatchAndPreparedMIFs()
{
    QFile::remove("C:/sys/bin/AknIcon1.dll");
    QSettings* pc=new QSettings("C:/system/apps/ChangeMIF/reason.ini",QSettings::IniFormat);
    pc->beginGroup("cat999999");
    QStringList keys=pc->allKeys();
    keys.removeOne("localName");
    keys.removeOne("checked");
    pc->sync();
    foreach (QString key, keys){
        QFile::remove(pc->value(key).toString());
    }
    pc->endGroup();
    delete pc;
    QFile::remove("C:/system/apps/ChangeMIF/reason.ini");


}

TReason Application::getAutoStartReason()
{
    QSettings* test=new QSettings("C:/system/apps/ChangeMIF/reason.ini",QSettings::IniFormat);
    TReason val=(TReason)test->value("reason").toInt();
    test->deleteLater();
    return val;
}

S60Version Application::getS60Version()
{
    if (QFile::exists("Z:/system/install/Series60v5.5.sis")) return EFeaturePack2;
    else if (QFile::exists("Z:/system/install/Series60v5.4.sis")) return EFeaturePack1;
    else if (QFile::exists("Z:/system/install/Series60v5.3.sis")) return EBelleRefresh;
}
