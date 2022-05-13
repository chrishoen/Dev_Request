/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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

      // Set the thread notification mask.
      mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);

      // Send a message to the responder.
      sendTestMsg();

      // Wait for the completion notification.
      mNotify.wait(cRxMsgTimeout);

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