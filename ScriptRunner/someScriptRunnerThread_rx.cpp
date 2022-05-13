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
// qcall registered to the mSerialMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

void ScriptRunnerThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   RGB::BaseMsg* tMsg = (RGB::BaseMsg*)aMsg;
   delete aMsg;
   mNotify.notify(cRxMsgNotifyCode);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace