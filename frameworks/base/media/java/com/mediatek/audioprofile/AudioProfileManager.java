/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

package com.mediatek.audioprofile;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;

import android.content.Context;
import android.media.AudioManager;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.UserHandle;
import android.provider.Settings;
import android.util.Log;

import com.mediatek.common.audioprofile.AudioProfileListener;
import com.mediatek.common.audioprofile.IAudioProfileExtension.IDefaultProfileStatesGetter;
import com.mediatek.common.audioprofile.IAudioProfileManager;
import com.mediatek.common.audioprofile.IAudioProfileService;
import com.mediatek.common.MPlugin;

/**
 * AudioProfileManager provides access to audioprofile mode control.
 * <p>
 * Use <code>Context.getSystemService(Context.AUDIO_PROFILE_SERVICE)</code> to get
 * an instance of this class.
 */
public class AudioProfileManager implements IAudioProfileManager {
    private static final String TAG = "AudioProfileManager";
    private final Context mContext;
    private static IAudioProfileService sService;

    /**
     * The max AudioProfile count.
     *
     * @internal
     */
    public static final int MAX_PROFILES_COUNT = 10;

    /**
     * The predefined AudioProfile count.
     *
     * @internal
     */
    public static final int PREDEFINED_PROFILES_COUNT = 4;

    /** The predefined max volume  outdoor. */
    public static final int DEFAULT_MAX_VOLUME_OUTDOOR = 15;

    /**
     * The audio stream for the phone ring.
     *
     * @internal
     */
    public static final int STREAM_RING = AudioManager.STREAM_RING;

    /**
     * The audio stream for notifications.
     *
     * @internal
     */
    public static final int STREAM_NOTIFICATION = AudioManager.STREAM_NOTIFICATION;

    /**
     * The audio stream for alarm.
     *
     * @internal
     */
    public static final int STREAM_ALARM = AudioManager.STREAM_ALARM;

    /**
     * Type that refers to sounds that are used for the phone ringer.
     *
     * @see #getRingtoneUri(Context, int)
     * @see #setRingtoneUri(Context, int, Uri)
     *
     * @internal
     */
    public static final int TYPE_RINGTONE = RingtoneManager.TYPE_RINGTONE;

    /**
     * Type that refers to sounds that are used for notifications.
     *
     * @see #getRingtoneUri(Context, int)
     * @see #setRingtoneUri(Context, int, Uri)
     *
     * @internal
     */
    public static final int TYPE_NOTIFICATION = RingtoneManager.TYPE_NOTIFICATION;

    /**
     * Type that refers to sounds that are used for the video call.
     *
     * @see #getRingtoneUri(Context, int)
     * @see #setRingtoneUri(Context, int, Uri)
     *
     * @internal
     */
    public static final int TYPE_VIDEO_CALL = RingtoneManager.TYPE_VIDEO_CALL;

    /**
     * Type that refers to sounds that are used for the sip call.
     *
     * @see #getRingtoneUri(Context, int)
     * @see #setRingtoneUri(Context, int, Uri)
     *
     * @internal
     */
    public static final int TYPE_SIP_CALL = RingtoneManager.TYPE_SIP_CALL;

    /** Broadcast intent action indicating that the audio profile has changed. */
    public static final String ACTION_PROFILE_CHANGED =
            "com.mediatek.audioprofile.ACTION_PROFILE_CHANGED";

    /** The scenario type of new audio profile. */
    public static final String EXTRA_PROFILE_SCENARIO =
            "com.mediatek.audioprofile.EXTRA_PROFILE_SCENARIO";

    /**
     * The prefixe of audio profile keys.
     *
     * @internal
     */
    public static final String PROFILE_PREFIX = "mtk_audioprofile_";

    /** The suffixes of the settings' keys. */
    private static final String SUFFIX_RINGER_URI = "_ringtone";
    private static final String SUFFIX_NOTIFICATION_URI = "_notification_sound";
    //M {
    private static final String SUFFIX_VIDEO_CALL_URI = "_video_call";
    private static final String SUFFIX_SIP_CALL_URI = "_sip_call";
    //}
    private static final String SUFFIX_RINGER_VOLUME = "_volume_ring";
    private static final String SUFFIX_ALARM_VOLUME = "_volume_alarm";
    private static final String SUFFIX_NOTIFICATION_VOLUME = "_volume_notification";
    private static final String SUFFIX_VIBRATION = "_vibrate_on";
    public static final String SUFFIX_SOUNDEFFECT = "_sound_effects_enabled";
    public static final String SUFFIX_DTMFTONE = "_dtmf_tone_enabled";
    public static final String SUFFIX_HAPTICFEEDBACK = "_haptic_feedback_enabled";
    public static final String SUFFIX_LOCK_SCRREN = "_lockscreen_sounds_enabled";
    public static final String SUFFIX_NOTIFICATION_USE_RING = "_notifications_use_ring_volume";
    public static final String SUFFIX_NAME = "_name";
    public static final String SUFFIX_DATE = "_date";
    public static final String SUFFIX_KEY = "_key";
    public static final String SUFFIX_DATA = "_data";
    public static final String SUFFIX_SIM_ID = "_sim_id";

    /**
     * The key used to store the active audio profile.
     *
     * @internal
     */
    public static final String KEY_ACTIVE_PROFILE = "mtk_audioprofile_active";

    /** The key used to store the previous active audio profile. */
    public static final String LAST_ACTIVE_PROFILE = "mtk_audioprofile_last_active";

    /** The key used to store whether one custom audio profile was deleted when it was set active.*/
    public static final String LAST_ACTIVE_CUSTOM_DELETED = "mtk_audioprofile_custom_deleted";

    /**
     * The key used to store the default ringtone of voice call.
     *
     * @internal
     */
    public static final String KEY_DEFAULT_RINGTONE = "mtk_audioprofile_default_ringtone";

    /**
     * The key used to store the default notification sound.
     *
     * @internal
     */
    public static final String KEY_DEFAULT_NOTIFICATION = "mtk_audioprofile_default_notification";

    /**
     * The key used to store the default ringtone of video call.
     *
     * @internal
     */
    public static final String KEY_DEFAULT_VIDEO_CALL = "mtk_audioprofile_default_video_call";

    /** The key used to store the default ringtone of sip call. */
    public static final String KEY_DEFAULT_SIP_CALL = "mtk_audioprofile_default_sip_call";

    /** The key used to store the bessuround state. */
    public static final String KEY_BESSUROUND_STATE = "mtk_audioprofile_bessuround";

