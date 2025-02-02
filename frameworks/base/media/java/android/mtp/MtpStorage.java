/*
* Copyright (C) 2014 MediaTek Inc.
* Modification based on code covered by the mentioned copyright
* and/or permission notice(s).
*/
/*
 * Copyright (C) 2011 The Android Open Source Project
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

package android.mtp;

import android.content.Context;
import android.os.storage.StorageVolume;

/**
 * This class represents a storage unit on an MTP device.
 * Used only for MTP support in USB responder mode.
 * MtpStorageInfo is used in MTP host mode
 *
 * @hide
 */
public class MtpStorage {

    private final int mStorageId;
    private final String mPath;
    private final long mReserveSpace;
    private final long mMaxFileSize;

    private String mDescription;
    private boolean mRemovable;

    public MtpStorage(StorageVolume volume, Context context) {
        mStorageId = volume.getStorageId();
        mPath = volume.getPath();
        mDescription = volume.getDescription(context);
        mReserveSpace = volume.getMtpReserveSpace() * 1024L * 1024L;
        mRemovable = volume.isRemovable();
        mMaxFileSize = volume.getMaxFileSize();
    }

   /**
     * set the description string for the storage unit
     *
     * @internal
     */
    public void setDescription(String desc) {
        mDescription = desc;
    }

    /**
     * set true if the storage is removable.
     *
     * @internal
     */
    public void setRemovable(boolean removable) {
        mRemovable = removable;
    }

    /**
     * Returns the storage ID for the storage unit
     *
     * @return the storage ID
     */
    public final int getStorageId() {
        return mStorageId;
    }

   /**
     * Returns the file path for the storage unit's storage in the file system
     *
     * @return the storage file path
     */
    public final String getPath() {
        return mPath;
    }

   /**
     * Returns the description string for the storage unit
     *
     * @return the storage unit description
     */
    public final String getDescription() {
        return mDescription;
    }

   /**
     * Returns the amount of space to reserve on the storage file system.
     * This can be set to a non-zero value to prevent MTP from filling up the entire storage.
     *
     * @return reserved space in bytes.
     */
    public final long getReserveSpace() {
        return mReserveSpace;
    }

   /**
     * Returns true if the storage is removable.
     *
     * @return is removable
     */
    public final boolean isRemovable() {
        return mRemovable;
    }

   /**
     * Returns maximum file size for the storage, or zero if it is unbounded.
     *
     * @return maximum file size
     */
    public long getMaxFileSize() {
        return mMaxFileSize;
    }
}
