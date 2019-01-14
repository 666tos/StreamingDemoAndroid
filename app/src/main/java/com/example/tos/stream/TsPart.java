package com.example.tos.stream;

public class TsPart {

    private final int mFps;
    private final int mIndex;
    private final int mNumberOfFrames;
    private final String mUrl;

    TsPart(final int index, final float duration, final String url) {
        mFps = 30;
        mIndex = index;
        mNumberOfFrames = (int) duration * mFps;
        mUrl = url;
    }

    public int getFps() {
        return mFps;
    }

    public int getIndex() {
        return mIndex;
    }

    public int getNumberOfFrames() {
        return mNumberOfFrames;
    }

    public String getUrl() {
        return mUrl;
    }

}