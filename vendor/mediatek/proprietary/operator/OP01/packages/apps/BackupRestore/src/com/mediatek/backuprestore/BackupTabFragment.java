/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

package com.mediatek.backuprestore;

import android.app.Activity;
import android.app.ListFragment;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.mediatek.backuprestore.utils.Constants;
import com.mediatek.backuprestore.utils.MyLogger;

public class BackupTabFragment extends ListFragment {
    private static final String CLASS_TAG = MyLogger.LOG_TAG + "/BackupTabFragment";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(CLASS_TAG, "onCreate");
        if (savedInstanceState != null) {
            sPersonData = savedInstanceState.getBundle(Constants.PERSON_DATA);
            sAppData = savedInstanceState.getBundle(Constants.APP_DATA);
        }
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        Log.i(CLASS_TAG, "onAttach");
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        init();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(CLASS_TAG, "onDestroy");
    }

    public void onPause() {
        super.onPause();
        Log.i(CLASS_TAG, "onPasue");
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.i(CLASS_TAG, "onResume");
    }

    @Override
    public void onDetach() {
        super.onDetach();
        Log.i(CLASS_TAG, "onDetach");
    }

    private void init() {
        initAdapter();
    }

    private void initAdapter() {
        String[] content = new String[] { getString(R.string.backup_personal_data),
                getString(R.string.backup_app) };

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getActivity(),
                android.R.layout.simple_list_item_1, content);
        setListAdapter(adapter);
    }

    @Override
    public void onListItemClick(ListView l, View v, int position, long id) {
        String item = (String) l.getAdapter().getItem(position);
        Log.v(CLASS_TAG, "onLIstItemClick: item is " + item);
        if (item.equals(getString(R.string.backup_personal_data))) {
            // Personal data
            Intent intent = new Intent(this.getActivity(), PersonalDataBackupActivity.class);
            if (sPersonData != null) {
                intent.putExtras(sPersonData);
            }
            startActivity(intent);
        } else {
            // app
            Intent intent = new Intent(this.getActivity(), AppBackupActivity.class);
            if (sAppData != null) {
                intent.putExtras(sAppData);
            }
            startActivity(intent);
        }
    }

    private static Bundle sPersonData;
    private static Bundle sAppData;

    /*
     * @Override public void onActivityResult(int requestCode, int resultCode,
     * Intent data) { super.onActivityResult(requestCode, resultCode, data);
     * Log.i(CLASS_TAG, "onActivityResult requestCode = " + requestCode +
     * ", resultCode = " + resultCode + ", data = " + data); if (resultCode ==
     * Activity.RESULT_OK && data != null) { switch (requestCode) { case
     * Constants.RESULT_PERSON_DATA: mPersonData = data.getExtras();
     * Log.i(CLASS_TAG, "onActivityResult mPersonData = " + mPersonData); break;
     * case Constants.RESULT_APP_DATA: mAppData = data.getExtras();
     * Log.i(CLASS_TAG, "onActivityResult mAppData = " + mAppData); break;
     * default: break;
     *
     * } } else { Log.w(CLASS_TAG, "Intent data is null !!!"); }
     *
     * }
     */

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        Log.i(CLASS_TAG, "onSaveInstanceState mPersonData = " + sPersonData + ", mAppData = "
                + sAppData);
        if (sPersonData != null || sAppData != null) {
            outState.putBundle(Constants.PERSON_DATA, sPersonData);
            outState.putBundle(Constants.APP_DATA, sAppData);
        }
    }

    public static void setResultData(int requestCode, Intent data) {
        switch (requestCode) {
        case Constants.RESULT_PERSON_DATA:
            sPersonData = data.getExtras();
            Log.i(CLASS_TAG, "setResultData mPersonData = " + sPersonData);
            break;
        case Constants.RESULT_APP_DATA:
            sAppData = data.getExtras();
            Log.i(CLASS_TAG, "setResultData mAppData = " + sAppData);
            break;
        default:
            break;

        }
    }

}