    /** The key used to store the bessuround mode. */
    public static final String KEY_BESSUROUND_MODE = "mtk_audioprofile_bessuround_mode";

    /** Volume returned when unSupported stream type was passed in. */
    public static final int UNSUPPORT_STREAM_VOLUME = 0;

    /** Default ringtone for normal calls. */
    public static final Uri DEFAULT_RINGER_STREAM_URI = Settings.System.DEFAULT_RINGTONE_URI;

    /** Default ringtone for notification. */
    public static final Uri DEFAULT_NOTIFICATION_STREAM_URI =
            Settings.System.DEFAULT_NOTIFICATION_URI;

    /** Default ringtone for video calls. */
    public static final Uri DEFAULT_VIDEO_STREAM_URI = Settings.System.DEFAULT_VIDEO_CALL_URI;

    /** Default ringtone for sip calls. */
    public static final Uri DEFAULT_SIP_STREAM_URI = Settings.System.DEFAULT_SIP_CALL_URI;

    // Default AudioProfile states
    private static HashMap<Integer, AudioProfileState> sDEFAULTSTATES = null;

    public static final int KEY_SURROUND_OFF = 0;
    public static final int KEY_SURROUND_ON = 1;
    public static final int KEY_MOVIE_MODE_CODE = 0;
    public static final int KEY_MUSIC_MODE_CODE = 1;

    /**
     * The profile settings that an audio profile should have.
     */
    public enum ProfileSettings {
        ringer_stream, notification_stream, videocall_Stream, sipcall_Stream,
        ringer_volume, notification_volume, alarm_volume,
        vibration_enabled, dtmftone_enabled, soundeffect_enbled, hapticfeedback_enabled,
        lockscreensound_enabled
    }

    /**
     * The scenario that an audio profile is designed for.
     */
    public enum Scenario {
        /** @internal */ GENERAL,
        /** @internal */ SILENT,
        /** @internal */ MEETING,
        /** @internal */ OUTDOOR,
        /** @internal */ CUSTOM
    }

    // M: fixed ALPS01270292
    private static final String[] REAL_PROFILE_KEY =
            {"general", "meeting", "silent", "outdoor", "custom"};

    private static IDefaultProfileStatesGetter mDefaultProfileStatesGetter;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Construction and get AudioProfileSevices
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * AudioProfile constructor
     *
     * @param context Context
     * @hide
     */
    public AudioProfileManager(Context context) {
        Log.d(TAG, "AudioProfileManager() start");
        mContext = context;
        mDefaultProfileStatesGetter =
            MPlugin.createInstance(IDefaultProfileStatesGetter.class.getName(), mContext);
        if (mDefaultProfileStatesGetter == null) {
            mDefaultProfileStatesGetter = new DefaultProfileStatesGetter();
        }
        Log.d(TAG, "AudioProfileManager() end");
    }

