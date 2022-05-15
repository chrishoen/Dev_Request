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

      // Set the thread notification mask and flush the string queue.
      mNotify.setMaskOne("RxString", cRxStringNotifyCode);
      flushRxStringQueue();

      // Send a string to the responder.
      std::string* tTxString = new std::string("test");
      sendString(tTxString);

      // Wait for the response string notification.
      mNotify.wait(cNoTimeout);

      // Read the receive string from the string queue and process it.
      std::string* tRxString = mRxStringQueue.tryRead();
      if (tRxString == 0) throw 888;

      // Show.
      Prn::print(Prn::Show1, "executeTest1 %s", tRxString->c_str());
      delete tRxString;
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