#include "s60autostart.h"


TBool S60AutoStart::IsAutoStartEnabled()
{
    return S60AutoStart::DoSetAutoStart( EFalse );
}


void S60AutoStart::SetAutoStart( TBool aAuto )
{
    S60AutoStart::DoSetAutoStart( ETrue, aAuto );
}


TBool S60AutoStart::DoSetAutoStart( TBool aSet, TBool aAuto )
{
    TBool autostart = EFalse;
    RFs fs;
    if( KErrNone == fs.Connect() ) {
        TFileName autoStartSettingFile;
        RProcess thisProc;
        TFileName procName( thisProc.FileName() );
        TParsePtr nameParser( procName );
        autoStartSettingFile.Format( KAutoStartSettingFileFmt, &nameParser.Name() );

        if( aSet ) {            // enable or disable autostart
            if ( !aAuto ) {
                BaflUtils::DeleteFile( fs, autoStartSettingFile );
            }
            else {
                RFile file;
                file.Create(fs, autoStartSettingFile, EFileShareAny|EFileWrite);
                file.Close();
            }
        }
        else {                  // just check current autostart setting
            autostart = BaflUtils::FileExists( fs, autoStartSettingFile );
        }

        fs.Close();
    }
    return autostart;
}

// eof
