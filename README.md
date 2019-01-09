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

  - Investigate memcpy vs bioinic-memcpy performance
  - Investigate VulkanAPI capabilities for no-copy texture rendering similar to Metal implementation in iOS
  https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/usage_patterns.html
  - Investigate Hardware Acceleration
  https://trac.ffmpeg.org/wiki/HWAccelIntro
  - Merge this repository with https://github.com/TacxDev/Tacx-streaming.git
  
Links:
  - Best JNI practices from IBM: https://www.ibm.com/developerworks/library/j-jni/index.html
