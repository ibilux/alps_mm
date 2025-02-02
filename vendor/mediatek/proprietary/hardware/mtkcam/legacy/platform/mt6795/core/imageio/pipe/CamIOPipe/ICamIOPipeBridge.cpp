/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#define LOG_TAG "iio/camiob"
//
#include <utils/threads.h>
//
//#include <my_log.h>
//
#include <mtkcam/imageio/IPipe.h>
#include <mtkcam/imageio/ICamIOPipe.h>
#include <cutils/properties.h>  // For property_get().

//
#include "PipeImp.h"
#include "CamIOPipe.h"
//
#include "imageio_log.h"                    // Note: DBG_LOG_TAG/LEVEL will be used in header file, so header must be included after definition.
//
using namespace android;


/*******************************************************************************
*
********************************************************************************/
namespace NSImageio {
namespace NSIspio   {
////////////////////////////////////////////////////////////////////////////////

#undef   DBG_LOG_TAG                        // Decide a Log TAG for current file.
#ifndef USING_MTK_LDVT   // Not using LDVT.
#define  DBG_LOG_TAG        ""
#else
#define  DBG_LOG_TAG        LOG_TAG
#endif



DECLARE_DBG_LOG_VARIABLE(pipe);
//EXTERN_DBG_LOG_VARIABLE(pipe);



/*******************************************************************************
*
********************************************************************************/
class ICamIOPipeBridge : public ICamIOPipe
{
    friend  class   ICamIOPipe;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementation.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////
    mutable android::Mutex      mLock;
    android::Mutex&             getLockRef()    { return mLock; }
    MUINT32                     mu4InitRefCount;

protected:  ////    Implementor.
    CamIOPipe*const            mpPipeImp;
    inline  CamIOPipe const*   getImp() const  { return mpPipeImp; }
    inline  CamIOPipe*         getImp()        { return mpPipeImp; }

protected:  ////    Constructor/Destructor.
                    ICamIOPipeBridge(CamIOPipe*const pCamIOPipe);
                    ~ICamIOPipeBridge();

private:    ////    Disallowed.
                    ICamIOPipeBridge(ICamIOPipeBridge const& obj);
    ICamIOPipeBridge&  operator=(ICamIOPipeBridge const& obj);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////    Instantiation.
    virtual MVOID   destroyInstance();
    virtual MBOOL   init();
    virtual MBOOL   uninit();

public:     ////    Attributes.
    virtual char const* getPipeName() const;
    virtual EPipeID     getPipeId() const;
    virtual MINT32      getLastErrorCode() const;

public:     ////    Callbacks.
    virtual MVOID   setCallbacks(PipeNotifyCallback_t notify_cb, PipeDataCallback_t data_cb, MVOID* user);
    //
    //  notify callback
    virtual MBOOL   isNotifyMsgEnabled(MINT32 const i4MsgTypes) const;
    virtual MVOID   enableNotifyMsg(MINT32 const i4MsgTypes);
    virtual MVOID   disableNotifyMsg(MINT32 const i4MsgTypes);
    //
    //  data callback
    virtual MBOOL   isDataMsgEnabled(MINT32 const i4MsgTypes) const;
    virtual MVOID   enableDataMsg(MINT32 const i4MsgTypes);
    virtual MVOID   disableDataMsg(MINT32 const i4MsgTypes);

public:     ////    Operations.
    virtual MBOOL   start();
    virtual MBOOL   startCQ0();
    virtual MBOOL   startCQ0B();
    virtual MBOOL   startCQ0_D();
    virtual MBOOL   startCQ0B_D();
    virtual MBOOL   stop();

public:     ////    Buffer Quening.
    virtual MBOOL   enqueInBuf(PortID const portID, QBufInfo const& rQBufInfo,MINT32 const enqueCq,MINT32 const dupCqIdx);
    virtual MBOOL   dequeInBuf(PortID const portID, QTimeStampBufInfo& rQBufInfo,EDrvScenario eDrvSce, MINT32 const dequeCq,MINT32 const dequeBurstQIdx, MINT32 const dequeDupCqIdx, MUINT32 const u4TimeoutMs /*= 0xFFFFFFFF*/);
    //
    virtual MBOOL   enqueOutBuf(PortID const portID, QBufInfo const& rQBufInfo,MINT32 const enqueCq,MINT32 const dupCqIdx);
    virtual MBOOL   dequeOutBuf(PortID const portID, QTimeStampBufInfo& rQBufInfo,EDrvScenario eDrvSce, MINT32 const dequeCq,MINT32 const dequeBurstQIdx, MINT32 const dequeDupCqIdx, MUINT32 const u4TimeoutMs /*= 0xFFFFFFFF*/);

public:     ////    Settings.
    virtual MBOOL   configPipe(vector<PortInfo const*>const& vInPorts, vector<PortInfo const*>const& vOutPorts,\
                                    MINT32 const burstQIdx=0,MINT32 const dupCqIdx=0,MVOID* const pCookie=NULL,vector<ModuleParaInfo>const& vModuleParams=vector<ModuleParaInfo>());
    virtual MBOOL   configPipeUpdate(vector<PortInfo const*>const& vInPorts, vector<PortInfo const*>const& vOutPorts);
public:
    ////    Interrupt handling
    virtual MBOOL   irq(EPipePass pass, EPipeIRQ irq_int, int userEnum); //old interface
    virtual MBOOL   registerIrq(Irq_t irq);
    virtual MBOOL   irq(Irq_t irq);
    virtual MUINT32* getIspReg(MUINT32 path);
    ////    original style sendCommand method
    virtual MBOOL   sendCommand(MINT32 cmd, MINTPTR arg1, MINTPTR arg2, MINTPTR arg3);
    ////    get packed raw out DMA info
};


/*******************************************************************************
*
********************************************************************************/
ICamIOPipe*
ICamIOPipe::
createInstance(EDrvScenario const eScenarioID, EScenarioFmt const eScenarioFmt, MINT8 const szUsrName[32])
{
    CamIOPipe* pPipeImp = new CamIOPipe("CamIOPipe", ICamIOPipe::ePipeID, eScenarioID, eScenarioFmt,szUsrName);
    if  ( ! pPipeImp )
    {
        PIPE_ERR("[ICamIOPipe] fail to new CamIOPipe");
        return  NULL;
    }
    //
    ICamIOPipeBridge*  pIPipe = new ICamIOPipeBridge(pPipeImp);
    if  ( ! pIPipe )
    {
        PIPE_ERR("[ICamIOPipe] fail to new ICamIOPipeBridge");
        delete  pPipeImp;
        return  NULL;
    }
    DBG_LOG_CONFIG(imageio, pipe);
    //
    return  pIPipe;
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
destroyInstance()
{
    delete  mpPipeImp;  //  Firstly, delete the implementor here instead of destructor.
    delete  this;       //  Finally, delete myself.
}


/*******************************************************************************
*
********************************************************************************/
ICamIOPipeBridge::
ICamIOPipeBridge(CamIOPipe*const pCamIOPipe)
    : ICamIOPipe()
    , mLock()
    , mu4InitRefCount(0)
    , mpPipeImp(pCamIOPipe)
{
}


/*******************************************************************************
*
********************************************************************************/
ICamIOPipeBridge::
~ICamIOPipeBridge()
{
    PIPE_DBG(":E");
    PIPE_DBG(":X");
}


/*******************************************************************************
*
********************************************************************************/
char const*
ICamIOPipeBridge::
getPipeName() const
{
    return  getImp()->getPipeName();
}


/*******************************************************************************
*
********************************************************************************/
EPipeID
ICamIOPipeBridge::
getPipeId() const
{
    return  getImp()->getPipeId();
}


/*******************************************************************************
*
********************************************************************************/
MINT32
ICamIOPipeBridge::
getLastErrorCode() const
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->getLastErrorCode();
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
init()
{
    MBOOL   ret = MTRUE;
    Mutex::Autolock _lock(mLock);

    if  ( 0 != mu4InitRefCount )
    {
        mu4InitRefCount++;
    }
    else if ( (ret = getImp()->init()) )
    {
        mu4InitRefCount = 1;
    }
    PIPE_DBG("- mu4InitRefCount(%d), ret(%d)", mu4InitRefCount, ret);
    return  ret;
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
uninit()
{
    MBOOL   ret = MTRUE;
    Mutex::Autolock _lock(mLock);

    if  ( 0 < mu4InitRefCount )
    {
        mu4InitRefCount--;
        if  ( 0 == mu4InitRefCount )
        {
            ret = getImp()->uninit();
        }
    }
    PIPE_DBG("- mu4InitRefCount(%d), ret(%d)", mu4InitRefCount, ret);
    return  ret;
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
setCallbacks(PipeNotifyCallback_t notify_cb, PipeDataCallback_t data_cb, MVOID* user)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->setCallbacks(notify_cb, data_cb, user);
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
isNotifyMsgEnabled(MINT32 const i4MsgTypes) const
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->isNotifyMsgEnabled(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
enableNotifyMsg(MINT32 const i4MsgTypes)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->enableNotifyMsg(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
disableNotifyMsg(MINT32 const i4MsgTypes)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->disableNotifyMsg(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
isDataMsgEnabled(MINT32 const i4MsgTypes) const
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->isDataMsgEnabled(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
enableDataMsg(MINT32 const i4MsgTypes)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->enableDataMsg(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MVOID
ICamIOPipeBridge::
disableDataMsg(MINT32 const i4MsgTypes)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->disableDataMsg(i4MsgTypes);
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
start()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->start();
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
startCQ0()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->startCQ0();
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
startCQ0B()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->startCQ0B();
}
/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
startCQ0_D()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->startCQ0_D();
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
startCQ0B_D()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->startCQ0B_D();
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
stop()
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->stop();
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
configPipe(vector<PortInfo const*>const& vInPorts, vector<PortInfo const*>const& vOutPorts,\
                                    MINT32 const burstQIdx,MINT32 const dupCqIdx,MVOID* const pCookie,vector<ModuleParaInfo>const& vModuleParams)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->configPipe(vInPorts, vOutPorts);
}
/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
configPipeUpdate(vector<PortInfo const*>const& vInPorts, vector<PortInfo const*>const& vOutPorts)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->configPipeUpdate(vInPorts, vOutPorts);
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
irq(EPipePass pass, EPipeIRQ irq_int, int userEnum)
{
    return  getImp()->irq(pass, irq_int,userEnum);
}

MBOOL
ICamIOPipeBridge::
registerIrq(Irq_t irq)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->registerIrq(irq);
}

MBOOL
ICamIOPipeBridge::
irq(Irq_t irq)
{
    //remove, no reason to use lock at waitirq
    //Mutex::Autolock _lock(mLock);
    return  getImp()->irq(irq);
}

MUINT32*
ICamIOPipeBridge::getIspReg(MUINT32 path)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->getIspReg(path);
}


/*******************************************************************************
* sendCommand
********************************************************************************/
MBOOL
ICamIOPipeBridge::
sendCommand(MINT32 cmd, MINTPTR arg1, MINTPTR arg2, MINTPTR arg3)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->sendCommand(cmd, arg1, arg2, arg3);
}
/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
enqueInBuf(PortID const portID, QBufInfo const& rQBufInfo,MINT32 const enqueCq,MINT32 const dupCqIdx)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->enqueInBuf(portID, rQBufInfo);
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
dequeInBuf(PortID const portID, QTimeStampBufInfo& rQBufInfo,EDrvScenario eDrvSce, MINT32 const dequeCq,MINT32 const dequeBurstQIdx, MINT32 const dequeDupCqIdx, MUINT32 const u4TimeoutMs /*= 0xFFFFFFFF*/)
{
    Mutex::Autolock _lock(mLock);
    return  getImp()->dequeInBuf(portID, rQBufInfo, u4TimeoutMs);
}

/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
enqueOutBuf(PortID const portID, QBufInfo const& rQBufInfo,MINT32 const enqueCq,MINT32 const dupCqIdx)
{
    MBOOL bImdMode = MFALSE;
    //borrow enqueCq for ImdMode.
    if(enqueCq != 0)
        bImdMode = MTRUE;
    //Mutex::Autolock _lock(mLock);
    return  getImp()->enqueOutBuf(portID, rQBufInfo,bImdMode);
}


/*******************************************************************************
*
********************************************************************************/
MBOOL
ICamIOPipeBridge::
dequeOutBuf(PortID const portID, QTimeStampBufInfo& rQBufInfo,EDrvScenario eDrvSce, MINT32 const dequeCq,MINT32 const dequeBurstQIdx, MINT32 const dequeDupCqIdx, MUINT32 const u4TimeoutMs /*= 0xFFFFFFFF*/)
{
    //Mutex::Autolock _lock(mLock);
    return  getImp()->dequeOutBuf(portID, rQBufInfo, u4TimeoutMs);
}


/*******************************************************************************
* ICmd
********************************************************************************/
ICamIOPipe::
ICmd::
ICmd(IPipe*const pIPipe)
    : mpIPipe(reinterpret_cast<ICamIOPipe*>(pIPipe))
{
}


MBOOL
ICamIOPipe::
ICmd::
verifySelf()
{
    ICamIOPipeBridge*const pBridge = reinterpret_cast<ICamIOPipeBridge*>(mpIPipe);
    //
    if  ( NULL == mpIPipe )
    {
        PIPE_ERR("[ICamIOPipe::ICmd::verifySelf] NULL mpIPipe");
        return  MFALSE;
    }
    //
    if  ( pBridge->getPipeId() != ICamIOPipe::ePipeID )
    {
        PIPE_ERR("[ICamIOPipe::ICmd::verifySelf] ID(%d) != ICamIOPipe::ePipeID(%d)", pBridge->getPipeId(), ICamIOPipe::ePipeID);
        return  MFALSE;
    }

    return  MTRUE;
}


/*******************************************************************************
* Cmd_Set2Params
* Command: Set 2 parameters.
********************************************************************************/
ICamIOPipe::
Cmd_Set2Params::
Cmd_Set2Params(IPipe*const pIPipe, MUINT32 const u4Param1, MUINT32 const u4Param2)
    : ICmd(pIPipe)
    , mu4Param1(u4Param1)
    , mu4Param2(u4Param2)
{
}

MBOOL
ICamIOPipe::
Cmd_Set2Params::
execute()
{
    if  ( verifySelf() )
    {
        ICamIOPipeBridge*const pBridge = reinterpret_cast<ICamIOPipeBridge*>(mpIPipe);
        //
        Mutex::Autolock _lock(pBridge->getLockRef());
        //
        CamIOPipe*const pPipe = pBridge->getImp();
        return  pPipe->onSet2Params(mu4Param1, mu4Param2);
    }
    return  MFALSE;
}


/*******************************************************************************
* Cmd_Get1ParamBasedOn1Input
* Command: Get 1 parameter based on 1 input parameter.
********************************************************************************/
ICamIOPipe::
Cmd_Get1ParamBasedOn1Input::
Cmd_Get1ParamBasedOn1Input(IPipe*const pIPipe, MUINT32 const u4InParam, MUINT32*const pu4OutParam)
    : ICmd(pIPipe)
    , mu4InParam(u4InParam)
    , mpu4OutParam(pu4OutParam)
{
}


MBOOL
ICamIOPipe::
Cmd_Get1ParamBasedOn1Input::
execute()
{
    if  ( verifySelf() )
    {
        ICamIOPipeBridge*const pBridge = reinterpret_cast<ICamIOPipeBridge*>(mpIPipe);
        //
        Mutex::Autolock _lock(pBridge->getLockRef());
        //
        CamIOPipe*const pPipe = pBridge->getImp();
        return  pPipe->onGet1ParamBasedOn1Input(mu4InParam, mpu4OutParam);
    }
    return  MFALSE;
}
////////////////////////////////////////////////////////////////////////////////
};  //namespace NSIspio
};  //namespace NSImageio

