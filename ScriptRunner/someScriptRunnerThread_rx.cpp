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
// qcall registered to the mSerialMsgThread child thread. It is invoked
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
// This is bound to the qcall. Write the received message to the message
// queue and notify the long thread, which will then process the message.

void ScriptRunnerThread::executeRxMsg(Ris::ByteContent* aRxMsg)
{
   // Try to write the received message to the message queue.
   // It will be processed by the long thread.
   RGB::BaseMsg* tRxMsg = (RGB::BaseMsg*)aRxMsg;
   if (!mRxMsgQueue.tryWrite(tRxMsg))
   {
      Prn::print(Prn::Show1, "RxMsgQueue ERROR queue full");
      delete tRxMsg;
   }
   // Notify the long thread.
   mNotify.notify(cRxMsgNotifyCode);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace