
#include <e32std.h>
#include "s60autostart.h"
#include "singleshottimer.h"

_LIT(KTxtAppName, "S60Launcher");
_LIT(KCommand, "autostart");


LOCAL_C void StartL()
  {
    
  TFileName autoStartSettingFile;
  
  RProcess proc;
  TName procName( proc.Name() );
  
  // Resolve the name[UID] of the app-to-be-launched
  procName.Delete(0,3);                                    // delete 'as_' from the beginning   
  procName.SetLength( procName.Find( KAppExtension ) );    // remove '.exe[UID]' of this process  
  autoStartSettingFile.Format( KAutoStartSettingFileFmt, &procName );
  
  RFs fs;
  User::LeaveIfError( fs.Connect() );    
  if( BaflUtils::FileExists( fs, autoStartSettingFile ) )
    {
    proc.Create( procName, KCommand );
    proc.Resume();
    }
    
  fs.Close();
  proc.Close();
  
  CSingleShotTimer* timer = CSingleShotTimer::NewLC( 5000000 ); // wait 5s, then terminate
  
  // start active scheduler wait loop - active object will call Stop() on exit
  CActiveScheduler::Start();
    
  CleanupStack::PopAndDestroy(timer);
  }


GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CActiveScheduler* scheduler = new CActiveScheduler;
	if(scheduler)
		{
		CActiveScheduler::Install(scheduler);
		CTrapCleanup* cleanup = CTrapCleanup::New();
		if (cleanup)
			{
			TRAPD( err, StartL() );	
			__ASSERT_ALWAYS(err == KErrNone, User::Panic(KTxtAppName, err) ); 			
			}
		delete scheduler;
		delete cleanup;
		}
	__UHEAP_MARKEND;
	return KErrNone;
  }
  
// eof
