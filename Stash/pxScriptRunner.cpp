/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnShare.h"
#include "pxPrinterParms.h"
#include "cxFileManager.h"
#include "cxScriptWriter.h"
#include "cxScriptReader.h"
#include "cxScriptTester.h"
#include "ccomControlThread.h"
#include "pxScriptSeqThread.h"
#include "logLogFileThread.h"

#define  _PXSCRIPTRUNNER_CPP_
#include "pxScriptRunner.h"

namespace PX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ScriptRunner::ScriptRunner()
{
   mUseZipNumber = false;
   mZipNumber = 0;
   mZipName[0] = 0;
   mUseSnipNumber = false;
   mSnipNumber = 0;
   mSnipName[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Generate and run a zip file script. Store file parameters and call
// the run script function below.

bool ScriptRunner::doRunZipScript(int aZipNumber)
{
   mUseZipNumber = true;
   mZipNumber = aZipNumber;
   return doRunZipScript();
}

bool ScriptRunner::doRunZipScript(const char* aZipName)
{
   mUseZipNumber = false;
   strcpy(mZipName,aZipName);
   return doRunZipScript();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Generate and run a snippet file script. Store file parameters and call
// the run script function below.

bool ScriptRunner::doRunSnipScript(int aSnipNumber)
{
   mUseSnipNumber = true;
   mSnipNumber = aSnipNumber;
   return doRunSnipScript();
}

bool ScriptRunner::doRunSnipScript(const char* aSnipName)
{
   mUseSnipNumber = false;
   strcpy(mSnipName, aSnipName);
   return doRunSnipScript();
}

bool ScriptRunner::doRunTempSnipScript(const char* aSnipFileContents)
{
   CX::gFileManager.doWriteTempSnip(aSnipFileContents);
   return doRunSnipScript("zzzztemp");
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Generate and run a zip file script.

bool ScriptRunner::doRunZipScript()
{
   // Initialize.
   bool tPass = false;
   Cmn::gShare.mGCodeTxTotal1 = 0;
   Cmn::gShare.mSliceTotal = 0;

   // Guard.
   if (Cmn::gShare.isStateSX1_Busy())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "zip already busy");
      CCom::gControlThread->sendRunZipNakMsg("zip already busy");
      return false; 
   }

   // Guard.
   if (Cmn::gShare.isStateSX1_Suspended())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "zip already suspended");
      CCom::gControlThread->sendRunZipNakMsg("zip already suspended");
      return false;
   }

   // Guard.
   if (Cmn::gShare.isStateSX2_Busy())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "snippet already busy");
      CCom::gControlThread->sendRunZipNakMsg("snippet already busy");
      return false;
   }

   // Load the zip.
   if (mUseZipNumber)
   {
      tPass = CX::gFileManager.doLoadZip(mZipNumber);
   }
   else
   {
      tPass = CX::gFileManager.doLoadZip(mZipName);
   }
   if (!tPass)
   {
      // If fail then send a message to the controller.
      CCom::gControlThread->sendRunZipNakMsg(CX::gFileManager.mError.c_str());
      return false;
   }

   // Find the snippet file name.
   tPass = CX::gFileManager.doFindWorkGCodeFileName();
   if (!tPass)
   {
      // If fail then send a message to the controller.
      CCom::gControlThread->sendRunZipNakMsg(CX::gFileManager.mError.c_str());
      return false;
   }

   // Write the script.
   CX::ScriptWriter tScriptWriter;
   tPass = tScriptWriter.doWrite1(
      CX::gFileManager.mWorkGCodeFilePath,
      CX::gFileManager.mWorkSliceFilePrefixPath,
      CX::gFileManager.mWorkDirPath,
      CX::gFileManager.mWorkScriptFilePath1);
   if (!tPass)
   {
      // If fail then send a message to the controller.
      CCom::gControlThread->sendRunZipNakMsg("ERROR SCRIPT INVALID1");
      return false;
   }

   // Test the script.
   CX::ScriptTester tScriptTester;
   tPass = tScriptTester.doTestScriptFile(CX::gFileManager.mWorkScriptFilePath1);
   if (!tPass)
   {
      // If fail then send a message to the controller.
      CCom::gControlThread->sendRunZipNakMsg("ERROR SCRIPT INVALID2");
      return false;
   }

   // Update the metadata.
   CX::gFileManager.doUpdateRunZipHistoryWithPrintTime();

   // Show the test results.
   tScriptTester.show();

   // Save some tester counts.
   Cmn::gShare.mGCodeTxTotal1 = tScriptTester.mCountSend;
   Cmn::gShare.mSliceTotal = tScriptTester.mCountSlice;

   // Set the state and send a message to the controller.
   Cmn::gShare.setStateSX1_Starting();
   CCom::gControlThread->sendRunZipAckMsg();

   // Run the script.
   PX::gScriptSeqThread->mRunScript1QCall(false);

   // Done.
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Generate and run a snippet file script.

