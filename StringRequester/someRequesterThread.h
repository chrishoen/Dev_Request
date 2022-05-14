#pragma once

/*==============================================================================
Serial string prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risSerialStringThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial string prototype thread class. It processes strings that are
// communicated via a serial channel. The strings follow the byte content
// binary string scheme.
//
// This is a prototype serial thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The prototype thread creates a child serial string thread that establishes
// and manages a serial connection, receives strings and passes them to the
// parent via a qcall callback, and allows for the transmission of strings.
// the child thread also notifies the parent thread of serial connection
// establishment/disestablishment via a qcall callback.
// 
// The prototype thread is based on a call queue and it uses qcalls to
// interface to the child thread. When the child thread detects a session
// change it invokes the prototypes thread's mSessionQCall, which defers
// execution of its executeSession member function. Likewise, when the child
// thread receives a string it invokes the serial thread's mRxStringQCall, which
// defers  execution of its executeRxString member function. 
//
// The child thread provides the execution context for actually managing
// session changes and receiving strings. The parent thread provides the
// execution context for processing the session changes and the received 
// strings.
//

class  RequesterThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial string thread, this manages serial string connections and
   // string transmission and reception.
   Ris::SerialStringThread*  mSerialStringThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // True if the serial connection is valid.
   bool mConnectionFlag;

   // State variables.
   int mTPCode;
   int mRxCount;
   int mTxCount;
   int mShowCode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   RequesterThread();
  ~RequesterThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the 
   // child SerialStringThread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child SerialStringThread.
   void threadExitFunction() override;

   // Thread shutdown function. This calls the base class shutdownThread
   // function to terminate the thread. This executes in the context of
   // the calling thread.
   void shutdownThread() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;

   // Show thread info for this thread and for child threads.
   void showThreadInfo() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // qcall registered to the mSerialStringThread child thread. It is invoked
   // when a session is established or disestablished (when the serial port
   // is opened or it is closed because of an error or a disconnection). 
   Ris::SerialStringThread::SessionQCall mSessionQCall;

   // Maintain session state variables. This is bound to the qcall.
   void executeSession(bool aConnected);

   // qcall registered to the mSerialStringThread child thread. It is invoked by
   // the child thread when a string is received.
   Ris::SerialStringThread::RxStringQCall mRxStringQCall;

   // Based on the receive string type, call one of the specific receive
   // string handlers. This is bound to the qcall.
   void executeRxString(std::string* aString);

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
   // Methods.

   // Send a string via the child thread.
   void sendString(std::string* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _SOMEREQUESTERTHREAD_CPP_
         RequesterThread* gRequesterThread;
#else
extern   RequesterThread* gRequesterThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
