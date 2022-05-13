/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someState.h"

#include "someScriptRunnerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeRunScript()
{
   // Print and log.
   Prn::print(0, "");
   Prn::print(0, "running script2");
   Prn::print(Prn::Show1, "ScriptSeqThread::executeRunScript2 BEGIN");

   // Initialize variables.
   mLoopExitCode = 0;
   gState.setStateSX2_Running();
   gState.initializeTime2();

   // Open the script file.
   mReadCount = 0;
   if (!mScript.doOpen("files/script_rgb.txt")) throw 991;

   // Set the state and send a message to the controller.
   gState.setStateSX2_Running();

   try
   {
      // Loop through the script file.
      while (true)
      {
         // Read a command from the script file. Exit if end of file.
         if (mScript.doRead(&mCmd))
         {
            if (mCmd.isCmd("EXIT")) break;
         }
         mReadCount++;

         // Test for a notification exception.
         mNotify.testException();

         // Delay.
         mNotify.waitForTimer(cScriptThrottle);

         // Execute the script file command.
         execute(&mCmd);
      }
   }
   catch (int aException)
   {
      mLoopExitCode = cLoopExitAborted;
      Prn::print(0, "EXCEPTION ScriptSeqThread::executeRunScript2 %d %s", aException, mNotify.mException);
   }

   // Test the exit code.
   if (mLoopExitCode == cLoopExitNormal)
   {
      // Print and log.
      Prn::print(0, "script done");
      Prn::print(0, "");

      // Set the state and send a message to the controller.
      gState.setStateSX2_Done();

      // Set the state and send a message to the controller.
      gState.setStateSX2_None();
   }
   else if (mLoopExitCode == cLoopExitAborted)
   {
      // Print and log.
      Prn::print(0, "script aborted");
      Prn::print(0, "");

      // Set the state and send a message to the controller.
      gState.setStateSX2_Aborted();
   }

   // Close the script file.
   mScript.doClose();

   Prn::print(Prn::Show1, "ScriptSeqThread::executeRunScript2 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command. It calls one of
// the following specific command execution functions.

void ScriptRunnerThread::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RED"))    executeRed(aCmd);
   if (aCmd->isCmd("GREEN"))  executeGreen(aCmd);
   if (aCmd->isCmd("BLUE"))   executeBlue(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptRunnerThread::executeRed(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "RED     %d", tN);
}

void ScriptRunnerThread::executeGreen(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "GREEN   %d", tN);
}

void ScriptRunnerThread::executeBlue(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 111);
   int tN = aCmd->argInt(1);
   Prn::print(0, "BLUE    %d", tN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace