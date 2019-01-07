# StreamingDemoAndroid

This project demontsrates power of FFMpeg-based streaming engine which is capable of showing video frame-by-frame, which can be used by Tacx Training App for playing video streams on varying speed. It contains:
  - C++ engine compatible with both iOS and Android.

Android-specific features:
  - High-level Java/JNI wrapper for C++ engine, used for setting up video stream for chunks for playlist and getting frames at specific index
  - OpenGL ES as rendering angine

Dependencies:
  - FFMpeg for headers and pre-built libraries
Sources and build scripts can be found here - https://github.com/tanersener/mobile-ffmpeg.git

TODO:

  - Merge this repository with https://github.com/TacxDev/Tacx-streaming.git
