/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someRequesterThread.h"

#define  _SOMEMONITORTHREAD_CPP_
#include "someMonitorThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MonitorThread::MonitorThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Monitor");
   BaseClass::setThreadPriorityLow();
   BaseClass::mTimerPeriod = 1000;

   // Set member variables.
   mTPFlag = true;
   mShowCode = 0;

   // Bind member variables.
   Ris::SerialStringPort* tStringPort = (Ris::SerialStringPort*)&gRequesterThread->mSerialStringThread->mSerialStringPort;
   mMon_TxStringCount.bind(&tStringPort->mTxStringCount);
   mMon_TxByteCount.bind(&tStringPort->mTxByteCount);
   mMon_RxStringCount.bind(&tStringPort->mRxStringCount);
   mMon_RxByteCount.bind(&tStringPort->mRxByteCount);
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxStringCount.update();
   mMon_RxStringCount.update();
   mMon_TxByteCount.update();
   mMon_RxByteCount.update();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void MonitorThread::executeOnTimer(int aTimeCount)
{
   // Update state status variables.
   update();

   // Show.
   if (mShowCode == 1)
   {
      Prn::print(Prn::Show1, "TxStringCount               %-10d  %d",
         mMon_TxStringCount.mValue, mMon_TxStringCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount                 %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxStringCount               %-10d  %d",
         mMon_RxStringCount.mValue, mMon_RxStringCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount                 %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace