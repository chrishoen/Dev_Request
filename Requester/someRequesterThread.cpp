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
   BaseClass::setThreadName("Proc");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mProc);
   BaseClass::mTimerPeriod = 100;

   // Initialize qcalls.
   mSessionQCall.bind(this, &RequesterThread::executeSession);
   mRxMsgQCall.bind   (this,&RequesterThread::executeRxMsg);
   mAbortQCall.bind(this, &RequesterThread::executeAbort);

   // Initialize member variables.
   mSerialMsgThread = 0;
   mMsgMonkey = new ProtoComm::MsgMonkey;
   mConnectionFlag = false;
   mTPCode = 0;
   mRxCount = 0;
   mTxCount = 0;
   mShowCode = 0;
}

RequesterThread::~RequesterThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
   delete mMsgMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread info for this thread and for child threads.

void RequesterThread::showThreadInfo()
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

void RequesterThread::threadInitFunction()
{
   Trc::write(11, 0, "RequesterThread::threadInitFunction");

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

   Trc::write(11, 0, "RequesterThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immedidately
// before the thread is terminated. It shuts down the child SerialMsgThread.

void RequesterThread::threadExitFunction()
{
   Trc::write(11, 0, "RequesterThread::threadExitFunction");
   Prn::print(0, "RequesterThread::threadExitFunction BEGIN");

   // Shutdown the child thread.
   mSerialMsgThread->shutdownThread();

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
      ProtoComm::TestMsg* tTxMsg = new ProtoComm::TestMsg;
      tTxMsg->mCode1 = aTimerCount;
      sendMsg(tTxMsg);
   }
   else if (mTPCode == 2)
   {
      ProtoComm::EchoRequestMsg* tTxMsg = new ProtoComm::EchoRequestMsg;
      tTxMsg->initialize(1000);
      sendMsg(tTxMsg);
   }
   else if (mTPCode == 3)
   {
      ProtoComm::ByteBlobMsg* tTxMsg = new ProtoComm::ByteBlobMsg;
      tTxMsg->initialize();
      sendMsg(tTxMsg);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort function. This is bound to the qcall. It aborts the serial port.

void RequesterThread::executeAbort()
{
   mSerialMsgThread->mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// qcall registered to the mSerialMsgThread child thread. It is invoked
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
// qcall registered to the mSerialMsgThread child thread. It is invoked by
// the child thread when a message is received.
// Based on the receive message type, call one of the specific receive
// message handlers. This is bound to the qcall.

void RequesterThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Call corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
   case ProtoComm::MsgIdT::cTestMsg:
      processRxMsg((ProtoComm::TestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoRequestMsg:
      processRxMsg((ProtoComm::EchoRequestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoResponseMsg:
      processRxMsg((ProtoComm::EchoResponseMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cDataMsg:
      processRxMsg((ProtoComm::DataMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cByteBlobMsg:
      processRxMsg((ProtoComm::ByteBlobMsg*)tMsg);
      break;
   default:
      Prn::print(Prn::Show1, "RequesterThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
   mRxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler - TestMsg.

void RequesterThread::processRxMsg(ProtoComm::TestMsg*  aRxMsg)
{
   if (mShowCode == 3)
   {
      aRxMsg->show(Prn::Show1);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void RequesterThread::processRxMsg(ProtoComm::EchoRequestMsg* aRxMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tTxMsg = new ProtoComm::EchoResponseMsg;
      tTxMsg->initialize(1000);
      tTxMsg->mCode1 = aRxMsg->mCode1;
      mSerialMsgThread->sendMsg(tTxMsg);
   }
   if (mShowCode == 3)
   {
      aRxMsg->show(Prn::Show1);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void RequesterThread::processRxMsg(ProtoComm::EchoResponseMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      aRxMsg->show(Prn::Show1);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void RequesterThread::processRxMsg(ProtoComm::DataMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      aRxMsg->show(Prn::Show1);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - ByteBlobMsg.

void RequesterThread::processRxMsg(ProtoComm::ByteBlobMsg* aRxMsg)
{
   if (mShowCode == 3)
   {
      aRxMsg->show(Prn::Show1);
   }
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void RequesterThread::sendMsg(ProtoComm::BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
   mTxCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via mSerialMsgThread:

void RequesterThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
   tMsg->mCode1 = 201;

   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace