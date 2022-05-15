/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someState.h"

#include "someScriptRunnerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked
// when a session is established or disestablished (when the serial port
// is opened or it is closed because of an error or a disconnection). 

void ScriptRunnerThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "ScriptRunnerThread CONNECTED");
   }
   else
   {
      Prn::print(Prn::Show1, "ScriptRunnerThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is bound to the qcall. Write the received string to the string
// queue and notify the long thread, which will then process the string.

void ScriptRunnerThread::executeRxString(std::string* aRxString)
{
   // Try to write the received string to the string queue.
   // It will be processed by the long thread.
   if (!mRxStringQueue.tryWrite(aRxString))
   {
      Prn::print(Prn::Show1, "RxStringQueue ERROR queue full");
      delete aRxString;
   }
   Trc::write(1, 0, "executeRxString %s", aRxString->c_str());

   // Notify the long thread.
   mNotify.notify(cRxStringNotifyCode);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace