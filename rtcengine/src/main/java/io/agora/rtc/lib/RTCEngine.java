package io.agora.rtc.lib;

/**
 * Created by eaglewangy on 04/08/2017.
 */

public class RTCEngine {

    static {
        System.loadLibrary("rtcengine");
    }

    public static native String getVersion();
}
