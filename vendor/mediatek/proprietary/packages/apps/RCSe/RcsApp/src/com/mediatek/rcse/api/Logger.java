/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2011. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
package com.mediatek.rcse.api;

import android.app.Activity;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;


//This is a common logger for MTK RCS-e solution.
/**
 * The Class Logger.
 */
public final class Logger {
    /**
     * The Constant TAG.
     */
    public static final String TAG = "[RCSe]";

    /**
     * Instantiates a new logger.
     */
    private Logger() {
    }; // Static common use class should not be instantiated.

    /**
     * The Constant XLOG_ENABLED.
     */
    private static final boolean XLOG_ENABLED = true;
    /**
     * The s is log enabled.
     */
    private static boolean sIsLogEnabled = true;
    // This is used for test codes
    /**
     * The Constant IS_DEBUG.
     */
    public static final boolean IS_DEBUG = true;
    /**
     * The Constant RCS_PREFS_IS_INTEGRATION_MODE.
     */
    public static final String RCS_PREFS_IS_INTEGRATION_MODE = "IS_INTEGRATION_MODE";
    // This is used for launch mode or integration mode
    /**
     * The s is integration mode.
     */
    private static boolean sIsIntegrationMode;

    /**
     * Get sIsIntegrationMode.
     *
     * @return sIsIntegrationMode
     */
    public static boolean getIsIntegrationMode() {
        return true;
        // return sIsIntegrationMode;
    }
    /**
     * Set sIsIntegrationMode.
     *
     * @param isIntegrationMode the new checks if is integration mode
     */
    public static void setIsIntegrationMode(boolean isIntegrationMode) {
        sIsIntegrationMode = isIntegrationMode;
    }
    /**
     * initialize sIsIntegrationMode.
     *
     * @param context the context
     */
    public static void initialize(Context context) {
        if (context != null) {
            v(TAG, "initialize(), context is not null");
            SharedPreferences preferences = context
                    .getSharedPreferences(
                            RCS_PREFS_IS_INTEGRATION_MODE,
                            Activity.MODE_PRIVATE);
            if (preferences.contains(RCS_PREFS_IS_INTEGRATION_MODE)) {
                sIsIntegrationMode = preferences.getBoolean(
                        RCS_PREFS_IS_INTEGRATION_MODE, false);
            } else {
                sIsIntegrationMode = false;
                Editor editor = preferences.edit();
                editor.putBoolean(RCS_PREFS_IS_INTEGRATION_MODE,
                        false);
                editor.commit();
            }
        } else {
            sIsIntegrationMode = false;
        }
        v(TAG, "initialize(), sIsIntegrationMode = "
                + sIsIntegrationMode);
    }
    /**
     * Run-time set whether need to enable the RCS-e log in this process.
     *
     * @param isLogEnable
     *            Enable/disable the RCS-e log.
     */
    public static void setLogEnabled(boolean isLogEnable) {
        sIsLogEnabled = isLogEnable;
        sIsLogEnabled = true;
    }
    /**
     * Send a verbose log message.
     *
     * @param tag
     *            Normally it's the class name who call this method.
     * @param message
     *            The message you want to send.
     */
    public static void v(String tag, String message) {
        if (true) {
            if (XLOG_ENABLED) {
                Log.v(TAG, getCombinedMessage(tag, message));
            } else {
                Log.v(TAG, getCombinedMessage(tag, message));
            }
        }
    }
    /**
     * Send a debug log message.
     *
     * @param tag
     *            Normally it's the class name who call this method.
     * @param message
     *            The message you want to send.
     */
    public static void d(String tag, String message) {
        if (true) {
            if (XLOG_ENABLED) {
                Log.d(TAG, getCombinedMessage(tag, message));
            } else {
                Log.d(TAG, getCombinedMessage(tag, message));
            }
        }
    }
    /**
     * Send an information log message.
     *
     * @param tag
     *            Normally it's the class name who call this method.
     * @param message
     *            The message you want to send.
     */
    public static void i(String tag, String message) {
        if (true) {
            if (XLOG_ENABLED) {
                Log.i(TAG, getCombinedMessage(tag, message));
            } else {
                Log.i(TAG, getCombinedMessage(tag, message));
            }
        }
    }
    /**
     * Send a warning log message.
     *
     * @param tag
     *            Normally it's the class name who call this method.
     * @param message
     *            The message you want to send.
     */
    public static void w(String tag, String message) {
        if (true) {
            if (XLOG_ENABLED) {
                Log.w(TAG, getCombinedMessage(tag, message));
            } else {
                Log.w(TAG, getCombinedMessage(tag, message));
            }
        }
    }
    /**
     * Send a error log message.
     *
     * @param tag
     *            Normally it's the class name who call this method.
     * @param message
     *            The message you want to send.
     */
    public static void e(String tag, String message) {
        if (true) {
            if (XLOG_ENABLED) {
                Log.e(TAG, getCombinedMessage(tag, message));
            } else {
                Log.e(TAG, getCombinedMessage(tag, message));
            }
        }
    }
    /**
     * Gets the combined message.
     *
     * @param tag the tag
     * @param message the message
     * @return the combined message
     */
    private static String getCombinedMessage(String tag,
            String message) {
        if (null != tag) {
            StringBuilder builder = new StringBuilder();
            builder.append("[");
            builder.append(tag);
            builder.append("]: ");
            builder.append(message);
            return builder.toString();
        } else {
            return message;
        }
    }
    
    /**
     * Is logger activated
     * 
     * @return boolean
     */
    public static boolean isActivated() {
        return true;
    }
}
