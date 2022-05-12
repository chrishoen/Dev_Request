/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "pxPrinterParms.h"
#include "fcomGCodeCommThread.h"
#include "lcdGraphicsThread.h"
#include "logLogFileThread.h"

#include "pxScriptSeqThread.h"

namespace PX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptSeqThread::executeTest1()
{
   // Guard.
   if (!FCom::gGCodeCommThread) return;

   try
   {
      Prn::print(Prn::View01, "ScriptSeqThread::executeTest1 BEGIN");

      // Set the thread notification mask.
      mNotify.setMaskOne("GCodeAck", cGCodeAckNotifyCode);

      // Send a gcode command to the feynman. It will send a completion 
      // notification.
      FCom::gGCodeCommThread->sendString("M105");

      // Wait for the completion notification.
      mNotify.wait(cGCodeAckTimeout);

      Prn::print(Prn::View01, "ScriptSeqThread::executeTest1 END");
   }
   catch (int aException)
   {
      Prn::print(0, "EXCEPTION ScriptSeqThread::executeTest1 %d %s", aException, mNotify.mException);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ScriptSeqThread::executeTest2(int aCode)
{
   // Guard.
   if (!Lcd::gGraphicsThread) return;

   try
   {
      Prn::print(Prn::View01, "ScriptSeqThread::executeTest2 BEGIN");

      // Set thread notification mask.
      mNotify.setMaskOne("Lcd", cLcdNotifyCode);

      // Send a png file load request to the lcd graphics thread. It will send
      // a completion notification.
      char* tFilePath;
      if (aCode == 0) tFilePath = "/opt/alpha/image/Image3840_0.png";
      else            tFilePath = "/opt/alpha/image/Image3840_1.png";
      Lcd::gGraphicsThread->postDraw1(tFilePath,&mLcdNotify);

      // Wait for the completion notification.
      mNotify.wait(cLcdTimeout);

      Prn::print(Prn::View01, "ScriptSeqThread::executeTest2 END");
   }
   catch (int aException)
   {
      Prn::print(0, "EXCEPTION ScriptSeqThread::executeTest2 %d %s", aException, mNotify.mException);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace