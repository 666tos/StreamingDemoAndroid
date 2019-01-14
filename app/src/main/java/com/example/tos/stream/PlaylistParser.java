package com.example.tos.stream;

import android.net.Uri;
import android.os.AsyncTask;

import com.iheartradio.m3u8.Encoding;
import com.iheartradio.m3u8.Format;
import com.iheartradio.m3u8.data.MasterPlaylist;
import com.iheartradio.m3u8.data.MediaPlaylist;
import com.iheartradio.m3u8.data.Playlist;
import com.iheartradio.m3u8.data.PlaylistData;
import com.iheartradio.m3u8.data.TrackData;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

class PlaylistParser extends AsyncTask<Void, Void, List<TsPart>> {

    private final String mPlaylistUrl;
    private final int mTargetBitrate;

    PlaylistParser(final String playlistUrl, final int targetBitrate) {
        mPlaylistUrl = playlistUrl;
        mTargetBitrate = targetBitrate;
    }

    @Override
    protected List<TsPart> doInBackground(final Void... params) {
        URL masterPlaylistUrl = null;
        try {
            masterPlaylistUrl = new URL(mPlaylistUrl);
        } catch (MalformedURLException ignore) {
        }

        if (masterPlaylistUrl == null) {
            return null;
        }

        InputStream inputStream = null;
        try {
            inputStream = masterPlaylistUrl.openStream();
        } catch (IOException ignore) {
        }

        if (inputStream == null) {
            return null;
        }

        final com.iheartradio.m3u8.PlaylistParser masterPlaylistParser = new com.iheartradio.m3u8.PlaylistParser(inputStream, Format.EXT_M3U, Encoding.UTF_8);
        Playlist playlist = null;
        try {
            playlist = masterPlaylistParser.parse();
        } catch (Exception ignore) {
        }

        if (playlist == null || !playlist.hasMasterPlaylist()) {
            return null;
        }

        final MasterPlaylist masterPlaylist = playlist.getMasterPlaylist();
        final List<PlaylistData> playlistData = new ArrayList<>(masterPlaylist.getPlaylists());
        Collections.sort(playlistData, new Comparator<PlaylistData>() {

            @Override
            public int compare(final PlaylistData o1, final PlaylistData o2) {
                return o1.getStreamInfo().getBandwidth() - o2.getStreamInfo().getBandwidth();
            }

        });

        PlaylistData mediaPlaylistData = null;
        for (PlaylistData playlistDataItem: playlistData) {
            if (playlistDataItem.getStreamInfo().getBandwidth() > mTargetBitrate) {
                mediaPlaylistData = playlistDataItem;

                break;
            }
        }

        if (mediaPlaylistData == null) {
            return null;
        }

        try {
            inputStream.close();
            inputStream = null;
        } catch (IOException ignore) {
        }

        URL mediaPlaylistUrl = null;
        try {
            final String url = masterPlaylistUrl.toString();
            final Uri uri = Uri.parse(url);
            mediaPlaylistUrl = new URL(url.replace(uri.getLastPathSegment(), "") + mediaPlaylistData.getUri());
        } catch (MalformedURLException ignore) {
        }

        if (mediaPlaylistUrl == null) {
            return null;
        }

        try {
            inputStream = mediaPlaylistUrl.openStream();
        } catch (IOException ignore) {
        }

        if (inputStream == null) {
            return null;
        }

        final com.iheartradio.m3u8.PlaylistParser mediaPlaylistParser = new com.iheartradio.m3u8.PlaylistParser(inputStream, Format.EXT_M3U, Encoding.UTF_8);
        try {
            playlist = mediaPlaylistParser.parse();
        } catch (Exception ignore) {
        }

        if (playlist == null || !playlist.hasMediaPlaylist()) {
            return null;
        }

        final MediaPlaylist mediaPlaylist = playlist.getMediaPlaylist();
        int index = 1;
        final List<TsPart> tsPartList = new ArrayList<>(mediaPlaylist.getTracks().size());
        for (TrackData trackData : mediaPlaylist.getTracks()) {
            final String url = mediaPlaylistUrl.toString();
            final Uri uri = Uri.parse(url);
            tsPartList.add(new TsPart(index++, trackData.getTrackInfo().duration, url.replace(uri.getLastPathSegment(), trackData.getUri())));
        }

        return tsPartList;
    }

}