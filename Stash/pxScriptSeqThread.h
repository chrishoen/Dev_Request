#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTwoThread.h"
#include "risThreadsSynch.h"
#include "cxScriptReader.h"

namespace PX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class ScriptSeqThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Lcd paint settle time.
   static const int cLcdSettleTime = 50;

   // Wait timeouts.
   static const int cGCodeAckTimeout = -1;
   static const int cLcdTimeout = 2000;

   // Notification codes.
   static const int cGCodeAckNotifyCode = 11;
   static const int cLcdNotifyCode = 12;

   static const int cFlushGCodeAckNotifyCode = 17;
   static const int cFlushLcdNotifyCode = 18;

   // Loop exit status codes.
   static const int cLoopExitNormal    = 0;
   static const int cLoopExitSuspended = 1;
   static const int cLoopExitAborted   = 2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notifications.
   Ris::Threads::NotifyWrapper mLcdNotify;
   Ris::Threads::NotifyWrapper mGCodeAckNotify;

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

   CX::ScriptReader mReader;

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
   ScriptSeqThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   void threadExitFunction() override;

   // Thread shutdown function. This shuts down the two threads.
   void shutdownThreads() override;

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

   // Test qcall. It is invoked by the command line executive.
   Ris::Threads::QCall1<int> mTest2QCall;

   // Test function. This is bound to the qcall.
   void executeTest2(int aCode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Run script qcall. It is invoked by the command line executive.
   Ris::Threads::QCall1<bool> mRunScript1QCall;
   Ris::Threads::QCall0       mRunScript2QCall;

   // Run script function. This is bound to the qcall. This runs the last 
   // script1 file that was generated by the file manager. It is passed a 
   // resume script flag. If the flag is false then the script is new and
   // the state should be reset and the script file reader should start at
   // the beginning of the file. if the flag is true then the script is being
   // resumed from a script suspend and the state should be restored and
   // the script reader should start where it left off when the suspend
   // ocurred. True means run the script file. False means resume the script
   // file.
   // 
   // This is used for running or resuming zip script files.
   void executeRunScript1(bool aResumeFlag);

   // Run script function. This is bound to the qcall. This runs the last 
   // script2 file that was generated by the file manager.
   // 
   // This is used for running gcode script files.
   void executeRunScript2();

   // Execute script commands.
   void doRunCmd_Send(int aCode);
   void doRunCmd_Slice();
   void doRunCmd_PWM();
   void doRunCmd_Test();
   void doRunCmd_CMarker();
   void doRunCmd_Info();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods: QCalls: These are used to send commands to the thread.

   // Abort a running grid or script qcall.

   // The qcall. This is a call that is queued to this thread.
   Ris::Threads::QCall0 mAbortScriptQCall;

   // Execute the call in the context of this thread.
   void executeAbortScript();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the suspend request flag true. If the RunScript1QCall is executing
   // then, when it encounters the next test script command, it saves state
   // and exits in a suspended state. The script is resumed by running
   // another RunScript1QCall.
   void suspendScript();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _PXSCRIPTSEQTHREAD_CPP_
          ScriptSeqThread* gScriptSeqThread = 0;
#else
   extern ScriptSeqThread* gScriptSeqThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
