package tacx.android.streaming.content;

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