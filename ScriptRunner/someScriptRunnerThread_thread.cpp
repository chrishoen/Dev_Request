/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "cmnPriorities.h"
#include "someState.h"
#include "someSerialParms.h"

#define  _SOMESCRIPTRUNNERTHREAD_CPP_
#include "someScriptRunnerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ScriptRunnerThread::ScriptRunnerThread()
   : mGCodeAckNotify(&mNotify, cGCodeAckNotifyCode),
   mLcdNotify(&mNotify, cLcdNotifyCode)
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mShortThread->setThreadName("ScriptShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mScriptShort);

   BaseClass::mLongThread->setThreadName("ScriptLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mScriptLong);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer = std::bind(&ScriptRunnerThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer = std::bind(&ScriptRunnerThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&ScriptRunnerThread::executeOnTimer, this, _1);

   // Bind qcalls.
   mSessionQCall.bind (this->mShortThread, this, &ScriptRunnerThread::executeSession);
   mRxMsgQCall.bind   (this->mShortThread, this, &ScriptRunnerThread::executeRxMsg);
   mAbortQCall.bind   (this->mShortThread, this, &ScriptRunnerThread::executeAbort);

   // Bind qcalls.
   mTest1QCall.bind(this->mLongThread, this, &ScriptRunnerThread::executeTest1);
   mRunScriptQCall.bind(this->mLongThread, this, &ScriptRunnerThread::executeRunScript);

   // Initialize member variables.
   mSerialMsgThread = 0;
   mMsgMonkey = new RGB::MsgMonkey;
   mConnectionFlag = false;
}

ScriptRunnerThread::~ScriptRunnerThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void ScriptRunnerThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mSerialMsgThread)
   {
      mSerialMsgThread->showThreadInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child SerialMsgThread.

void ScriptRunnerThread::threadInitFunction()
{
   Trc::write(11, 0, "ScriptRunnerThread::threadInitFunction");

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialParms.mSerialPortSetup);
   tSerialSettings.mThreadPriority = Ris::Threads::gPriorities.mSerial;
   tSerialSettings.mRxTimeout = gSerialParms.mSerialRxTimeout;
   tSerialSettings.mMsgMonkey = mMsgMonkey;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxMsgQCall = mRxMsgQCall;
   tSerialSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create child thread with the settings.
   mSerialMsgThread = new Ris::SerialMsgThread(tSerialSettings);

   // Launch child thread.
   mSerialMsgThread->launchThread(); 

   Trc::write(11, 0, "ScriptRunnerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialMsgThread.

void ScriptRunnerThread::threadExitFunction()
{
   Trc::write(11, 0, "ScriptRunnerThread::threadExitFunction");
   Prn::print(0, "ScriptRunnerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialMsgThread->shutdownThread();

   Prn::print(0, "ScriptRunnerThread::threadExitFunction END");
   Trc::write(11, 0, "ScriptRunnerThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ScriptRunnerThread::shutdownThreads()
{
   Trc::write(11, 0, "ScriptRunnerThread::shutdownThread");
   Prn::print(0, "ScriptRunnerThread::shutdownThread BEGIN");

   // Abort the long thread.
   BaseClass::mNotify.abort();

   // Shutdown the two threads.
   BaseClass::shutdownThreads();

   Prn::print(0, "ScriptRunnerThread::shutdownThread END");
   Trc::write(11, 0, "ScriptRunnerThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ScriptRunnerThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort function. This is bound to the qcall. It aborts the serial port.

void ScriptRunnerThread::executeAbort()
{
   // Abort the long thread.
   BaseClass::mNotify.abort();
   // Abort the serial child thread.
   mSerialMsgThread->mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void ScriptRunnerThread::sendMsg(RGB::BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void ScriptRunnerThread::sendTestMsg()
{
   RGB::TestMsg* tMsg = new RGB::TestMsg;
   tMsg->mCode1 = 201;

   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace