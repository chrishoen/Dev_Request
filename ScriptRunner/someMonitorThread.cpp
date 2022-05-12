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
   RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mTxMsgMetrics;
   RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mRxMsgMetrics;

   mMon_TxMsgCount.bind(&tTxMsgMetrics->mMsgCount);
   mMon_TxByteCount.bind(&tTxMsgMetrics->mByteCount);
   mMon_TxRedRequestMsgCount.bind(&tTxMsgMetrics->mRedRequestMsgCount);
   mMon_TxGreenRequestMsgCount.bind(&tTxMsgMetrics->mGreenRequestMsgCount);
   mMon_TxBlueRequestMsgCount.bind(&tTxMsgMetrics->mBlueRequestMsgCount);

   mMon_RxMsgCount.bind(&tRxMsgMetrics->mMsgCount);
   mMon_RxByteCount.bind(&tRxMsgMetrics->mByteCount);
   mMon_RxRedResponseMsgCount.bind(&tRxMsgMetrics->mRedResponseMsgCount);
   mMon_RxGreenResponseMsgCount.bind(&tRxMsgMetrics->mGreenResponseMsgCount);
   mMon_RxBlueResponseMsgCount.bind(&tRxMsgMetrics->mBlueResponseMsgCount);
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxMsgCount.update();
   mMon_TxByteCount.update();
   mMon_TxRedRequestMsgCount.update();
   mMon_TxGreenRequestMsgCount.update();
   mMon_TxBlueRequestMsgCount.update();

   mMon_RxMsgCount.update();
   mMon_RxByteCount.update();
   mMon_RxRedResponseMsgCount.update();
   mMon_RxGreenResponseMsgCount.update();
   mMon_RxBlueResponseMsgCount.update();
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
      RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mTxMsgMetrics;
      RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mRxMsgMetrics;

      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld", 
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "TxTestMsgCount           %-10d", tTxMsgMetrics->mTestMsgCount);

      Prn::print(Prn::Show1, "TxRedRequestMsgCount     %-10d  %d",
         mMon_TxRedRequestMsgCount.mValue, mMon_TxRedRequestMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxGreenRequestMsgCount   %-10d  %d",
         mMon_TxGreenRequestMsgCount.mValue, mMon_TxGreenRequestMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxBlueRequestMsgCount    %-10d  %d",
         mMon_TxBlueRequestMsgCount.mValue, mMon_TxBlueRequestMsgCount.mDelta);

      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxTestMsgCount           %-10d", tRxMsgMetrics->mTestMsgCount);

      Prn::print(Prn::Show1, "RxRedResponseMsgCount    %-10d  %d",
         mMon_RxRedResponseMsgCount.mValue, mMon_RxRedResponseMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxGreenResponseMsgCount  %-10d  %d",
         mMon_RxGreenResponseMsgCount.mValue, mMon_RxGreenResponseMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxBlueResponseMsgCount   %-10d  %d",
         mMon_RxBlueResponseMsgCount.mValue, mMon_RxBlueResponseMsgCount.mDelta);

      Prn::print(Prn::Show1, "");
   }

   // Show.
   if (mShowCode == 2)
   {
      RGB::MsgMetrics* tTxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mTxMsgMetrics;
      RGB::MsgMetrics* tRxMsgMetrics = (RGB::MsgMetrics*)gRequesterThread->mMsgMonkey->mRxMsgMetrics;
      Ris::SerialMsgPort* tMsgPort = (Ris::SerialMsgPort*)&gRequesterThread->mSerialMsgThread->mSerialMsgPort;

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