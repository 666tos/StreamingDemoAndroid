package tacx.android.streaming.content;

public interface StateDelegate {

    enum StreamState {
        Unknown,
        Stopped,
        Buffering,
        Ready,
        Error;

        public static StreamState fromInt(int value) {
            switch (value) {
                case 0:     return Unknown;
                case 1:     return Stopped;
                case 2:     return Buffering;
                case 3:     return Ready;
                case 4:     return Error;
            }
            return null;
        }
    }

    void stateChanged(int state);

}