
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someScriptRunnerThread.h"
#include "someMonitorThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   Some::gScriptRunnerThread = new Some::ScriptRunnerThread;
   Some::gScriptRunnerThread->launchThreads();

   Some::gMonitorThread = new Some::MonitorThread;
   Some::gMonitorThread->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   Some::gScriptRunnerThread->showThreadInfo();
   Some::gMonitorThread->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute console command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   Some::gMonitorThread->shutdownThread();
   delete Some::gMonitorThread;
   Some::gMonitorThread = 0;

   Some::gScriptRunnerThread->shutdownThreads();
   delete Some::gScriptRunnerThread;
   Some::gScriptRunnerThread = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