    /**
     * Get AudioProfile service
     *
     * @return AudioProfile service
     */
    private static IAudioProfileService getService() {
        if (sService != null) {
            return sService;
        }
        IBinder binder = ServiceManager.getService(Context.AUDIO_PROFILE_SERVICE);
        sService = IAudioProfileService.Stub.asInterface(binder);
        return sService;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Get default state method
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the default settings .
     *
     * @param profileKey
     *            The key of the profile which's default settings to be
     *            retrieved.
     * @return The corresponding default settings of profiles except that
     *         it returns the GENERAL profile's default setting when scenario is CUSTOM.
     */
    public static AudioProfileState getDefaultState(String profileKey) {
        if (null == sDEFAULTSTATES) {
            if (mDefaultProfileStatesGetter != null) {
                sDEFAULTSTATES = mDefaultProfileStatesGetter.getDefaultProfileStates();
            }
            if (null == sDEFAULTSTATES) {
                sDEFAULTSTATES = new HashMap<Integer, AudioProfileState>(PREDEFINED_PROFILES_COUNT);
                /** Default values of ringer volume for different audio profiles. */
                final int DEFAULT_RINGER_VOLUME_GENERAL = 8;
                final int DEFAULT_RINGER_VOLUME_SILENT = 0;
                final int DEFAULT_RINGER_VOLUME_MEETING = 0;
                final int DEFAULT_RINGER_VOLUME_OUTDOOR = 15;

                /** Default values of notification volume for different audio profiles. */
                final int DEFAULT_NOTIFICATION_VOLUME_GENERAL = 8;
                final int DEFAULT_NOTIFICATION_VOLUME_SILENT = 0;
                final int DEFAULT_NOTIFICATION_VOLUME_MEETING = 0;
                final int DEFAULT_NOTIFICATION_VOLUME_OUTDOOR = 15;

                /** Default values of alarm volume for different audio profiles. */
                final int DEFAULT_ALARM_VOLUME_GENERAL = 8;
                final int DEFAULT_ALARM_VOLUME_SILENT = 0;
                final int DEFAULT_ALARM_VOLUME_MEETING = 0;
                final int DEFAULT_ALARM_VOLUME_OUTDOOR = 15;

                /** Default values of vibration for different audio profiles. */
                final boolean DEFAULT_VIBRATION_GENERAL = false;
                final boolean DEFAULT_VIBRATION_SILENT = false;
                final boolean DEFAULT_VIBRATION_MEETING = true;
                final boolean DEFAULT_VIBRATION_OUTDOOR = true;

                /**
                 * Default values that indicate whether the audible DTMF tone should be
                 * played by the dialer when dialing.
                 */
                final boolean DEFAULT_DTMFTONE_GENERAL = true;
                final boolean DEFAULT_DTMFTONE_SILENT = false;
                final boolean DEFAULT_DTMFTONE_MEETING = false;
                final boolean DEFAULT_DTMFTONE_OUTDOOR = true;

                /**
                 * Default values of sound effect(Key clicks, lid open/close...) for
                 * different audio profiles.
                 */
                final boolean DEFAULT_SOUNDEFFECT_GENERAL = false;
                final boolean DEFAULT_SOUNDEFFECT_SILENT = false;
                final boolean DEFAULT_SOUNDEFFECT_MEETING = false;
                final boolean DEFAULT_SOUNDEFFECT_OUTDOOR = false;

                /** Default values that indicate whether the lock screen sound are enabled. */
                final boolean DEFAULT_LOCK_SCREEN_GENERAL = true;
                final boolean DEFAULT_LOCK_SCREEN_SILENT = false;
                final boolean DEFAULT_LOCK_SCREEN_MEETING = false;
                final boolean DEFAULT_LOCK_SCREEN_OUTDOOR = true;

                /** Default values that indicate whether the haptic feedback are enabled. */
                final boolean DEFAULT_HAPTIC_FEEDBACK_GENERAL = true;
                final boolean DEFAULT_HAPTIC_FEEDBACK_SILENT = false;
                final boolean DEFAULT_HAPTIC_FEEDBACK_MEETING = false;
                final boolean DEFAULT_HAPTIC_FEEDBACK_OUTDOOR = true;

                // Init general state and push it to DEFAULTSTATES
                AudioProfileState generalState = new AudioProfileState
                    .Builder(getProfileKey(Scenario.GENERAL))
                    .ringtone(DEFAULT_RINGER_STREAM_URI, DEFAULT_NOTIFICATION_STREAM_URI,
                            DEFAULT_VIDEO_STREAM_URI)
                    .volume(DEFAULT_RINGER_VOLUME_GENERAL, DEFAULT_NOTIFICATION_VOLUME_GENERAL,
                            DEFAULT_ALARM_VOLUME_GENERAL)
                    .vibration(DEFAULT_VIBRATION_GENERAL)
                    .dtmfTone(DEFAULT_DTMFTONE_GENERAL)
                    .soundEffect(DEFAULT_SOUNDEFFECT_GENERAL)
                    .lockScreenSound(DEFAULT_LOCK_SCREEN_GENERAL)
                    .hapticFeedback(DEFAULT_HAPTIC_FEEDBACK_GENERAL)
                    .build();
                sDEFAULTSTATES.put(Scenario.GENERAL.ordinal(), generalState);

                // Init silent state and push it to DEFAULTSTATES
                AudioProfileState silentState = new AudioProfileState
                    .Builder(getProfileKey(Scenario.SILENT))
                    .ringtone(DEFAULT_RINGER_STREAM_URI, DEFAULT_NOTIFICATION_STREAM_URI,
                            DEFAULT_VIDEO_STREAM_URI)
                    .volume(DEFAULT_RINGER_VOLUME_SILENT, DEFAULT_NOTIFICATION_VOLUME_SILENT,
                            DEFAULT_ALARM_VOLUME_SILENT)
                    .vibration(DEFAULT_VIBRATION_SILENT)
                    .dtmfTone(DEFAULT_DTMFTONE_SILENT)
                    .soundEffect(DEFAULT_SOUNDEFFECT_SILENT)
                    .lockScreenSound(DEFAULT_LOCK_SCREEN_SILENT)
                    .hapticFeedback(DEFAULT_HAPTIC_FEEDBACK_SILENT)
                    .build();
                sDEFAULTSTATES.put(Scenario.SILENT.ordinal(), silentState);

                // Init meeting state and push it to DEFAULTSTATES
                AudioProfileState meetingState = new AudioProfileState
                    .Builder(getProfileKey(Scenario.MEETING))
                    .ringtone(DEFAULT_RINGER_STREAM_URI, DEFAULT_NOTIFICATION_STREAM_URI,
                            DEFAULT_VIDEO_STREAM_URI)
                    .volume(DEFAULT_RINGER_VOLUME_MEETING, DEFAULT_NOTIFICATION_VOLUME_MEETING,
                            DEFAULT_ALARM_VOLUME_MEETING)
                    .vibration(DEFAULT_VIBRATION_MEETING)
                    .dtmfTone(DEFAULT_DTMFTONE_MEETING)
                    .soundEffect(DEFAULT_SOUNDEFFECT_MEETING)
                    .lockScreenSound(DEFAULT_LOCK_SCREEN_MEETING)
                    .hapticFeedback(DEFAULT_HAPTIC_FEEDBACK_MEETING)
                    .build();
                sDEFAULTSTATES.put(Scenario.MEETING.ordinal(), meetingState);

                // Init outdoor state and push it to DEFAULTSTATES
                AudioProfileState outdoorState = new AudioProfileState
                    .Builder(getProfileKey(Scenario.OUTDOOR))
                    .ringtone(DEFAULT_RINGER_STREAM_URI, DEFAULT_NOTIFICATION_STREAM_URI,
                            DEFAULT_VIDEO_STREAM_URI)
                    .volume(DEFAULT_RINGER_VOLUME_OUTDOOR, DEFAULT_NOTIFICATION_VOLUME_OUTDOOR,
                            DEFAULT_ALARM_VOLUME_OUTDOOR)
                    .vibration(DEFAULT_VIBRATION_OUTDOOR)
                    .dtmfTone(DEFAULT_DTMFTONE_OUTDOOR)
                    .soundEffect(DEFAULT_SOUNDEFFECT_OUTDOOR)
                    .lockScreenSound(DEFAULT_LOCK_SCREEN_OUTDOOR)
                    .hapticFeedback(DEFAULT_HAPTIC_FEEDBACK_OUTDOOR)
                    .build();
                sDEFAULTSTATES.put(Scenario.OUTDOOR.ordinal(), outdoorState);
                Log.d(TAG, "getDefaultState from default!");
            }
        }

        Scenario scenario = getScenario(profileKey);
        if (scenario == null) {
            Log.w(TAG, "getDefaultState: get null scenario and use custom scenario default!");
            scenario = Scenario.CUSTOM;
        }
        int index = scenario.ordinal();
        if (index >= sDEFAULTSTATES.size()) {
            return sDEFAULTSTATES.get(0);
        } else {
            return sDEFAULTSTATES.get(index);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Get key methods
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the key for persisting setting that indicates whether phone should vibrate for incoming
     * calls.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting setting.
     * @internal
     */
    public static String getVibrationKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_VIBRATION;
        }
        return null;
    }

    /**
     * Gets the key for persisting setting that indicates whether sound should be played when making
     * screen selection.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting setting.
     *
     */
    public static String getSoundEffectKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_SOUNDEFFECT;
        }
        return null;
    }

    /**
     * Gets the key for persisting setting that indicates whether sound should be played when using
     * dial pad.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting setting.
     */
    public static String getDtmfToneKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_DTMFTONE;
        }
        return null;
    }

    /**
     * Gets the key for persisting the setting that indicates whether the phone should vibrate when
     * pressing soft keys and on certain UI interactions.
     *
     * @param profileKey
     *            The key of the profile..
     * @return The key for persisting setting.
     */
    public static String getHapticKey(String profileKey) {
         if (profileKey != null) {
            return profileKey + SUFFIX_HAPTICFEEDBACK;
        }
        return null;
    }

    /**
     * Gets the key for persisting volume.
     *
     * @param profileKey
     *            The key of the profile.
     * @param type
     *            The stream type whose volume index to be persisted. One of
     *            {@link AudioProfile#STREAM_RING}, {@link AudioProfile#STREAM_NOTIFICATION}, or
     *            {@link AudioProfile#STREAM_ALARM}.
     * @return The key for persisting setting.
     */
    public static String getStreamVolumeKey(String profileKey, int type) {
        if (profileKey == null) {
            Log.e(TAG, "getStreamVolumeKey with null profile key!");
            return null;
        }

        String volumeKey = null;
        switch (type) {
            case STREAM_RING:
                volumeKey = profileKey + SUFFIX_RINGER_VOLUME;
                break;

            case STREAM_NOTIFICATION:
                volumeKey = profileKey + SUFFIX_NOTIFICATION_VOLUME;
                break;

            case STREAM_ALARM:
                volumeKey = profileKey + SUFFIX_ALARM_VOLUME;
                break;

            default:
                Log.e(TAG, "getStreamVolumeKey with unsupport type!");
        }
        return volumeKey;
    }

    /**
     * Gets the key for persisting stream volume.
     *
     * @param profileKey
     *            The key of the profile.
     * @param type
     *            The type whose default sound to be persisted. One of
     *            {@link AudioProfile#TYPE_RINGTONE}, {@link AudioProfile#TYPE_NOTIFICATION}, or
     *            {@link AudioProfile#TYPE_VOICE_CALL}.
     * @return The key for persisting setting.
     */
    public static String getStreamUriKey(String profileKey, int type) {
        long simId = -1 ;
        return getStreamUriKey(profileKey, type, simId);
    }

    /**
     * Gets the key for persisting stream volume.
     *
     * @param profileKey
     *            The key of the profile.
     * @param type
     *            The type whose default sound to be persisted. One of
     *            {@link AudioProfile#TYPE_RINGTONE}, {@link AudioProfile#TYPE_NOTIFICATION}, or
     *            {@link AudioProfile#TYPE_VOICE_CALL}.
     * @return The key for persisting setting.
     */
    public static String getStreamUriKey(String profileKey, int type, long simId) {
        if (profileKey == null) {
            Log.e(TAG, "getStreamUriKey with null profile key!");
            return null;
        }

        String uriKey = null;
        switch (type) {
            case TYPE_RINGTONE:
                uriKey = profileKey + SUFFIX_RINGER_URI;
                // M add for sim difference ringtone {
                if (simId != -1) {
                    uriKey = uriKey + SUFFIX_SIM_ID + simId;
                }

                //}
                break;

            case TYPE_NOTIFICATION:
                uriKey = profileKey + SUFFIX_NOTIFICATION_URI;
                break;

            case TYPE_VIDEO_CALL:
                uriKey = profileKey + SUFFIX_VIDEO_CALL_URI;
                //uriKey = profileKey + SUFFIX_RINGER_URI;
                // M add for sim difference ringtone {
                if (simId != -1) {
                    uriKey = uriKey + SUFFIX_SIM_ID + simId;
                }

                //}
                break;

            case TYPE_SIP_CALL:
                uriKey = profileKey + SUFFIX_SIP_CALL_URI;
                //uriKey = profileKey + SUFFIX_RINGER_URI;
                // M add for sim difference ringtone {
                if (simId != -1) {
                    uriKey = uriKey + SUFFIX_SIM_ID + simId;
                }

                //}
                break;
            default:
                Log.e(TAG, "getStreamUriKey with unsupport type!");
        }
        Log.d(TAG, "getStreamUriKey: StreamUriKey = " + uriKey);
        return uriKey;
    }

    /**
     * Gets the keys to save the default ringtone for predefined scenarios except for silent.
     *
     * @param type
     *            Stream type. One of {@link AudioProfile#TYPE_RINGTONE},
     *            {@link AudioProfile#TYPE_NOTIFICATION}, or {@link AudioProfile#TYPE_VOICE_CALL}.
     * @return The stream uri key of given type
     */
    public static List<String> getStreamUriKeys(int type) {
        List<String> keys = new ArrayList<String>();
        for (Scenario scenario : Scenario.values()) {
            if (!Scenario.SILENT.equals(scenario) && !Scenario.CUSTOM.equals(scenario)) {
                String prefix = PROFILE_PREFIX + coverToProfileKey(scenario);
                String key = getStreamUriKey(prefix, type);
                if (key != null) {
                    keys.add(key);
                }
            }
        }
        return keys;
    }

    public static String getDataKey(String uriKey) {
        if (uriKey != null) {
            return uriKey + SUFFIX_DATA;
        }
        return null;
    }

    /**
     * Gets the key for persisting the setting that indicates whether sounds should be played when
     * the keyguard is shown and dismissed.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting setting.
     */
    public static String getLockScreenKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_LOCK_SCRREN;
        }
        return null;
    }

    /**
     * Gets the key for persisting the setting that indicates whether notification use ring volume.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting setting.
     */
    public static String getNotificationUseRingKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_NOTIFICATION_USE_RING;
        }
        return null;
    }

    /**
     * Gets the key for persisting the profile's name.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for persisting profile's name.
     */
    public static String getProfileNameKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_NAME;
        }
        return null;
    }

    /**
     * Gets the key for created date of the profile.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key for created date.
     */
    public static String getProfileCreateDateKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_DATE;
        }
        return null;
    }

    /**
     * Gets the key for persisting the profile's key.
     *
     * @param profileKey
     *            The key of the profile.
     * @return The key of given profile key
     */
    public static String getKey(String profileKey) {
        if (profileKey != null) {
            return profileKey + SUFFIX_KEY;
        }
        return null;
    }

    /**
     * Gets the profile's key from scenario type.
     *
     * @param scenario
     *            The scenario type.
     * @return The scenario's profile key. If scenario is one of four predefine types of scenario,
     *         the profile key can be used immediately, is it is custom one, the profile key should
     *         be add more information to mark off custom profiles.
     * @internal
     */
    public static String getProfileKey(Scenario scenario) {
        if (scenario != null) {
            return PROFILE_PREFIX + coverToProfileKey(scenario);
        }
        Log.e(TAG, "getProfileKey with null scenario!");
        return null;
    }

    // M: fixed ALPS01270292
    private static String coverToProfileKey(Scenario scenario) {
        String realProfileKey = null;
        if (scenario.equals(scenario.GENERAL)) {
            realProfileKey = REAL_PROFILE_KEY[0];
        } else if (scenario.equals(scenario.MEETING)) {
            realProfileKey = REAL_PROFILE_KEY[1];
        } else if (scenario.equals(scenario.SILENT)) {
            realProfileKey = REAL_PROFILE_KEY[2];
        } else if (scenario.equals(scenario.OUTDOOR)) {
            realProfileKey = REAL_PROFILE_KEY[3];
        } else if (scenario.equals(scenario.CUSTOM)) {
            realProfileKey = REAL_PROFILE_KEY[4];
        }
        return realProfileKey;
    }

    /**
     * Gets the scenario type from profile's key.
     *
     * @param profileKey
     *            The profile's key.
     * @return The scenario type of this profile. If the key doesn't contains the scenario, returns
     *         {@link Scenario#CUSTOM}.
     * @internal
     */
    public static Scenario getScenario(String profileKey) {
        if (profileKey == null) {
            Log.w(TAG, "getScenario: Null key! Return CUSTOM as default!");
            return Scenario.CUSTOM;
        }

        int keyLen = profileKey.length();
        int startIndex = PROFILE_PREFIX.length();
        if (keyLen < startIndex) {
            Log.w(TAG, "getScenario: Invalid key :" + profileKey + ", Return CUSTOM as default!");
            return Scenario.CUSTOM;
        }

        int endIndex = profileKey.indexOf('_', startIndex);

        String scenarioStr = profileKey.substring(
                startIndex, endIndex == -1 ? profileKey.length() : endIndex);
        try {
            return Scenario.valueOf(scenarioStr.toUpperCase(Locale.ENGLISH));
        } catch (IllegalArgumentException e) {
            Log.e(TAG, "Can not convert string " + scenarioStr + " to Scenario type!");
            return Scenario.CUSTOM;
        }
    }

    /**
     * Gets all the keys used to store the settings of one audio profile.
     *
     * @param profileKey
     *            Audio profile key.
     * @return All keys of given profile
     */
    public static List<String> getAllKeys(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "getAllKeys: Null profileKey!");
            return null;
        }

        final int keySize = 14; // avoid magic number
        List<String> keys = new ArrayList<String>(keySize);
        keys.add(getKey(profileKey));
        keys.add(getProfileNameKey(profileKey));
        keys.add(getProfileCreateDateKey(profileKey));
        keys.add(getNotificationUseRingKey(profileKey));
        keys.add(getLockScreenKey(profileKey));
        keys.add(getHapticKey(profileKey));
        keys.add(getDtmfToneKey(profileKey));
        keys.add(getSoundEffectKey(profileKey));
        keys.add(getVibrationKey(profileKey));
        keys.add(getStreamVolumeKey(profileKey, STREAM_ALARM));
        keys.add(getStreamVolumeKey(profileKey, STREAM_NOTIFICATION));
        keys.add(getStreamVolumeKey(profileKey, STREAM_RING));
        keys.add(getStreamUriKey(profileKey, TYPE_RINGTONE));
        keys.add(getDataKey(getStreamUriKey(profileKey, TYPE_RINGTONE)));
        keys.add(getStreamUriKey(profileKey, TYPE_NOTIFICATION));
        keys.add(getDataKey(getStreamUriKey(profileKey, TYPE_NOTIFICATION)));
        keys.add(getStreamUriKey(profileKey, TYPE_VIDEO_CALL));
        keys.add(getDataKey(getStreamUriKey(profileKey, TYPE_VIDEO_CALL)));
        return keys;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Set active profile, add profile, delete profile and reset profiles
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Sets the active profile with given profile key.
     *
     * @param profileKey
     *            The key of the profile that set to be active.
     * @internal
     */
    public void setActiveProfile(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "setActiveProfile with null profile key!");
            return;
        }

        Log.d(TAG, "setActiveProfile: profileKey = " + profileKey);
        IAudioProfileService service = getService();
        try {
            service.setActiveProfile(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setActiveProfile", e);
        }
    }

    /**
     * Adds a new {@link Scenario#CUSTOM} type profile.
     *
     * @return The new profile key.
     * @internal
     */
    public String addProfile() {
        IAudioProfileService service = getService();
        try {
            return service.addProfile();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setActiveProfile", e);
            return null;
        }
    }

    /**
     * Deletes a {@link Scenario#CUSTOM} type profile.
     *
     * @param profileKey
     *            The key of the profile that to be deleted.
     * @return True if delete succeed, otherwise false.
     * @internal
     */
    public boolean deleteProfile(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "deleteProfile with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.deleteProfile(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in deleteProfile", e);
            return false;
        }
    }

    /**
     * Reset all profiles.
     *
     * @internal
     */
    public void resetProfiles() {
        IAudioProfileService service = getService();
        try {
            service.reset();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in resetProfile", e);
        }
    }

    /**
     * Gets the number of current existing profiles. Include the predefined and custom ones.
     *
     * @return The number of existing profiles.
     * @internal
     */
    public int getProfileCount() {
        IAudioProfileService service = getService();
        try {
            return service.getProfileCount();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in deleteProfile", e);
            return MAX_PROFILES_COUNT;
        }
    }

    /**
     * Gets the all existed profiles' keys.
     *
     * @return The existed profiles' keys.
     * @internal
     */
    public List<String> getAllProfileKeys() {
        IAudioProfileService service = getService();
        try {
            return service.getAllProfileKeys();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getAllProfileKeys", e);
            return null;
        }
    }

    /**
     * Gets predefined profiles' keys.
     *
     * @return The predefined profiles' keys.
     * @internal
     */
    public List<String> getPredefinedProfileKeys() {
        IAudioProfileService service = getService();
        try {
            return service.getPredefinedProfileKeys();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getPredefinedProfileKeys", e);
            return null;
        }
    }

    /**
     * Gets customized profiles' keys.
     *
     * @return The customized profiles' keys.
     */
    public List<String> getCustomizedProfileKeys() {
        IAudioProfileService service = getService();
        try {
            return service.getCustomizedProfileKeys();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getCustomizedProfileKeys", e);
            return null;
        }
    }

    /**
     * Checks out whether the name existed.
     *
     * @param name
     *            The name to be checked.
     * @return True if the specified name had existed or if the name is null or empty, false
     *         otherwise.
     * @internal
     */
    public boolean isNameExist(String name) {
        if ((name == null) || (name.equals(""))) {
            Log.w(TAG, "isNameExist: Null or empty name!");
            return true;
        }

        IAudioProfileService service = getService();
        try {
            return service.isNameExist(name);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isNameExist", e);
            return true;
        }
    }

    /**
     * Gets the key of active profile.
     *
     * @return The key of the active profile.
     * @internal
     */
    public String getActiveProfileKey() {
        IAudioProfileService service = getService();
        try {
            return service.getActiveProfileKey();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getActiveProfileKey", e);
            return null;
        }
    }

    /**
     * Gets the key of previous non-silent active profile.
     *
     * @return The key of last non-silent active profile.
     */
    public String getLastActiveProfileKey() {
        IAudioProfileService service = getService();
        try {
            return service.getLastActiveProfileKey();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getLastActiveProfileKey", e);
            return null;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Get methods to get profile settings with given profile key from persisted profile states
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the {@link Uri} of the default sound for a given ring tone type.
     *
     * @param profileKey The profile key ringtone uri is returned.
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}
     *            or {@link #TYPE_VIDEO_CALL}.
     * @return A {@link Uri} pointing to the default sound to set.
     * @see #setRingtoneUri(Context, int, Uri)
     * @internal
     */
    public Uri getRingtoneUri(String profileKey, int type, long simId) {
        if (profileKey == null) {
            Log.e(TAG, "getRingtoneUri with null profile key!");
            return null;
        }

        if ((type == TYPE_RINGTONE) || (type == TYPE_NOTIFICATION)
                || (type == TYPE_VIDEO_CALL) || (type == TYPE_SIP_CALL)) {
            IAudioProfileService service = getService();
            try {
                return service.getRingtoneUriWithSIM(profileKey, type, simId);
            } catch (RemoteException e) {
                Log.e(TAG, "Dead object in getRingtoneUri", e);
                return null;
            }
        } else {
            Log.e(TAG, "getRingtoneUri with unsupport stream type!");
            return null;
        }
    }

    /**
     * Gets the {@link Uri} of the default sound for a given ring tone type.
     *
     * @param profileKey The profile key ringtone uri is returned.
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}
     *            or {@link #TYPE_VIDEO_CALL}.
     * @return A {@link Uri} pointing to the default sound to set.
     * @see #setRingtoneUri(Context, int, Uri)
     * @internal
     */
    public Uri getRingtoneUri(String profileKey, int type) {
        long simId = -1;
        return getRingtoneUri(profileKey, type, simId);
    }

    /**
     * Gets the {@link Uri} of the default sound for a given ring tone type.
     *
     * @param profileKey The profile key ringtone uri is returned.
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}
     *            or {@link #TYPE_VIDEO_CALL}.
     * @return A {@link Uri} pointing to the default sound to set.
     * @see #setRingtoneUri(Context, int, Uri)
     */
    public Uri getRingtoneUri(int simId) {
        String profileKey = getActiveProfileKey();
        int type = TYPE_RINGTONE ;
        return getRingtoneUri(profileKey, type, simId);
    }

    /**
     * Get ringtone URI
     *
     * @param callType VOICE_CALL ,VIDEO_CALL, SIP_CALL
     * @param simId  SIM id
     *
     * @return
     */
    public Uri getRingtoneUri(int callType, long simId) {
        String profileKey = getActiveProfileKey();
        return getRingtoneUri(profileKey, callType, simId);
    }

    /**
     * Returns the current volume index for a particular stream.
     * @param profileKey The profile key whose volume index is returned.
     * @param streamType The stream type whose volume index is returned.
     *                   One of {@link #STREAM_RING}, {@link #STREAM_NOTIFICATION}
     *                   or {@link #STREAM_ALARM}.
     *
     * @return The current volume index for the stream.
     * @see #getStreamMaxVolume(int)
     * @see #setStreamVolume(int, int, int)
     * @internal
     */
    public int getStreamVolume(String profileKey, int streamType) {
        if (profileKey == null) {
            Log.e(TAG, "getStreamVolume with null profile key!");
            return UNSUPPORT_STREAM_VOLUME;
        }

        if ((streamType == STREAM_RING) || (streamType == STREAM_NOTIFICATION)
                || (streamType == STREAM_ALARM)) {
            IAudioProfileService service = getService();
            try {
                return service.getStreamVolume(profileKey, streamType);
            } catch (RemoteException e) {
                Log.e(TAG, "Dead object in getStreamVolume", e);
                return UNSUPPORT_STREAM_VOLUME;
            }
        } else {
            Log.e(TAG, "getStreamVolume with unsupport stream type!");
            return UNSUPPORT_STREAM_VOLUME;
        }
    }

    /**
     * Gets whether the phone should vibrate for incoming calls.
     *
     * @param profileKey The profile key whose DtmfTone whether enabled is returned.
     * @return The current vibration status, if enabled return true, otherwise false.
     * @see #setVibrationEnabled(boolean)
     * @internal
     */
    public boolean isVibrationEnabled(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isVibrationEnabled with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isVibrationEnabled(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isVibrationEnabled", e);
            return false;
        }
    }

    /**
     * Gets whether tone should be played when using dial pad with the given profile key.
     *
     * @param profileKey The profile key whose DtmfTone whether enabled is returned.
     * @return The current DtmfTone status, if enabled return true, otherwise false.
     * @see #setDtmfToneEnabled(boolean)
     * @internal
     */
    public boolean isDtmfToneEnabled(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isDtmfToneEnabled with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isDtmfToneEnabled(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isDtmfToneEnabled", e);
            return false;
        }
    }

    /**
     * Gets whether sound should be played when making screen selection.
     *
     * @param profileKey The profile key whose SoundEffect whether enabled is returned.
     * @return The current SoundEffect status, if enabled return true, otherwise false.
     * @see #setSoundEffectEnabled(boolean)
     * @internal
     */
    public boolean isSoundEffectEnabled(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isSoundEffectEnabled with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isSoundEffectEnabled(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isSoundEffectEnabled", e);
            return false;
        }
    }

    /**
     * Gets whether sound should be played when lock or unlock screen.
     *
     * @param profileKey The profile key whose LockScreen whether enabled is returned.
     * @return The current LockScreen status, if enabled return true, otherwise false.
     * @see #setLockScreenEnabled(String, boolean)
     * @internal
     */
    public boolean isLockScreenEnabled(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isLockScreenEnabled with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isLockScreenEnabled(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isLockScreenEnabled", e);
            return false;
        }
    }

    /**
     * Gets whether the phone should vibrate when pressing soft keys and on certain UI interactions.
     *
     * @param profileKey The profile key whose HapticFeedback whether enabled is returned.
     * @return The current HapticFeedback status, if enabled return true, otherwise false.
     * @see #setVibrateOnTouchEnabled(boolean)
     * @internal
     */
    public boolean isVibrateOnTouchEnabled(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isVibrateOnTouchEnabled with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isVibrateOnTouchEnabled(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isVibrateOnTouchEnabled", e);
            return false;
        }
    }

    /**
     * Gets the profile state from profile states hash map with given profile key.
     *
     * @param profileKey The profile key.
     * @return The current profile state referred to given profile key.
     *
     */
    public AudioProfileState getProfileState(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "getProfileState with null profile key!");
            return null;
        }
        List<String> state;

        IAudioProfileService service = getService();
        try {
            state = service.getProfileStateString(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getProfileState", e);
            return null;
        }

        String value = state.get(ProfileSettings.ringer_stream.ordinal());
        Uri voiceCallUri = (value == null ? null : Uri.parse(value));
        value = state.get(ProfileSettings.notification_stream.ordinal());
        Uri notificationUri = (value == null ? null : Uri.parse(value));
        value = state.get(ProfileSettings.videocall_Stream.ordinal());
        Uri videoCallUri = (value == null ? null : Uri.parse(value));
        value = state.get(ProfileSettings.sipcall_Stream.ordinal());
        Uri sipCallUri = (value == null ? null : Uri.parse(value));

        value = state.get(ProfileSettings.ringer_volume.ordinal());
        int ringerVolume = (value == null ? 0 : Integer.valueOf(value));
        value = state.get(ProfileSettings.notification_volume.ordinal());
        int notificationVolume = (value == null ? 0 : Integer.valueOf(value));
        value = state.get(ProfileSettings.alarm_volume.ordinal());
        int alarmVolume = (value == null ? 0 : Integer.valueOf(value));

        value = state.get(ProfileSettings.vibration_enabled.ordinal());
        boolean vibration = Boolean.valueOf(value);
        value = state.get(ProfileSettings.dtmftone_enabled.ordinal());
        boolean dtmfTone = Boolean.valueOf(value);
        value = state.get(ProfileSettings.soundeffect_enbled.ordinal());
        boolean soundEffect = Boolean.valueOf(value);
        value = state.get(ProfileSettings.lockscreensound_enabled.ordinal());
        boolean lockScreensound = Boolean.valueOf(value);
        value = state.get(ProfileSettings.hapticfeedback_enabled.ordinal());
        boolean hapticFeedback = Boolean.valueOf(value);

        AudioProfileState profileState = new AudioProfileState.Builder(profileKey)
            .ringtone(voiceCallUri, notificationUri, videoCallUri, sipCallUri)
            .volume(ringerVolume, notificationVolume, alarmVolume)
            .vibration(vibration)
            .dtmfTone(dtmfTone)
            .soundEffect(soundEffect)
            .lockScreenSound(lockScreensound)
            .hapticFeedback(hapticFeedback)
            .build();
        Log.d(TAG, "getProfileState for profileKey = " + profileKey + ": "
                + profileState.toString());
        return profileState;
    }

    /**
     * Returns the name of given custom profile.
     *
     * @param profileKey The custom profile key.
     * @return profile name
     * @see #setProfileName(String, String)
     * @internal
     */
    public String getProfileName(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "getProfileName with null profile key!");
            return null;
        }

        IAudioProfileService service = getService();
        try {
            return service.getProfileName(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getProfileName", e);
            return null;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Set methods to set profile setting to database with given profile key
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Sets the {@link Uri} of the default sound for a given ring tone type and profile key.
     *
     * @param profileKey The profile key given to set ringtone uri.
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION},
     *             or {@link #TYPE_VIDEO_CALL}.
     * @param ringtoneUri A {@link Uri} pointing to the default sound to set.
     * @see #getRingtoneUri(Context, int)
     * @internal
     */
    public void setRingtoneUri(String profileKey, int type, Uri ringtoneUri) {
        int simId = -1;
        setRingtoneUri(profileKey, type, simId, ringtoneUri);
    }

    /**
     * Sets the {@link Uri} of the default sound for a given ring tone type and profile key.
     *
     * @param profileKey The profile key given to set ringtone uri.
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION},
     *             or {@link #TYPE_VIDEO_CALL}.
     * @param ringtoneUri A {@link Uri} pointing to the default sound to set.
     * @see #getRingtoneUri(Context, int)
     */
    public void setRingtoneUri(String profileKey, int type, long simId, Uri ringtoneUri) {
        if (profileKey == null) {
            Log.e(TAG, "setStreamVolume with null profile key!");
            return;
        }

        if ((type != TYPE_RINGTONE) && (type != TYPE_NOTIFICATION)
                && (type != TYPE_VIDEO_CALL) && (type != TYPE_SIP_CALL)) {
            Log.e(TAG, "setStreamVolume with unsupport stream type!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setRingtoneUri(profileKey, type, simId, ringtoneUri);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setRingtoneUri", e);
        }
    }

    /**
     * Sets the volume index for a particular stream to database.
     *
     * @param profileKey The profile key given to set stream volume.
     * @param streamType The stream whose volume index should be set.
     *                   One of {@link #STREAM_RING}, {@link #STREAM_NOTIFICATION}
     *                   or {@link #STREAM_ALARM}.
     * @param index The volume index to set.
     * @see #getStreamMaxVolume(int)
     * @see #getStreamVolume(int)
     * @internal
     */
    public void setStreamVolume(String profileKey, int streamType, int index) {
        if (profileKey == null) {
            Log.e(TAG, "setStreamVolume with null profile key!");
            return;
        }

        if ((streamType != STREAM_RING) && (streamType != STREAM_NOTIFICATION)
                && (streamType != STREAM_ALARM)) {
            Log.e(TAG, "setStreamVolume with unsupport stream type!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setStreamVolume(profileKey, streamType, index);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setStreamVolume", e);
        }
    }

    /**
     * Sets whether the phone should vibrate for incoming calls.
     *
     * @param profileKey The profile key given to set vibration enabled.
     * @param enabled Whether vibration enabled.
     * @see #isVibrationEnabled()
     * @internal
     */
    public void setVibrationEnabled(String profileKey, boolean enabled) {
        if (profileKey == null) {
            Log.e(TAG, "setVibrationEnabled with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setVibrationEnabled(profileKey, enabled);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setVibrationEnabled", e);
        }
    }

    /**
     * Gets whether tone should be played when using dial pad.
     *
     * @param profileKey The profile key given to set vibration enabled.
     * @param enabled Whether DtmfTone enabled.
     * @see #isDtmfToneEnabled(boolean)
     * @internal
     */
    public void setDtmfToneEnabled(String profileKey, boolean enabled) {
        if (profileKey == null) {
            Log.e(TAG, "setDtmfToneEnabled with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setDtmfToneEnabled(profileKey, enabled);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setDtmfToneEnabled", e);
        }
    }

    /**
     * Sets whether sound should be played when making screen selection.
     *
     * @param profileKey The profile key given to set vibration enabled.
     * @param enabled Whether SoundEffect enabled.
     * @see #isSoundEffectEnabled()
     * @internal
     */
    public void setSoundEffectEnabled(String profileKey, boolean enabled) {
        if (profileKey == null) {
            Log.e(TAG, "setSoundEffectEnabled with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setSoundEffectEnabled(profileKey, enabled);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setSoundEffectEnabled", e);
        }
    }

    /**
     * Sets whether sound should be played when lock or unlock screen.
     *
     * @param profileKey The profile key given to set vibration enabled.
     * @param enabled Whether LockScreen sound enabled.
     * @see #isLockScreenEnabled(String)
     * @internal
     */
    public void setLockScreenEnabled(String profileKey, boolean enabled) {
        if (profileKey == null) {
            Log.e(TAG, "setLockScreenEnabled with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setLockScreenEnabled(profileKey, enabled);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setLockScreenEnabled", e);
        }
    }

    /**
     * Sets whether the phone should vibrate when pressing soft keys and on certain UI interactions.
     *
     * @param profileKey The profile key given to set vibration enabled.
     * @param enabled Whether HapticFeedback enabled.
     * @see #isVibrateOnTouchEnabled(String)
     * @internal
     */
    public void setVibrateOnTouchEnabled(String profileKey, boolean enabled) {
        if (profileKey == null) {
            Log.e(TAG, "setVibrateOnTouchEnabled with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setVibrateOnTouchEnabled(profileKey, enabled);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setVibrateOnTouchEnabled", e);
        }
    }

    /**
     * Sets the given profile's name.
     *
     * @param profileKey The key of the profile.
     * @param newName The new name to be set.
     * @see #getProfileName(String)
     * @internal
     */
    public void setProfileName(String profileKey, String newName) {
        if (profileKey == null) {
            Log.e(TAG, "setProfileName with null profile key!");
            return;
        }

        IAudioProfileService service = getService();
        try {
            service.setProfileName(profileKey, newName);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setProfileName", e);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Other methods
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Checks whether this given profile is the active one.
     *
     * @param profileKey The profile key .
     * @return True if the given profile is active, otherwise false.
     * @internal
     */
    public boolean isActiveProfile(String profileKey) {
        if (profileKey == null) {
            Log.e(TAG, "isActiveProfile with null profile key!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isActiveProfile(profileKey);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isActiveProfile", e);
            return false;
        }
    }

    /**
     * Check whether the given uri is exist
     *
     * @param uri The uri to be checked
     * @return If the uri is exist, return true, otherwise false
     */
    public boolean isRingtoneExist(Uri uri) {
        if (uri == null) {
            Log.e(TAG, "isRingtoneExist with null uri!");
            return false;
        }

        IAudioProfileService service = getService();
        try {
            return service.isRingtoneExist(uri);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isRingtoneExist", e);
            return false;
        }
    }

    /**
     * Returns the maximum volume index for a particular stream.
     *
     * @param streamType The stream type whose maximum volume index is returned. Currently only
     *                   {@link #STREAM_RING}, {@link #STREAM_NOTIFICATION} and
     *                   {@link #STREAM_ALARM} are supported.
     * @return The maximum valid volume index for the stream.
     * @see #getStreamVolume(int)
     * @internal
     */
    public int getStreamMaxVolume(int streamType) {
        IAudioProfileService service = getService();
        try {
            return service.getStreamMaxVolume(streamType);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isRingtoneExist", e);
            return UNSUPPORT_STREAM_VOLUME;
        }
    }

    /**
     * Returns the default ringtone for a particular stream.
     *
     * @param type The type whose default sound should be set. One of
     *            {@link #TYPE_RINGTONE}, {@link #TYPE_NOTIFICATION}
     *            or {@link #TYPE_VIDEO_CALL}.
     * @return The default ringtone uri.
     * @see #setRingtoneUri(String, int, Uri)
     */
    public Uri getDefaultRingtone(int type) {
        IAudioProfileService service = getService();
        try {
            return service.getDefaultRingtone(type);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in isRingtoneExist", e);
            return null;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Register a AudiopPofile listener callback to AudioProfileService
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Register the AudioProfileListener to AudioProfileService to listen
     * AudioProfile changed.
     *
     * @param listener AudioProfileListener.
     * @param event The event for listener.
     * @internal
     *
     * */
    public void listenAudioProfie(AudioProfileListener listener, int event) {
        IAudioProfileService service = getService();
        try {
            service.listenAudioProfie(listener.getCallback(), event);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in listenAudioProfie", e);
        }
    }

    /**
     * Get the BesSurround State.
     *
     * @param
     * @return BesSurround state
     */
    public boolean getBesSurroundState() {
        IAudioProfileService service = getService();
        try {
            return service.getBesSurroundState();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getBesSurroundState", e);
            return false;
        }
    }

    /**
     * Set the BesSurround State.
     *
     * @param isEnableBes
     * @return
     */
    public int setBesSurroundState(boolean isEnableBes) {
        IAudioProfileService service = getService();
        try {
            return service.setBesSurroundState(isEnableBes);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setBesSurroundState", e);
            return -1;
        }
    }

    /**
     * Get the BesSurround mode.
     *
     * @param
     * @return BesSurround mode
     */
    public int getBesSurroundMode() {
        IAudioProfileService service = getService();
        try {
            return service.getBesSurroundMode();
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in getBesSurroundMode", e);
            return -1;
        }
    }

    /**
     * Set the BesSurround mode.
     *
     * @param isEnableBes
     * @return
     */
    public int setBesSurroundMode(int besSurroundMode) {
        IAudioProfileService service = getService();
        try {
            return service.setBesSurroundMode(besSurroundMode);
        } catch (RemoteException e) {
            Log.e(TAG, "Dead object in setBesSurroundMode", e);
            return -1;
        }
    }
}
