
#include "stdafx.h"

#include "someSerialParms.h"
#include "procoMsg.h"

#include "someRequesterThread.h"
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
   if (aCmd->isCmd("TP"))        Some::gRequesterThread->mTPCode = aCmd->argInt(1);
   if (aCmd->isCmd("SEND"))      executeSend(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho(aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("ABORT"))     executeAbort(aCmd);
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
   Some::gRequesterThread->mShowCode = aSpecial;
   Some::gMonitorThread->mShowCode = aSpecial;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
         Some::gRequesterThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         tMsg->initialize();
         Some::gRequesterThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   int tNumWords = aCmd->argInt(1);
   
   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
   tMsg->initialize(tNumWords);
   Some::gRequesterThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   tMsg->initialize();
   Some::gRequesterThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort (Ris::CmdLineCmd* aCmd)
{
   Some::gRequesterThread->mAbortQCall();
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
   ProtoComm::MsgMetrics tMsgMetrics;
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
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

