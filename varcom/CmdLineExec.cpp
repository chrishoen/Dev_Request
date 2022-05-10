#include "stdafx.h"

#include "someSerialParms.h"
#include "someVarcomSRThread.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("REQ"))      Some::gVarcomSRThread->mRxReqNumBytes = aCmd->argInt(1);
   if (aCmd->isCmd("SEND"))     executeSend(aCmd);
   if (aCmd->isCmd("ABORT"))    executeAbort(aCmd);
   if (aCmd->isCmd("START"))    executeStart(aCmd);
   if (aCmd->isCmd("HM"))       executeHome(aCmd);
   if (aCmd->isCmd("UP"))       executeUp(aCmd);
   if (aCmd->isCmd("DN"))       executeDown(aCmd);
   if (aCmd->isCmd("SP"))       executeStopped(aCmd);

   if (aCmd->isCmd("GO1"))      executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))      executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))      executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))      executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))      executeGo5(aCmd);
   if (aCmd->isCmd("PARMS"))    executeParms(aCmd);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend(Ris::CmdLineCmd* aCmd)
{
   char tString[100];
   if (aCmd->numArg() == 0)
   {
      strcpy(tString, "ADDR");
   }
   else
   {
      sprintf(tString, "%s", aCmd->argWhole());
   }
   Some::gVarcomSRThread->sendString(tString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   Some::gVarcomSRThread->abort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeStart(Ris::CmdLineCmd* aCmd)
{
   int tTimeout = 200;
   Some::gVarcomSRThread->sendString("\\3");
   Ris::Threads::threadSleep(tTimeout);
   Some::gVarcomSRThread->sendString("en");
   Ris::Threads::threadSleep(tTimeout);
   Some::gVarcomSRThread->sendString("echo 1");
   Ris::Threads::threadSleep(tTimeout);
   Some::gVarcomSRThread->sendString("homecmd");
   Ris::Threads::threadSleep(tTimeout);
   Some::gVarcomSRThread->sendString("ready");
   Ris::Threads::threadSleep(tTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeHome(Ris::CmdLineCmd* aCmd)
{
   Some::gVarcomSRThread->sendString("homecmd");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeUp(Ris::CmdLineCmd* aCmd)
{
   int tTimeout = 200;
   int tCount = 4;

   Some::gVarcomSRThread->sendString("moveinc 50000 20");

   for (int i = 0; i < tCount; i++)
   {
      Ris::Threads::threadSleep(tTimeout);
      Some::gVarcomSRThread->sendString("stopped");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeDown(Ris::CmdLineCmd* aCmd)
{
   int tTimeout = 200;
   int tCount = 4;

   Some::gVarcomSRThread->sendString("moveinc -50000 20");

   for (int i = 0; i < tCount; i++)
   {
      Ris::Threads::threadSleep(tTimeout);
      Some::gVarcomSRThread->sendString("stopped");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeStopped(Ris::CmdLineCmd* aCmd)
{
   Some::gVarcomSRThread->sendString("stopped");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Some::gSerialParms.reset();
   Some::gSerialParms.readSection("default");
   Some::gSerialParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

