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
#ifndef _AAA_TYPES_H_
#define _AAA_TYPES_H_
/******************************************************************************
 *
 ******************************************************************************/
#include <stdint.h>


/******************************************************************************
 *
 ******************************************************************************/
//
typedef signed char             MINT8;
typedef unsigned char           MUINT8;
//
typedef signed short            MINT16;
typedef unsigned short          MUINT16;
//
typedef signed int              MINT32;
typedef unsigned int            MUINT32;
//
typedef int64_t                 MINT64;
typedef uint64_t                MUINT64;
//
typedef signed int              MINT;
typedef unsigned int            MUINT;
//
typedef intptr_t                MINTPTR;
typedef uintptr_t               MUINTPTR;

/******************************************************************************
 *
 ******************************************************************************/
//
typedef void                    MVOID;
typedef float                   MFLOAT;
typedef double                  MDOUBLE;
//
typedef int                     MBOOL;
//
#ifndef MTRUE
    #define MTRUE               1
#endif
#ifndef MFALSE
    #define MFALSE              0
#endif
#ifndef MNULL
    #define MNULL               0
#endif
/******************************************************************************
 *
 ******************************************************************************/
#endif // _AAA_TYPES_H_

