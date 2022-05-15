#include "stdafx.h"

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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Return a string from a host ordered address.

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   // abc 9
   std::string* tInput = new std::string(aCmd->argWhole());
   Prn::print(0, "input  %s", tInput->c_str());

   int tRet = 0;
   int tValue = 0;
   tRet = sscanf(tInput->c_str(), "abc %d", &tValue);

   Prn::print(0, "tRet    %d", tRet);
   Prn::print(0, "tValue  %d", tValue);
   Prn::print(0, "");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   // abpqc
   // abpqrsc
   std::string* tInput = new std::string(aCmd->argWhole());
   Prn::print(0, "input  %s", tInput->c_str());

   int tRet = 0;
   char tString1[40];
   int tInt1 = 0;
   tString1[0] = 0;
   tRet = sscanf(tInput->c_str(), "ab%2scd%d", tString1, &tInt1);

   Prn::print(0, "tRet      %d", tRet);
   Prn::print(0, "tString1  %s", tString1);
   Prn::print(0, "tInt1     %d", tInt1);
   Prn::print(0, "");
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