bool ScriptRunner::doRunSnipScript()
{
   // Initialize.
   bool tPass = false;
   Cmn::gShare.mGCodeTxTotal2 = 0;

   // Guard.
   if (Cmn::gShare.isStateSX1_Busy())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "zip already busy");
      CCom::gControlThread->sendRunSnipNakMsg("zip already busy");
      return false;
   }

   // Guard.
   if (Cmn::gShare.isStateSX2_Busy())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "snippet already busy");
      CCom::gControlThread->sendRunSnipNakMsg("snippet already busy");
      return false;
   }

   // Load the zip.
   if (mUseSnipNumber)
   {
      tPass = CX::gFileManager.doLoadSnip(mSnipNumber);
   }
   else
   {
      tPass = CX::gFileManager.doLoadSnip(mSnipName);
   }
   if (!tPass)
   {
      // If fail then send a message to the controller.
      CCom::gControlThread->sendRunSnipNakMsg(CX::gFileManager.mError.c_str());
      return false;
   }

   // Write the script.
   CX::ScriptWriter tScriptWriter;
   tPass = tScriptWriter.doWrite2(
      CX::gFileManager.mRunSnipFilePath,
      CX::gFileManager.mWorkScriptFilePath2);
   if (!tPass)
   {
      // If fail then send a message to the controller.
      Prn::print(0, "ERROR SCRIPT INVALID1");
      CCom::gControlThread->sendRunSnipNakMsg("ERROR SCRIPT INVALID1");
      return false;
   }

   // Test the script.
   CX::ScriptTester tScriptTester;
   tPass = tScriptTester.doTestScriptFile(CX::gFileManager.mWorkScriptFilePath2);
   if (!tPass)
   {
      // If fail then send a message to the controller.
      Prn::print(0, "ERROR SCRIPT INVALID2");
      CCom::gControlThread->sendRunSnipNakMsg("ERROR SCRIPT INVALID2");
      return false;
   }

   // Show the test results.
   if (false)
   {
      tScriptTester.show();
   }

   // Save some tester counts.
   Cmn::gShare.mGCodeTxTotal2 = tScriptTester.mCountSend;

   // Send a message to the controller.
   CCom::gControlThread->sendRunSnipAckMsg();

   // Run the script.
   PX::gScriptSeqThread->mRunScript2QCall();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suspend a running zip script.

bool ScriptRunner::doSuspendZipScript()
{
   // Guard.
   if (!Cmn::gShare.isStateSX1_Running())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "zip not running");
      CCom::gControlThread->sendRunZipNakMsg("zip not running");
      return false;
   }

   // Set the state and send a message to the controller.
   Cmn::gShare.setStateSX1_Suspending();
   CCom::gControlThread->sendRunZipAckMsg();

   // Suspend the script.
   PX::gScriptSeqThread->suspendScript();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a suspended zip script.

bool ScriptRunner::doResumeZipScript()
{
   // Guard.
   if (!Cmn::gShare.isStateSX1_Suspended())
   {
      // If fail then send nak message.
      Prn::print(0, "zip not suspended");
      CCom::gControlThread->sendRunZipNakMsg("zip not suspended");
      return false;
   }

   // Set the state and send a message to the controller.
   Cmn::gShare.setStateSX1_Starting();
   CCom::gControlThread->sendRunZipAckMsg();

   // Resume the script.
   PX::gScriptSeqThread->mRunScript1QCall(true);

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort a running or suspended zip script.

bool ScriptRunner::doAbortZipScript()
{
   // Guard.
   if (!Cmn::gShare.isStateSX1_Busy() && !Cmn::gShare.isStateSX1_Suspended())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "zip not busy");
      CCom::gControlThread->sendRunZipNakMsg("zip not busy");
      return false;
   }

   // Guard.
   if (Cmn::gShare.isStateSX1_Suspended())
   {
      // Write to log file.
      Log::TString* tString2 = new Log::TString("SCRIPT1 ABORTED");
      tString2->sendToLogFile();

      // Set the state and send a message to the controller.
      Cmn::gShare.setStateSX1_Aborted();
      CCom::gControlThread->sendRunZipStateMsg();

      // Show.
      Prn::print(0, "script1 aborted");
      Prn::print(0, "");
      return false;
   }

   // Set the state and send a message to the controller.
   Cmn::gShare.setStateSX1_Aborting();
   CCom::gControlThread->sendRunZipStateMsg();

   // Abort the script.
   PX::gScriptSeqThread->mAbortScriptQCall();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort a running or suspended snippet script.

bool ScriptRunner::doAbortSnipScript()
{
   // Guard.
   if (!Cmn::gShare.isStateSX2_Busy())
   {
      // If fail then send a message to the controller.
      Prn::print(0, "snippet not busy");
      CCom::gControlThread->sendRunSnipNakMsg("snippet not busy");
      return false;
   }

   // Set the state and send a message to the controller.
   Cmn::gShare.setStateSX2_Aborting();
   CCom::gControlThread->sendRunSnipStateMsg();

   // Abort the script.
   PX::gScriptSeqThread->mAbortScriptQCall();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace