/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "rgbMsg.h"
#include "someScriptRunnerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeTest1()
{
   try
   {
      Prn::print(Prn::Show1, "ScriptRunnerThread::executeTest1 BEGIN");

      // Set the thread notification mask and flush the message queue.
      mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);
      mRxMsgQueue.flushRead();

      // Send a message to the responder.
      sendTestMsg();

      // Wait for the response message notification.
      mNotify.wait(cRxMsgTimeout);

      // Read the receive message from the message queue and process it.
      RGB::TestMsg* tRxMsg = (RGB::TestMsg*)mRxMsgQueue.tryRead();
      if (tRxMsg == 0) throw 888;
      if (tRxMsg->mMessageType != RGB::MsgIdT::cTestMsg) throw 889;

      // Show.
      tRxMsg->show(Prn::Show1);

      Prn::print(Prn::Show1, "ScriptRunnerThread::executeTest1 END");
   }
   catch (int aException)
   {
      Prn::print(0, "EXCEPTION ScriptRunnerThread::executeTest1 %d %s", aException, mNotify.mException);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace