package com.gscoder.android.labadienatts;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;


public class CheckVoiceData extends Activity {
    private final static String LOG_TAG = "Laba_Diena_TTS_Java_" + CheckVoiceData.class.getSimpleName();
    public final static String VOICE_LIST_FILE_ = "voices.list";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        int result = TextToSpeech.Engine.CHECK_VOICE_DATA_PASS;
        Intent returnData = new Intent();
        returnData.putExtra(TextToSpeech.Engine.EXTRA_VOICE_DATA_ROOT_DIRECTORY,
                Voice.getDataStorageBasePath(this));

        ArrayList<String> available = new ArrayList<String>();
        ArrayList<String> unavailable = new ArrayList<String>();

        if(!Utility.pathExists( Voice.getDataStorageBasePath(this))) {
            // Create the directory.
            Log.e(LOG_TAG, "Liepa data directory missing. Trying to create it.");
            boolean success = false;

            try {
                Log.e(LOG_TAG,  Voice.getDataStorageBasePath(this));
                success = new File(Voice.getDataStorageBasePath(this)).mkdirs();
            }
            catch (Exception e) {
                Log.e(LOG_TAG,"Could not create directory structure. "+e.getMessage());
                success = false;
            }

            if(!success) {
                Log.e(LOG_TAG, "Failed");
                // Can't do anything without appropriate directory structure.
                result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
                setResult(result, returnData);
                finish();
            }
        }

		/* Connect to CMU TTS server and get the list of voices available,
		 * if we don't already have a file.
		 */

        if(!Utility.pathExists(getVoiceFilePath(this))) {
            Log.e(LOG_TAG, "Voice list file doesn't exist. Try getting it from server.");

            DownloadVoiceList(this, null);
        }

		/* At this point, we MUST have a voices.list file. If this file is not there,
		 * possibly because Internet connection was not available, we must create a dummy
		 *
		 */
        if(!Utility.pathExists(getVoiceFilePath(this))) {
            try {
                Log.w(LOG_TAG, "Voice list not found, creating dummy list.");
                BufferedWriter out = new BufferedWriter(new FileWriter(getVoiceFilePath(this)));
                out.write("ltu-LTU-Aiste\t1234567890\n");
                out.write("ltu-LTU-Regina\t1234567890\n");
                out.write("ltu-LTU-Edvardas\t1234567890\n");
                out.write("ltu-LTU-Vladas\t1234567890\n");
                out.close();
            } catch (IOException e) {
                Log.e(LOG_TAG, "Failed to create voice list dummy file.");
                // Can't do anything without that file.
                result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
                setResult(result, returnData);
                finish();
            }
        }
		/* Go through each line in voices.list file and see
		 * if the data for that voice is installed.
		 */

        ArrayList<Voice> voiceList = getVoices(this);
        if (voiceList.isEmpty()) {
            Log.e(LOG_TAG,"Problem reading voices list. This shouldn't happen!");
            result = TextToSpeech.Engine.CHECK_VOICE_DATA_FAIL;
            setResult(result, returnData);
            finish();
        }

        for(Voice vox:voiceList) {
            if(vox.isAvailable()) {
                available.add(vox.getName());
            } else {
                unavailable.add(vox.getName());
            }
        }
        returnData.putStringArrayListExtra("availableVoices", available);
        returnData.putStringArrayListExtra("unavailableVoices", unavailable);
        setResult(result, returnData);
        finish();
    }

    public static String getVoiceFilePath(Context context) {
        return new File(Voice.getDataStorageBasePath(context), VOICE_LIST_FILE_).getPath();
    }

    public static void DownloadVoiceList(Context context, Runnable callback) {
        // Download the voice list and call back to notify of update
        String voiceListURL = Voice.getDownloadURLBasePath() + "voices.list";

        FileDownloader fdload = new FileDownloader();
        fdload.saveUrlAsFile(voiceListURL, getVoiceFilePath(context));
        while(!fdload.finished) {}
        boolean savedVoiceList = fdload.success;

        if(!savedVoiceList)
            Log.w(LOG_TAG,"Could not update voice list from server");
        else
            Log.w(LOG_TAG,"Successfully updated voice list from server");

        if (callback != null) {
            callback.run();
        }

    }

    public static ArrayList<Voice> getVoices(Context context) {
        ArrayList<String> voiceList = null;
        try {
            voiceList = Utility.readLines(getVoiceFilePath(context));
        } catch (IOException e) {
            // Ignore exception, since we will return empty anyway.
        }
        if (voiceList == null) {
            voiceList = new ArrayList<String>();
        }

        ArrayList<Voice> voices = new ArrayList<Voice>();

        for(String strLine:voiceList) {
            Voice vox = new Voice(context, strLine);
            if (!vox.isValid())
                continue;
            voices.add(vox);
        }

        return voices;
    }

    public static ArrayList<Voice> getAvailableVoices(Context context) {
        ArrayList<Voice> voices = new ArrayList<Voice>();

        for(Voice vox:getVoices(context)) {
            if (!vox.isAvailable())
                continue;
            voices.add(vox);
        }

        return voices;
    }

    public static Voice getAnyVoiceAvailable(Context context, String language) {
        for (Voice v : getVoices(context)) {
            if (v.getLanguage().equals(language) && v.isAvailable()) {
                return v;
            }
        }
        return null;
    }

    public static Voice getAnyVoiceAvailable(Context context, String language, String country) {
        for (Voice v : getVoices(context)) {
            if (v.getLanguage().equals(language) && v.getCountry().equals(country) && v.isAvailable())
            {
                return v;
            }
        }
        return null;
    }

    public static boolean isLanguageAvailable(Context context, String language) {
        for (Voice v : getVoices(context)) {
            if (v.getLanguage().equals(language) && v.isAvailable())
            {
                return true;
            }
        }
        return false;
    }
}