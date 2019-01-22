package com.example.tos.stream;

import com.iheartradio.m3u8.data.EncryptionData;
import com.iheartradio.m3u8.data.EncryptionMethod;

import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.stream.Collectors;

public class TsPart {
    private final double mDuration;
    private final int mIndex;
    private final String mUrl;
    private final TsPartKey mEncryptionKey;

    TsPart(final int index, final double duration, final String url, TsPartKey encryptionKey) {
        mDuration = duration;
        mIndex = index;
        mUrl = url;
        mEncryptionKey = encryptionKey;
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

    public TsPartKey getEncryptionKey() {
        return mEncryptionKey;
    }
}