
#include "stdafx.h"

#include "someSerialParms.h"
#include "rgbMsg.h"

#include "someResponderThread.h"
#include "someMonitorThread.h"

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

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("TP"))        Some::gResponderThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("SEND"))      executeSend(aCmd);
   if (aCmd->isCmd("A"))         executeAbort(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::special(int aSpecial)
{
   Some::gResponderThread->mShowCode = aSpecial;
   Some::gMonitorThread->mShowCode = aSpecial;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   RGB::TestMsg* tMsg = new RGB::TestMsg;
   Some::gResponderThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort (Ris::CmdLineCmd* aCmd)
{
   Some::gResponderThread->mAbortQCall();
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
   RGB::MsgMetrics tMsgMetrics;
   RGB::TestMsg* tMsg = new RGB::TestMsg;
   tMsgMetrics.update(tMsg, 100);
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

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Some::gSerialParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

