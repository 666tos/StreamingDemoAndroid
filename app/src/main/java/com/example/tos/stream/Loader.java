package com.example.tos.stream;

import android.os.AsyncTask;

public class Loader {
    AsyncTask<Void, Void, Void> mTask;

    public void load(final String url, final int tag) {
        System.out.println("HI JAVA: tag: " + tag + "url: " + url);

        mTask = new AsyncTask<Void, Void, Void> () {

            @Override
            protected Void doInBackground(Void... params) {
//                try {
////                    jsonString = getJsonFromServer(url);
//                } catch (IOException e) {
//                    // TODO Auto-generated catch block
////                    e.printStackTrace();
//                }
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                super.onPostExecute(result);

//                tv.setText(jsonString);

            }
        };
    }
}
