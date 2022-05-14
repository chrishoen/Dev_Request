/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someSerialParms.h"

#define  _SOMEREQUESTERTHREAD_CPP_
#include "someRequesterThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

RequesterThread::RequesterThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Requester");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &RequesterThread::executeSession);
   mRxStringQCall.bind   (this,&RequesterThread::executeRxString);
   mAbortQCall.bind(this, &RequesterThread::executeAbort);

   // Initialize member variables.
   mSerialStringThread = 0;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

RequesterThread::~RequesterThread()
{
   if (mSerialStringThread) delete mSerialStringThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void RequesterThread::showThreadInfo()
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

void RequesterThread::threadInitFunction()
{
   Trc::write(11, 0, "RequesterThread::threadInitFunction");

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

   Trc::write(11, 0, "RequesterThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialStringThread.

void RequesterThread::threadExitFunction()
{
   Trc::write(11, 0, "RequesterThread::threadExitFunction");
   Prn::print(0, "RequesterThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialStringThread->shutdownThread();

   Prn::print(0, "RequesterThread::threadExitFunction END");
   Trc::write(11, 0, "RequesterThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This calls the base class shutdownThread
// function to terminate the thread. This executes in the context of
// the calling thread.

void RequesterThread::shutdownThread()
{
   Trc::write(11, 0, "RequesterThread::shutdownThread");
   Prn::print(0, "RequesterThread::shutdownThread BEGIN");
   BaseClass::shutdownThread();
   Prn::print(0, "RequesterThread::shutdownThread END");
   Trc::write(11, 0, "RequesterThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void RequesterThread::executeOnTimer(int aTimerCount)
{
   if (mTPCode == 1)
   {
      std::string* tTxString = new std::string("RED");
      sendString(tTxString);
   }
   else if (mTPCode == 2)
   {
      std::string* tTxString = new std::string("GREEN");
      sendString(tTxString);
   }
   else if (mTPCode == 3)
   {
      std::string* tTxString = new std::string("BLUE");
      sendString(tTxString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort function. This is bound to the qcall. It aborts the serial port.

void RequesterThread::executeAbort()
{
   mSerialStringThread->mSerialStringPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked
// when a session is established or disestablished (when the serial port
// is opened or it is closed because of an error or a disconnection). 

void RequesterThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::Show1, "RequesterThread CONNECTED");
   }
   else
   {
      Prn::print(Prn::Show1, "RequesterThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialStringThread child thread. It is invoked by
// the child thread when a string is received.
// Based on the receive string type, call one of the specific receive
// string handlers. This is bound to the qcall.

void RequesterThread::executeRxString(std::string* aString)
{
   Prn::print(Prn::Show1, "RequesterThread::executeRxString %s", aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string via mSerialStringThread:

void RequesterThread::sendString(std::string* aString)
{
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace