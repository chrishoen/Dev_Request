#pragma once

/*==============================================================================
Script runner prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTwoThread.h"
#include "risThreadsSynch.h"
#include "risSerialMsgThread.h"
#include "risCmdLineScript.h"
#include "risSRSWPointerQueue.h"

#include "rgbMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Script runner prototype thread class.
// 
// This is a script runner two thread. It's purpose is to run a script text
// file that contains instructions to send request messages to a responder
// program that sends back response messages. For each command instruction,
// a request message is formulated and send to the responder. It then
// receives and processes the response messages.
// 
// It inherits from BaseTwoThread to obtain a two thread functionality.
//
// The long thread executes a run script qcall that provides the execution
// context for the request/response message processing sequence. It reads
// a command instruction from the script file and executes it to send
// a request and wait for a notication of the response.
//
// This creates a child serial message thread that establishes and manages
// a serial connection, receives messages and passes them to the parent via
// a qcall callback, and allows for the transmission of messages. The child
// thread also notifies the parent thread of serial connection
// establishment/disestablishment via a qcall callback.

// The short thread executes a receive message qcall that is invoked by the
// child serial message thread. It notifies the long thread
// when responses are received.
//

class  ScriptRunnerThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Wait timeouts.
   static const int cRxMsgTimeout = -1;
   static const int cScriptThrottle = 100;

   // Notification codes.
   static const int cRxMsgNotifyCode = 11;

   static const int cFlushRxMsgNotifyCode = 17;

   // Loop exit status codes.
   static const int cLoopExitNormal = 0;
   static const int cLoopExitSuspended = 1;
   static const int cLoopExitAborted = 2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial message thread, this manages serial message connections and
   // message transmission and reception.
   Ris::SerialMsgThread*  mSerialMsgThread;

   // Message monkey used by mSerialMsgThread.
   RGB::MsgMonkey* mMsgMonkey;

   // True if the serial connection is valid.
   bool mConnectionFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notifications.
   Ris::Threads::NotifyWrapper mRxMsgNotify;

   // Receive message queue. This is written with a pointer to a message
   // when it is received by the receive message qcall. It is read by the
   // run script qcall.
   Ris::SRSWPointerQueue<RGB::BaseMsg*, 10> mRxMsgQueue;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Run script exit code.
   int mLoopExitCode;

   // If true then at the next test point script command that is encountered
   // in the script file the suspend exit flag is set.
   bool mSuspendRequestFlag;

   // If true then the script qcall loop exits in a suspended state.
   bool mSuspendExitFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Command line script file reader.
   Ris::CmdLineScript mScript;

   // Command line command for script reader.
   Ris::CmdLineCmd mCmd;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true the execute periodically.
   bool mTPFlag;

   // Metrics.
   int  mStatusCount1;
   int  mStatusCount2;

   // Metrics.
   int mReadCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ScriptRunnerThread();
  ~ScriptRunnerThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the 
   // child SerialMsgThread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child SerialMsgThread.
   void threadExitFunction() override;

   // Thread shutdown function. This calls the base class shutdownThread
   // function to terminate the thread. This executes in the context of
   // the calling thread.
   void shutdownThreads() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;

   // Show thread info for this thread and for child threads.
   void showThreadInfo() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // qcall registered to the mSerialMsgThread child thread. It is invoked
   // when a session is established or disestablished (when the serial port
   // is opened or it is closed because of an error or a disconnection). 
   Ris::SerialMsgThread::SessionQCall mSessionQCall;

   // This is bound to the qcall. Maintain session state variables. 
   void executeSession(bool aConnected);

   // qcall registered to the mSerialMsgThread child thread. It is invoked by
   // the serial child thread when a message is received.
   Ris::SerialMsgThread::RxMsgQCall mRxMsgQCall;

   // This is bound to the qcall. Write the received message to the message
   // queue and notify the long thread, which will then process the message.
   void executeRxMsg(Ris::ByteContent* aRxMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Abort qcall. It is invoked by the control thread to execute a
   // an abort in the context of this thread.
   Ris::Threads::QCall0 mAbortQCall;

   // Abort function. This is bound to the qcall. It aborts the serial port.
   void executeAbort();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Test qcall. It is invoked by the command line executive.
   Ris::Threads::QCall0 mTest1QCall;

   // Test function. This is bound to the qcall.
   void executeTest1();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Run script qcall. It is invoked by the command line executive.
   Ris::Threads::QCall0       mRunScriptQCall;

   // Run script function. This is bound to the qcall. This runs a
   // script file.
   // 
   // This is used for running script files.
   void executeRunScript();

   // Execute a command line command from the script file. It calls one of
   // the following specific command execution functions.
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute specific commands.
   void executeRed(Ris::CmdLineCmd* aCmd);
   void executeGreen(Ris::CmdLineCmd* aCmd);
   void executeBlue(Ris::CmdLineCmd* aCmd);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via mSerialMsgThread:
   void sendMsg (RGB::BaseMsg* aTxMsg);
   void sendTestMsg();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _SOMESCRIPTRUNNERTHREAD_CPP_
         ScriptRunnerThread* gScriptRunnerThread = 0;
#else
extern   ScriptRunnerThread* gScriptRunnerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
