#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************


namespace PX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a control wrapper class that is used to run scripts via the
// script sequencer thread. 

class ScriptRunner
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Zip name variables.
   bool mUseZipNumber;
   int  mZipNumber;
   char mZipName[100];

   // Snip name variables.
   bool mUseSnipNumber;
   int  mSnipNumber;
   char mSnipName[100];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ScriptRunner();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Generate and run a zip file script. Store file parameters and call
   // the run script function below.
   bool doRunZipScript(int aZipNumber);
   bool doRunZipScript(const char* aZipName);

   // Suspend a running zip script.
   bool doSuspendZipScript();

   // Resume a suspended zip script.
   bool doResumeZipScript();

   // Abort a running or suspended zip or snippet script.
   bool doAbortZipScript();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Generate and run a snippet file script. Store file parameters and call
   // the run script function below.
   bool doRunSnipScript(int aSnipNumber);
   bool doRunSnipScript(const char* aSnipName);

   // Write new contents to the temp snippet file. Call the run snip script
   // method.
   bool doRunTempSnipScript(const char* aSnipFileContents);

   // Abort a running or suspended snippet or snippet script.
   bool doAbortSnipScript();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Generate and run a zip file script.
   bool doRunZipScript();

   // Generate and run a snippet file script.
   bool doRunSnipScript();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _PXSCRIPTRUNNER_CPP_
          ScriptRunner gScriptRunner;
#else
   extern ScriptRunner gScriptRunner;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
