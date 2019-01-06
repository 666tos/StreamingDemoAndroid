package com.example.tos.stream;

import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;

public class Loader {
    AsyncTask<Void, Void, Void> mTask;
    public StreamingCore mStreamingCore;

    public void load(final String urlString, final int tag) {
        System.out.println("HI JAVA: tag: " + tag + "url: " + urlString);

        mTask = new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    URL url = new URL(urlString);
                    URLConnection conection = url.openConnection();
                    conection.connect();

                    // this will be useful so that you can show a tipical 0-100%
                    // progress bar
                    int lenghtOfFile = conection.getContentLength();

                    // download the file
                    InputStream inputStream = new BufferedInputStream(url.openStream(), 1024 * 1024 * 8);

                    ByteArrayOutputStream buffer = new ByteArrayOutputStream();

                    int nRead;
                    byte[] bufferData = new byte[16384];

                    while ((nRead = inputStream.read(bufferData, 0, bufferData.length)) != -1) {
                        buffer.write(bufferData, 0, nRead);
                    }

                    byte data[] = buffer.toByteArray();

                    // closing streams
                    inputStream.close();

                    mStreamingCore.setData(data, tag);

                } catch (Exception e) {
                    Log.e("Error: ", e.getMessage());
                }

                return null;
            }
        };

        mTask.execute();
    }
}