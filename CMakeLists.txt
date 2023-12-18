cmake_minimum_required(VERSION 3.14)
project(youtube__)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp
        TextBox.cpp
        Button.cpp
        TranscriptStealer.h
        ChatGPTClient.h
        openai.hpp
        json.h
)

add_executable(youtube__ ${SOURCE_FILES})

#
#find_package(cpp-httplib REQUIRED)
#target_link_libraries(youtube__ PRIVATE cpp-httplib::cpp-httplib)
# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(youtube__ PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Find nlohmann_json
find_package(nlohmann_json REQUIRED)
target_link_libraries(youtube__ PRIVATE nlohmann_json::nlohmann_json)

# Find CURL
find_package(CURL REQUIRED)
target_include_directories(youtube__ PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(youtube__ PRIVATE ${CURL_LIBRARIES})
