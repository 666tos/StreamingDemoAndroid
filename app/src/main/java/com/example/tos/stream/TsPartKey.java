package com.example.tos.stream;

import com.iheartradio.m3u8.data.EncryptionData;

import java.util.Iterator;
import java.util.List;

public class TsPartKey {
    private final String mMethod;
    private final String mUri;
    private final byte[] mIV;
    private final String mKeyFormat;
    private final String mKeyFormatVersions;

    TsPartKey(EncryptionData encryptionData) {
        mMethod = encryptionData.getMethod().getValue();
        mUri = encryptionData.getUri();
        mIV = bytesListToByteArray(encryptionData.getInitializationVector());
        mKeyFormat = encryptionData.getKeyFormat();
        mKeyFormatVersions = integerArrayToString(encryptionData.getKeyFormatVersions());
    }

    public String getMethod() {
        return mMethod;
    }
    public String getUri() {
        return mUri;
    }
    public byte[] getIV() {
        return mIV;
    }
    public String getKeyFormat() {
        return mKeyFormat;
    }
    public String getKeyFormatVersions() {
        return mKeyFormatVersions;
    }

    private byte[] bytesListToByteArray(List<Byte> bytesList) {
        int n = bytesList.size();

        byte[] out = new byte[16];

        if (bytesList.size() == 32) {
            /**
             * Fix bug in playlist parser parsing IV string, for example "0X11223344..."
             * Expected result: [0x11, 0x22, 0x33, 0x44...]
             * Actual result:   [0x01, 0x01, 0x02, 0x02, 0x03, 0x03...]
             * String is parsed symbol by symbol, rather then by 2 symbols (2 hex symbols represent 1 byte)
             */
            for (int i = 0; i < 16; i++) {
                byte high = bytesList.get(i * 2);
                byte low = bytesList.get(i * 2 + 1);
                out[i] = (byte) (high * 16 + low);
            }
        }
        else if (bytesList.size() == 16) {
            for (int i = 0; i < n; i++) {
                out[i] = bytesList.get(i);
            }
        }

        return out;

    }

    private String integerArrayToString(List<Integer> array) {
        StringBuilder stringBuilder = new StringBuilder();
        Iterator<Integer> iterator = array.iterator();

        while(iterator.hasNext())  {
            stringBuilder.append(iterator.next());
            if(iterator.hasNext()){
                stringBuilder.append(",");
            }
        }

        return stringBuilder.toString();
    }
}
