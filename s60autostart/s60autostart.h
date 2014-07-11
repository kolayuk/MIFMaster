#ifndef S60AUTOSTART_H
#define S60AUTOSTART_H

#include <bautils.h>

_LIT(KAutoStartSettingFileFmt, "c:\\system\\data\\%S.ini");
_LIT(KAppExtension, ".exe");

namespace S60AutoStart {


    /**
     * TBool IsAutoStartEnabled()
     * Description: Checks if autostart ini file exists for this application
     * Return value: true if autostart is enabled.
     */
    TBool IsAutoStartEnabled();


    /**
     * void SetAutoStart( TBool aAuto )
     * Description: Enables or disables autostart for this application, by
     *              by creating/deleting the ini file, respectively.
     * Parameters: TBool aAuto - true if this application is to be started
     *                           during device bootup process.
     * Return value: n/a
     */
    void SetAutoStart( TBool aAuto );


    /**
     * TBool DoSetAutoStart( TBool aSet, TBool aAuto )
     * Description: Internal function, use the above instead
     */
    TBool DoSetAutoStart( TBool aSet, TBool aAuto = EFalse );

}

#endif // S60AUTOSTART_H

// eof
