/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "rgbMsg.h"


namespace RGB
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg()
{
   mMessageType = MsgIdT::cTestMsg;
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
} 

void TestMsg::show(int aPF)
{
   Prn::print(aPF, "TestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "Code2      %d", mCode2);
   Prn::print(aPF, "Code3      %d", mCode3);
   Prn::print(aPF, "Code4      %d", mCode4);
   Prn::print(aPF, "");
}

void TestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( &mCode1 );
   aBuffer->copy( &mCode2 );
   aBuffer->copy( &mCode3 );
   aBuffer->copy( &mCode4 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

FirstMessageMsg::FirstMessageMsg()
{
   mMessageType = MsgIdT::cFirstMessageMsg;
   mCode1 = 0;
} 

void FirstMessageMsg::show(int aPF)
{
   Prn::print(aPF, "FirstMessageMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void FirstMessageMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

RedRequestMsg::RedRequestMsg()
{
   mMessageType = MsgIdT::cRedRequestMsg;

   mCode1 = 101;
} 

void RedRequestMsg::show(int aPF)
{
   Prn::print(aPF, "RedRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void RedRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1    );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

RedResponseMsg::RedResponseMsg()
{
   mMessageType = MsgIdT::cRedResponseMsg;

   mCode1 = 201;
}

void RedResponseMsg::show(int aPF)
{
   Prn::print(aPF, "RedRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void RedResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

GreenRequestMsg::GreenRequestMsg()
{
   mMessageType = MsgIdT::cGreenRequestMsg;

   mCode1 = 101;
}

void GreenRequestMsg::show(int aPF)
{
   Prn::print(aPF, "GreenRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void GreenRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

GreenResponseMsg::GreenResponseMsg()
{
   mMessageType = MsgIdT::cGreenResponseMsg;

   mCode1 = 201;
}

void GreenResponseMsg::show(int aPF)
{
   Prn::print(aPF, "GreenRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void GreenResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BlueRequestMsg::BlueRequestMsg()
{
   mMessageType = MsgIdT::cBlueRequestMsg;

   mCode1 = 101;
}

void BlueRequestMsg::show(int aPF)
{
   Prn::print(aPF, "BlueRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void BlueRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BlueResponseMsg::BlueResponseMsg()
{
   mMessageType = MsgIdT::cBlueResponseMsg;

   mCode1 = 201;
}

void BlueResponseMsg::show(int aPF)
{
   Prn::print(aPF, "BlueRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void BlueResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message, based on a message type.

void* createMsg(int aMessageType)
{
   Ris::ByteContent* tMsg = 0;

   switch (aMessageType)
   {
   case MsgIdT::cTestMsg:
      tMsg = new TestMsg;
      break;
   case MsgIdT::cFirstMessageMsg:
      tMsg = new FirstMessageMsg;
      break;
   case MsgIdT::cRedRequestMsg:
      tMsg = new RedRequestMsg;
      break;
   case MsgIdT::cRedResponseMsg:
      tMsg = new RedResponseMsg;
      break;
   case MsgIdT::cGreenRequestMsg:
      tMsg = new GreenRequestMsg;
      break;
   case MsgIdT::cGreenResponseMsg:
      tMsg = new GreenResponseMsg;
      break;
   case MsgIdT::cBlueRequestMsg:
      tMsg = new BlueRequestMsg;
      break;
   case MsgIdT::cBlueResponseMsg:
      tMsg = new BlueResponseMsg;
      break;
   default:
      return 0;
      break;
   }
   return tMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


