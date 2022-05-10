/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "risSleep.h"
#include "prnPrint.h"

#include "risThreadsPriorities.h"
#include "risSerialStringSR.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialStringSR::SerialStringSR()
{
   // Set member variables.
   mConnectionFlag = false;
   mAbortFlag = false;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

SerialStringSR::~SerialStringSR()
{
}

void SerialStringSR::initialize(SerialSettings& aSettings)
{
   mSerialStringPort.initialize(aSettings);
}

void SerialStringSR::finalize()
{
   // Close the serial port.
   mSerialStringPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialStringSR::doRestart()
{
   // Top of the loop.
   mRestartCount = 0;
   mConnectionFlag = false;
   mAbortFlag = false;

restart:
   // Guard.
   if (mAbortFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      sleepMs(1000);
   }
   Prn::print(Prn::Show1, "SerialStringSR restart %d", mRestartCount);
   mRestartCount++;

   // Test if a session is established.
   if (mConnectionFlag)
   {
      // Connection was disestablished.
      mConnectionFlag = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // Open the serial port.
   if (!mSerialStringPort.doOpen())
   {
      // If error then restart.
      goto restart;
   }

   // Connection was established.
   mConnectionFlag = true;
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. Append terminator
// bytes as specified in the settings. Receive a given number of strings
// from the serial port and store them in the string buffers. The receive
// string terminator bytes are also specified in the settings.

bool SerialStringSR::doSendReceive(const char* aTxString, int aNumReceive)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Send a request string.

   mSerialStringPort.doSendString(aTxString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive response strings.

   mRxCount = 0;
   for (int tIndex = 0; tIndex < cMaxReceive; tIndex++)
   {
      Prn::print(Prn::Show4, "SerialStringSR read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Receive. 
      mRetCode = mSerialStringPort.doReceiveString(mRxString[tIndex], cMaxStringSize);

      // Check for terminate.
      if (mAbortFlag)
      {
         Prn::print(Prn::Show1, "SerialStringSR read ABORT");
         return false;
      }

      // Check the return code.
      if (mRetCode == 0)
      {
         Prn::print(Prn::Show1, "SerialStringSR read EMPTY");
         return false;
      }
      else if (mRetCode == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "SerialStringSR read ERROR");
         return false;
      }
      else if (mRetCode == Ris::cSerialRetTimeout)
      {
         Prn::print(Prn::Show1, "SerialStringSR read TIMEOUT");
         return false;
      }
      else if (mRetCode == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "SerialStringSR read ABORT");
         return false;
      }
      else if (mRetCode == Ris::cSerialRetDataError)
      {
         Prn::print(Prn::Show1, "SerialStringSR read DATA ERROR");
         return false;
      }
      // Process the read.
      else
      {
      }
   }

   // Done.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialStringSR::doAbort()
{
   printf("SerialStringSR::abort BEGIN\n");

   // Abort pending serial port receives
   mSerialStringPort.doAbort();

   printf("SerialStringSR::abort END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. Append terminator
// bytes as specified in the settings.

void SerialStringSR::sendString(const char* aString)
{
   mSerialStringPort.doSendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

