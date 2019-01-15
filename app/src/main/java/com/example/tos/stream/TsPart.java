package com.example.tos.stream;

public class TsPart {

    private final double mDuration;
    private final int mIndex;
    private final String mUrl;

    TsPart(final int index, final double duration, final String url) {
        mDuration = duration;
        mIndex = index;
        mUrl = url;
    }

    public double getDuration() {
        return mDuration;
    }

    public int getIndex() {
        return mIndex;
    }

    public String getUrl() {
        return mUrl;
    }

}