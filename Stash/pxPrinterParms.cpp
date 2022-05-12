//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _PXPRINTPARMS_CPP_
#include "pxPrinterParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace PX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PrinterParms::PrinterParms()
{
   reset();
}

void PrinterParms::reset()
{
   BaseClass::reset();
   BaseClass::setFileName_RelAlphaFiles("/Print/Print_Parms.txt");

   mShowCode = 0;
   mSplashCode = 0;
   mSliceCode = 0;
   mScriptThrottle = 0;

   mModelEnable = false;
   mModelAdjustZ = 0.0;

   mGCodeCommEnable = false;
   mLcdGraphicsEnable = false;
   mControlEnable = false;
   mIsochEnable = false;
   
   mPrintViewEnable = false;
   mPrintViewIPAddress[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void PrinterParms::show()
{
   char tBuffer[40];
   printf("\n");
   printf("PrinterParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("ShowCode                   %-10d\n", mShowCode);
   printf("SplashCode                 %-10d\n", mSplashCode);
   printf("SliceCode                  %-10d\n", mSliceCode);
   printf("ScriptThrottle             %-10d\n", mScriptThrottle);
   printf("\n");
   printf("ModelEnable                %-10s\n", my_string_from_bool(mModelEnable));
   printf("ModelAdjustZ               %-10.3f\n", mModelAdjustZ);
   printf("\n");
   printf("GCodeCommEnable            %-10s\n", my_string_from_bool(mGCodeCommEnable));
   printf("LcdGraphicsEnable          %-10s\n", my_string_from_bool(mLcdGraphicsEnable));
   printf("ControlEnable              %-10s\n", my_string_from_bool(mControlEnable));
   printf("IsochEnable                %-10s\n", my_string_from_bool(mIsochEnable));

   printf("\n");
   printf("PrintViewEnable            %-10s\n", my_string_from_bool(mPrintViewEnable));
   printf("PrintViewIPAddress         %-10s\n", mPrintViewIPAddress);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void PrinterParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("ShowCode"))                 mShowCode = aCmd->argInt(1);
   if (aCmd->isCmd("SplashCode"))               mSplashCode = aCmd->argInt(1);
   if (aCmd->isCmd("SliceCode"))                mSliceCode = aCmd->argInt(1);
   if (aCmd->isCmd("ScriptThrottle"))           mScriptThrottle = aCmd->argInt(1);

   if (aCmd->isCmd("ModelEnable"))              mModelEnable = aCmd->argBool(1);
   if (aCmd->isCmd("ModelAdjustZ"))             mModelAdjustZ = aCmd->argDouble(1);

   if (aCmd->isCmd("GCodeCommEnable"))          mGCodeCommEnable = aCmd->argBool(1);
   if (aCmd->isCmd("LcdGraphicsEnable"))        mLcdGraphicsEnable = aCmd->argBool(1);
   if (aCmd->isCmd("ControlEnable"))            mControlEnable = aCmd->argBool(1);
   if (aCmd->isCmd("IsochEnable"))              mIsochEnable = aCmd->argBool(1);

   if (aCmd->isCmd("PrintViewIPAddress"))       aCmd->copyArgString(1, mPrintViewIPAddress, cMaxStringSize);
   if (aCmd->isCmd("PrintViewEnable"))          mPrintViewEnable = aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void PrinterParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace