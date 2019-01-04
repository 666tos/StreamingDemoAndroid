cmake_minimum_required(VERSION 3.4)
project(TacxStreaming LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES SOURCES "src/main/cpp/*.cpp"
        )

add_library(TacxStreaming
        SHARED
        ${SOURCES}
        )

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "src/main/cpp/*.h" "src/main/cpp/*.hpp")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(HEADERS)
include_directories(${HEADERS} "src/main/jniLibs/include/")
link_directories("src/main/jniLibs")

set(ffmpeg_libs libavcodec;libavdevice;libavfilter;libavformat;libavutil;libswresample;libswscale;libc++_shared)

foreach(ffmpeg_lib ${ffmpeg_libs})
    add_library(${ffmpeg_lib} SHARED IMPORTED)
    set_target_properties(${ffmpeg_lib} PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/${ffmpeg_lib}.so)
endforeach()

target_link_libraries(TacxStreaming
        PRIVATE
        android
        log
        ${ffmpeg_libs}
        )