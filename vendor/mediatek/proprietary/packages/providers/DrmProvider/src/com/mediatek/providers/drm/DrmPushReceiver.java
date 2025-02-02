/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.mediatek.providers.drm;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.drm.DrmRights;
import android.os.AsyncTask;
import android.util.Log;

import com.mediatek.drm.OmaDrmClient;
import com.mediatek.drm.OmaDrmUtils;

public class DrmPushReceiver extends BroadcastReceiver {
    private static final String TAG = "DRM/DrmPushReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceive " + intent);
        // only when OMA DRM enabled, we need install right
        if (OmaDrmClient.isOmaDrmEnabled()) {
            new SaveRightTask(context).execute(intent);
        }
    }

    private static class SaveRightTask extends AsyncTask<Intent, Void, Boolean> {
        private Context mContext;
        public SaveRightTask(Context context) {
            mContext = context;
        }

        @Override
        protected Boolean doInBackground(Intent... intents) {
            Boolean success = null;
            Intent intent = intents[0];
            String rightMimeType = intent.getType();
            if (!OmaDrmUtils.isDrmRightsFile(rightMimeType, null)) {
                Log.w(TAG, "SaveRightTask with not drm mimetype");
                return null;
            }
            byte[] rightData = intent.getByteArrayExtra("data");
            if (rightData == null) {
                Log.w(TAG, "SaveRightTask with null right data");
                return null;
            }
            File tmpFile = null; // the temporary file for rightData
            FileOutputStream fos = null;
            OmaDrmClient client = new OmaDrmClient(mContext);
            try {
                // 1. Write right data to a temp file
                tmpFile = File.createTempFile("rights", "tmp");
                fos = new FileOutputStream(tmpFile);
                fos.write(rightData);
                fos.flush();
                // 2. Save right with temp file
                DrmRights drmRights = new DrmRights(tmpFile, rightMimeType);
                int result = client.saveRights(drmRights, null, null);
                // 3. Rescan corresponding drm file
                result = client.rescanDrmMediaFiles(mContext, drmRights, null);
                success = (result == OmaDrmClient.ERROR_NONE);
            } catch (FileNotFoundException e) {
                Log.e(TAG, "SaveRightTask: FileNotFoundException when save right", e);
            } catch (IOException e) {
                Log.e(TAG, "SaveRightTask: IOException when save right", e);
            } finally {
                // close output stream, delete temp file and release drm client.
                if (fos != null) {
                    try {
                        fos.close();
                    } catch (IOException e) {
                        Log.e(TAG, "SaveRightTask: IOException when close out put stream", e);
                    }
                }
                tmpFile.delete();
                tmpFile = null;
                client.release();
                client = null;
            }
            return success;
        }

        @Override
        protected void onPostExecute(Boolean success) {
            if (success != null) {
                Log.d(TAG, "SaveRightTask with success = " + success);
                Resources res = mContext.getResources();
                String text = success.booleanValue() ?
                        res.getString(com.mediatek.internal.R.string.drm_license_install_success) :
                            res.getString(com.mediatek.internal.R.string.drm_license_install_fail);
                Notification notification = new Notification.Builder(mContext)
                        .setSmallIcon(com.mediatek.internal.R.drawable.drm_stat_notify_wappush)
                        .setTicker(text)
                        .setAutoCancel(true)
                        .setWhen(System.currentTimeMillis())
                        .build();
                NotificationManager nm = (NotificationManager) mContext.getSystemService(
                        Context.NOTIFICATION_SERVICE);
                nm.notify(0, notification);
            }
        }
    }
}
