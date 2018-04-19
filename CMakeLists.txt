project(winrtmidi)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

add_library(winrtmidi SHARED)
target_sources(winrtmidi PRIVATE
    WinRTMidi/WinRTMidi.h
    WinRTMidi/WinRTMidiImpl.h
    WinRTMidi/WinRTMidiPortWatcher.h
    WinRTMidi/WinRTMidi.cpp
    WinRTMidi/WinRTMidiImpl.cpp
    WinRTMidi/WinRTMidiPortWatcher.cpp
)
target_include_directories(winrtmidi PRIVATE winrtmidi)
set_target_properties(winrtmidi PROPERTIES VS_WINRT_COMPONENT 1)
