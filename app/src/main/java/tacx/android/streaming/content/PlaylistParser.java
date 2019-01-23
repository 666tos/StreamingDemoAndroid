package tacx.android.streaming.content;

import android.net.Uri;
import android.os.AsyncTask;

import com.iheartradio.m3u8.Encoding;
import com.iheartradio.m3u8.Format;
import com.iheartradio.m3u8.data.EncryptionData;
import com.iheartradio.m3u8.data.MasterPlaylist;
import com.iheartradio.m3u8.data.MediaPlaylist;
import com.iheartradio.m3u8.data.Playlist;
import com.iheartradio.m3u8.data.PlaylistData;
import com.iheartradio.m3u8.data.TrackData;

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
        final PlaylistData mediaPlaylistData = findMediaPlaylist(mPlaylistUrl, mTargetBitrate);
        if (mediaPlaylistData == null) {
            return null;
        }

        final String mediaPlaylistUrlSpec = getUrlFromManifest(mPlaylistUrl, mediaPlaylistData.getUri());
        final List<TsPart> tsPartList = getParts(mediaPlaylistUrlSpec);

        return tsPartList;
    }

    private PlaylistData findMediaPlaylist(final String masterPlaylistSpec, final int targetBitrate) {
        final Playlist playlist = getPlaylist(masterPlaylistSpec);
        final MasterPlaylist masterPlaylist = getMasterPlaylist(playlist);
        if (masterPlaylist == null) {
            return null;
        }

        final List<PlaylistData> playlistData = sortPlaylistsByBandwidth(masterPlaylist.getPlaylists());
        PlaylistData mediaPlaylistData = playlistData.get(playlistData.size() - 1);
        for (PlaylistData playlistDataItem: playlistData) {
            if (playlistDataItem.getStreamInfo().getBandwidth() > targetBitrate) {
                mediaPlaylistData = playlistDataItem;

                break;
            }
        }

        return mediaPlaylistData;
    }

    private MasterPlaylist getMasterPlaylist(final Playlist playlist) {
        if (playlist == null || !playlist.hasMasterPlaylist()) {
            return null;
        }

        return playlist.getMasterPlaylist();
    }

    private MediaPlaylist getMediaPlaylist(final Playlist playlist) {
        if (playlist == null || !playlist.hasMediaPlaylist()) {
            return null;
        }

        return playlist.getMediaPlaylist();
    }

    private List<TsPart> getParts(final String mediaPlaylistUrlSpec) {
        final Playlist playlist = getPlaylist(mediaPlaylistUrlSpec);
        final MediaPlaylist mediaPlaylist = getMediaPlaylist(playlist);
        int index = 1;
        final List<TsPart> tsPartList = new ArrayList<>(mediaPlaylist.getTracks().size());
        for (TrackData trackData : mediaPlaylist.getTracks()) {
            EncryptionData encryptionData = trackData.getEncryptionData();

            TsPartKey key = new TsPartKey(encryptionData);

            tsPartList.add(new TsPart(index++, trackData.getTrackInfo().duration, getUrlFromManifest(mediaPlaylistUrlSpec, trackData.getUri()), key));
        }

        return tsPartList;
    }

    private Playlist getPlaylist(final String playlistUrlSpec) {
        if (playlistUrlSpec == null) {
            return null;
        }

        URL playlistUrl = null;
        try {
            playlistUrl = new URL(playlistUrlSpec);
        } catch (MalformedURLException ignore) {
        }

        if (playlistUrl == null) {
            return null;
        }

        Playlist playlist = null;
        try (InputStream inputStream = playlistUrl.openStream()) {
            final com.iheartradio.m3u8.PlaylistParser playlistParser = new com.iheartradio.m3u8.PlaylistParser(inputStream, Format.EXT_M3U, Encoding.UTF_8);
            playlist = playlistParser.parse();
        } catch (Exception ignore) {
        }

        return playlist;
    }

    private String getUrlFromManifest(final String manifest, final String lastPathSegment) {
        final Uri manifestUri = Uri.parse(manifest);
        final String manifestLastPathSegment = manifestUri.getLastPathSegment();
        if (manifestLastPathSegment == null) {
            return null;
        }

        return manifest.replace(manifestLastPathSegment, lastPathSegment);
    }

    private List<PlaylistData> sortPlaylistsByBandwidth(final List<PlaylistData> playlists) {
        final List<PlaylistData> playlistData = new ArrayList<>(playlists);
        Collections.sort(playlistData, new Comparator<PlaylistData>() {

            @Override
            public int compare(final PlaylistData o1, final PlaylistData o2) {
                return o1.getStreamInfo().getBandwidth() - o2.getStreamInfo().getBandwidth();
            }

        });

        return playlistData;
    }

}