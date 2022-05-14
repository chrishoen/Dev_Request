/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someSerialParms.h"

#define  _SOMERESPONDERTHREAD_CPP_
#include "someResponderThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ResponderThread::ResponderThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Responder");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ResponderThread::executeSession);
   mRxStringQCall.bind   (this,&ResponderThread::executeRxString);
   mAbortQCall.bind(this, &ResponderThread::executeAbort);

   // Initialize member variables.
   mSerialStringThread = 0;
   mConnectionFlag = false;
   mTPFlag = true;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

ResponderThread::~ResponderThread()
{
   if (mSerialStringThread) delete mSerialStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void ResponderThread::showThreadInfo()
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

void ResponderThread::threadInitFunction()
{
   Trc::write(11, 0, "ResponderThread::threadInitFunction");

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

   Trc::write(11, 0, "ResponderThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialStringThread.

void ResponderThread::threadExitFunction()
{
   Trc::write(11, 0, "ResponderThread::threadExitFunction");
   Prn::print(0, "ResponderThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialStringThread->shutdownThread();

   Prn::print(0, "ResponderThread::threadExitFunction END");
   Trc::write(11, 0, "ResponderThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void ResponderThread::shutdownThread()
{
   Trc::write(11, 0, "ResponderThread::shutdownThread");
   Prn::print(0, "ResponderThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "ResponderThread::shutdownThread END");
   Trc::write(11, 0, "ResponderThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void ResponderThread::executeOnTimer(int aTimerCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort function. This is bound to the qcall. It aborts the serial port.

void ResponderThread::executeAbort()
{
   mSerialStringThread->mSerialStringPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked
// when a session is established or disestablished (when the serial port
// is opened or it is closed because of an error or a disconnection). 

void ResponderThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "ResponderThread CONNECTED");
   }
   else
   {
      Prn::print(Prn::Show1, "ResponderThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked by
// the child thread when a string is received.

void ResponderThread::executeRxString(std::string* aString)
{
   Prn::print(Prn::Show1, "RequesterThread::executeRxString %s", aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via the child thread.

void ResponderThread::sendString(std::string* aString)
{
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace