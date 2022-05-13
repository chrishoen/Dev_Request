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

void ScriptRunnerThread::executeRunScript()
{
   // Print and log.
   Prn::print(0, "");
   Prn::print(0, "running script");
   Prn::print(Prn::Show1, "ScriptSeqThread::executeRunScript BEGIN");

   // Initialize variables.
   mLoopExitCode = 0;
   gState.setStateSX2_Running();
   gState.initializeTime2();

   // Open the script file.
   mReadCount = 0;
   if (!mScript.doOpen("files/script_rgb.txt")) throw 991;

   // Set the state and send a message to the controller.
   gState.setStateSX2_Running();

   try
   {
      // Loop through the script file.
      while (true)
      {
         // Read a command from the script file. Exit if end of file.
         if (mScript.doRead(&mCmd))
         {
            if (mCmd.isCmd("EXIT")) break;
         }
         mReadCount++;

         // Test for a notification exception.
         mNotify.testException();

         // Delay.
         mNotify.waitForTimer(cScriptThrottle);

         // Execute the script file command.
         execute(&mCmd);
      }
   }
   catch (int aException)
   {
      mLoopExitCode = cLoopExitAborted;
      Prn::print(0, "EXCEPTION ScriptSeqThread::executeRunScript %d %s", aException, mNotify.mException);
   }

   // Test the exit code.
   if (mLoopExitCode == cLoopExitNormal)
   {
      // Print and log.
      Prn::print(0, "script done");
      Prn::print(0, "");

      // Set the state and send a message to the controller.
      gState.setStateSX2_Done();

      // Set the state and send a message to the controller.
      gState.setStateSX2_None();
   }
   else if (mLoopExitCode == cLoopExitAborted)
   {
      // Print and log.
      Prn::print(0, "script aborted");
      Prn::print(0, "");

      // Set the state and send a message to the controller.
      gState.setStateSX2_Aborted();
   }

   // Close the script file.
   mScript.doClose();

   Prn::print(Prn::Show1, "ScriptSeqThread::executeRunScript END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command. It calls one of
// the following specific command execution functions.

void ScriptRunnerThread::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RED"))    executeRed(aCmd);
   if (aCmd->isCmd("GREEN"))  executeGreen(aCmd);
   if (aCmd->isCmd("BLUE"))   executeBlue(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeRed(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::Show4, "executeRed");

   // Set the thread notification mask and flush the message queue.
   mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);
   mRxMsgQueue.flushRead();

   // Send a message to the responder.
   RGB::RedRequestMsg* tTxMsg = new RGB::RedRequestMsg;
   mSerialMsgThread->sendMsg(tTxMsg);

   // Wait for the response message notification.
   mNotify.wait(cRxMsgTimeout);

   // Read the receive message from the message queue and process it.
   RGB::RedResponseMsg* tRxMsg = (RGB::RedResponseMsg*)mRxMsgQueue.tryRead();
   if (tRxMsg == 0) throw 888;
   if (tRxMsg->mMessageType != RGB::MsgIdT::cRedResponseMsg) throw 889;

   Prn::print(Prn::Show4, "executeRed done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeGreen(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::Show4, "executeGreen");

   // Set the thread notification mask and flush the message queue.
   mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);
   mRxMsgQueue.flushRead();

   // Send a message to the responder.
   RGB::GreenRequestMsg* tTxMsg = new RGB::GreenRequestMsg;
   mSerialMsgThread->sendMsg(tTxMsg);

   // Wait for the response message notification.
   mNotify.wait(cRxMsgTimeout);

   // Read the receive message from the message queue and process it.
   RGB::GreenResponseMsg* tRxMsg = (RGB::GreenResponseMsg*)mRxMsgQueue.tryRead();
   if (tRxMsg == 0) throw 888;
   if (tRxMsg->mMessageType != RGB::MsgIdT::cGreenResponseMsg) throw 889;

   Prn::print(Prn::Show4, "executeGreen done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeBlue(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::Show4, "executeBlue");

   // Set the thread notification mask and flush the message queue.
   mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);
   mRxMsgQueue.flushRead();

   // Send a message to the responder.
   RGB::BlueRequestMsg* tTxMsg = new RGB::BlueRequestMsg;
   mSerialMsgThread->sendMsg(tTxMsg);

   // Wait for the response message notification.
   mNotify.wait(cRxMsgTimeout);

   // Read the receive message from the message queue and process it.
   RGB::BlueResponseMsg* tRxMsg = (RGB::BlueResponseMsg*)mRxMsgQueue.tryRead();
   if (tRxMsg == 0) throw 888;
   if (tRxMsg->mMessageType != RGB::MsgIdT::cBlueResponseMsg) throw 889;

   Prn::print(Prn::Show4, "executeBlue done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace