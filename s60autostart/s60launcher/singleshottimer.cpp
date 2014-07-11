#include <e32base.h>
#include "singleshottimer.h"


CSingleShotTimer::CSingleShotTimer()
: CTimer( CActive::EPriorityLow )
    {
	  CActiveScheduler::Add( this );		
    }


CSingleShotTimer* CSingleShotTimer::NewL( TUint32 aMicroSecs )
  {
  CSingleShotTimer* self = CSingleShotTimer::NewLC( aMicroSecs );
  CleanupStack::Pop(); // self
  return self;
  }


CSingleShotTimer* CSingleShotTimer::NewLC( TUint32 aMicroSecs )
  {
  CSingleShotTimer* self = new (ELeave) CSingleShotTimer;
  CleanupStack::PushL( self ) ;
  self->ConstructL( aMicroSecs );
  return self;
  }  


void CSingleShotTimer::ConstructL( TUint32 aMicroSecs )
  {
  CTimer::ConstructL();
  After( aMicroSecs );
  }


void CSingleShotTimer::DoCancel()
    {
    CTimer::DoCancel();
    }


void CSingleShotTimer::RunL()
	{
  CActiveScheduler::Stop();
	}

// eof
