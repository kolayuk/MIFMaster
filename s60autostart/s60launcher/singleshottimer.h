#ifndef SINGLESHOTTIMER_H
#define SINGLESHOTTIMER_H

#include <e32std.h>

class CSingleShotTimer : public CTimer
  {
public:

	static CSingleShotTimer* NewL( TUint32 aMicroSecs );
	static CSingleShotTimer* NewLC( TUint32 aMicroSecs );

private:
  
  CSingleShotTimer();
  void ConstructL( TUint32 aMicroSecs );
  
	void DoCancel();
	void RunL();
	
  };
   
#endif // SINGLESHOTTIMER_H
