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
   : mRxStringNotify(&mNotify, cRxStringNotifyCode)
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mShortThread->setThreadName("ScriptShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mScriptShort);

   BaseClass::mLongThread->setThreadName("ScriptLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mScriptLong);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer = 
      std::bind(&ScriptRunnerThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer = 
      std::bind(&ScriptRunnerThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = 
      std::bind(&ScriptRunnerThread::executeOnTimer, this, _1);

   // Bind qcalls.
   mSessionQCall.bind      (this->mShortThread, this, &ScriptRunnerThread::executeSession);
   mRxStringQCall.bind        (this->mShortThread, this, &ScriptRunnerThread::executeRxString);
   mAbortScriptQCall.bind  (this->mShortThread, this, &ScriptRunnerThread::executeAbortScript);

   // Bind qcalls.
   mTest1QCall.bind        (this->mLongThread, this, &ScriptRunnerThread::executeTest1);
   mRunScriptQCall.bind    (this->mLongThread, this, &ScriptRunnerThread::executeRunScript);

   // Initialize member variables.
   mSerialStringThread = 0;
   mConnectionFlag = false;
}

ScriptRunnerThread::~ScriptRunnerThread()
{
   if (mSerialStringThread) delete mSerialStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void ScriptRunnerThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mSerialStringThread)
   {
      mSerialStringThread->showThreadInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immedidately 
// after the thread starts running. It creates and launches the 
// child SerialStringThread.

void ScriptRunnerThread::threadInitFunction()
{
   Trc::write(11, 0, "ScriptRunnerThread::threadInitFunction");

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialParms.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialParms.mSerialPortSetup);
   tSerialSettings.mThreadPriority = Ris::Threads::gPriorities.mSerial;
   tSerialSettings.mRxTimeout = gSerialParms.mSerialRxTimeout;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxStringQCall = mRxStringQCall;
   tSerialSettings.mTraceIndex = 11;
   Trc::start(11);

   // Create child thread with the settings.
   mSerialStringThread = new Ris::SerialStringThread(tSerialSettings);

   // Launch child thread.
   mSerialStringThread->launchThread(); 

   Trc::write(11, 0, "ScriptRunnerThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialStringThread.

void ScriptRunnerThread::threadExitFunction()
{
   Trc::write(11, 0, "ScriptRunnerThread::threadExitFunction");
   Prn::print(0, "ScriptRunnerThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialStringThread->shutdownThread();

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
// Flush the receive string queue and print an error if it was not empty.

void ScriptRunnerThread::flushRxStringQueue()
{
   int tNumFlush = mRxStringQueue.flushRead();
   if (tNumFlush)
   {
      Prn::print(0, "mRxStringQueue.flushRead() NOT EMPTY %d", tNumFlush);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is bound to the qcall. It notifies the long thread to abort
// any running script.

void ScriptRunnerThread::executeAbortScript()
{
   // Notify the the long thread to abort any running script.
   BaseClass::mNotify.abort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via the child thread.

void ScriptRunnerThread::sendString(std::string* aString)
{
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace