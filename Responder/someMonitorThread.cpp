/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someResponderThread.h"

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
   RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mTxMsgMetrics;
   RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mRxMsgMetrics;

   mMon_TxMsgCount.bind(&tTxMsgMetrics->mMsgCount);
   mMon_TxByteCount.bind(&tTxMsgMetrics->mByteCount);
   mMon_RxMsgCount.bind(&tRxMsgMetrics->mMsgCount);
   mMon_RxByteCount.bind(&tRxMsgMetrics->mByteCount);
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxMsgCount.update();
   mMon_RxMsgCount.update();
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
      RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mTxMsgMetrics;
      RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mRxMsgMetrics;

      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld", 
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "TxTestMsgCount           %-10d", tTxMsgMetrics->mTestMsgCount);
      Prn::print(Prn::Show1, "TxRedResponseMsgCount    %-10d", tTxMsgMetrics->mRedResponseMsgCount);
      Prn::print(Prn::Show1, "TxGreenResponseMsgCount  %-10d", tTxMsgMetrics->mGreenResponseMsgCount);
      Prn::print(Prn::Show1, "TxBlueResponseMsgCount   %-10d", tTxMsgMetrics->mBlueResponseMsgCount);

      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxTestMsgCount           %-10d", tRxMsgMetrics->mTestMsgCount);
      Prn::print(Prn::Show1, "RxRedRequestMsgCount     %-10d", tRxMsgMetrics->mRedRequestMsgCount);
      Prn::print(Prn::Show1, "RxGreenRequestMsgCount   %-10d", tRxMsgMetrics->mGreenRequestMsgCount);
      Prn::print(Prn::Show1, "RxBlueRequestMsgCount    %-10d", tRxMsgMetrics->mBlueRequestMsgCount);

      Prn::print(Prn::Show1, "");
   }

   // Show.
   if (mShowCode == 2)
   {
      RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mTxMsgMetrics;
      RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gResponderThread->mMsgMonkey->mRxMsgMetrics;
      Ris::SerialMsgPort* tMsgPort = (Ris::SerialMsgPort*)&gResponderThread->mSerialMsgThread->mSerialMsgPort;

      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "PortErrorCount           %-10d",
         tMsgPort->mPortErrorCount);

      Prn::print(Prn::Show1, "MsgErrorCount            %-10d",
         tMsgPort->mMsgErrorCount);

      Prn::print(Prn::Show1, "MsgResyncCount           %-10d",
         tMsgPort->mMsgResyncCount);

      Prn::print(Prn::Show1, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace