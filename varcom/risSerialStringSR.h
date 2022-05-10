#pragma once

/*==============================================================================
Serial string port sender receiver.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSerialStringPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial string sender receiver.
//
// This class contains a serial string port. It provides a capability to
// send a request string and then receive a fixed number of response strings. 

class SerialStringSR
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 2000;
   static const int cMaxReceive = 4;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial string port.
   SerialStringPort mSerialStringPort;

   // Rx string.
   char mRxString[cMaxReceive][cMaxStringSize];

   // If true then the serial port is open. If false then it is closed
   // because of an error.
   bool mConnectionFlag;

   // If true then abort any pending restart or receive operations.
   bool mAbortFlag;

   // Return code from a send receive call.
   int mRetCode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialStringSR();
   ~SerialStringSR();
   virtual void initialize(SerialSettings& aSettings);
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Enter a loop that initializes and opens the serial port until
   // it is successful.
   void doRestart();

   // Abort any pending restart or read operations. This can be called by
   // any thread.
   void doAbort();

   // Send a null terminated string via the serial port. Append terminator
   // bytes as specified in the settings. Receive a given number of strings
   // from the serial port and store them in the string buffers. The receive
   // string terminator bytes are also specified in the settings.
   bool doSendReceive(const char* aTxString, int aNumReceive);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the serial port. Append terminator
   // bytes as specified in the settings.
   void sendString(const char* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

