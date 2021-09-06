cmake_minimum_required(VERSION 3.20)
project(voip_client3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package (bson-1.0 1.7 REQUIRED)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

find_package(Boost)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)
SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

if (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)
    # in cache already
    set(PORTAUDIO_FOUND TRUE)
else (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)
    if (NOT WIN32)
        include(FindPkgConfig)
        pkg_check_modules(PORTAUDIO2 portaudio-2.0)
    endif (NOT WIN32)

    if (PORTAUDIO2_FOUND)
        set(PORTAUDIO_INCLUDE_DIRS
                ${PORTAUDIO2_INCLUDE_DIRS}
                )
        if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(PORTAUDIO_LIBRARIES "${PORTAUDIO2_LIBRARY_DIRS}/lib${PORTAUDIO2_LIBRARIES}.dylib")
        else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(PORTAUDIO_LIBRARIES
                    ${PORTAUDIO2_LIBRARIES}
                    )
        endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(PORTAUDIO_VERSION
                19
                )
        set(PORTAUDIO_FOUND TRUE)
    else (PORTAUDIO2_FOUND)
        find_path(PORTAUDIO_INCLUDE_DIR
                NAMES
                portaudio.h
                PATHS
                /usr/include
                /usr/local/include
                /opt/local/include
                /sw/include
                )

        find_library(PORTAUDIO_LIBRARY
                NAMES
                portaudio
                PATHS
                /usr/lib
                /usr/local/lib
                /opt/local/lib
                /sw/lib
                )

        find_path(PORTAUDIO_LIBRARY_DIR
                NAMES
                portaudio
                PATHS
                /usr/lib
                /usr/local/lib
                /opt/local/lib
                /sw/lib
                )

        set(PORTAUDIO_INCLUDE_DIRS
                ${PORTAUDIO_INCLUDE_DIR}
                )
        set(PORTAUDIO_LIBRARIES
                ${PORTAUDIO_LIBRARY}
                )

        set(PORTAUDIO_LIBRARY_DIRS
                ${PORTAUDIO_LIBRARY_DIR}
                )

        set(PORTAUDIO_VERSION
                19
                )

        if (PORTAUDIO_INCLUDE_DIRS AND PORTAUDIO_LIBRARIES)
            set(PORTAUDIO_FOUND TRUE)
        endif (PORTAUDIO_INCLUDE_DIRS AND PORTAUDIO_LIBRARIES)

        if (PORTAUDIO_FOUND)
            if (NOT Portaudio_FIND_QUIETLY)
                message(STATUS "Found Portaudio: ${PORTAUDIO_LIBRARIES}")
            endif (NOT Portaudio_FIND_QUIETLY)
        else (PORTAUDIO_FOUND)
            if (Portaudio_FIND_REQUIRED)
                message(FATAL_ERROR "Could not find Portaudio")
            endif (Portaudio_FIND_REQUIRED)
        endif (PORTAUDIO_FOUND)
    endif (PORTAUDIO2_FOUND)


    # show the PORTAUDIO_INCLUDE_DIRS and PORTAUDIO_LIBRARIES variables only in the advanced view
    mark_as_advanced(PORTAUDIO_INCLUDE_DIRS PORTAUDIO_LIBRARIES)

endif (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS)


find_library(OPUS_LIBRARY NAMES opus libopus)
if(OPUS_LIBRARY)
    message(STATUS "Opus library found: " ${OPUS_LIBRARY})
    #Set header to default include path
    add_definitions(-DOPUS_HEADER="opus/opus.h")
endif()


find_path(OPUS_HEADER_PATH "opus.h" HINTS lib/opus/include)
if(OPUS_HEADER_PATH)
    message(STATUS "Compiling provided opus-library!")
    # Compile ourselves and set correct header-path
    add_subdirectory(lib/opus build/opus)
    add_definitions(-DOPUS_HEADER="../lib/opus/include/opus.h")
endif()
if(NOT OPUS_LIBRARY AND NOT OPUS_HEADER_PATH)
    message(WARNING "No opus library found")
endif()


add_executable(voip_client3 sources/main.cpp
        sources/mainwindow.cpp
        headers/mainwindow.h
        sources/mainwindow.ui
        headers/button.h
        sources/button.cpp
        sources/mainwindow.cpp
        headers/mainwindow.h
        sources/button.cpp
        headers/button.h
        headers/StringList.h
        headers/database_handler.h
        sources/add_contact.cpp
        headers/add_contact.h
        sources/Login.cpp headers/Login.h headers/UdpCall.h headers/SenderUdp.h headers/DelivererUdp.h headers/Serialization.h headers/Audio.h headers/ReceiverUdp.h)

target_link_libraries(voip_client3
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        sqlite3
        mongo::bson_shared
        ${OPUS_LIBRARIES}
        opus
        Boost::boost)

