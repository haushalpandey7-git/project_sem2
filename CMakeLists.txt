cmake_minimum_required(VERSION 3.16)

project(AIFolderManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(AIFolderManager
    src/main.cpp
    src/FileScanner.cpp
    src/FileClassifier.cpp
    src/AIClassifier.cpp
    src/FolderManager.cpp
    src/Logger.cpp
    
)

target_include_directories(AIFolderManager PRIVATE
    include
    D:/msys64/ucrt64/include
)

target_link_directories(AIFolderManager PRIVATE
    D:/msys64/ucrt64/lib
)

target_link_libraries(AIFolderManager PRIVATE
    
    curl
)


add_executable(JsonTest
    src/json_test.cpp
)

target_include_directories(JsonTest PRIVATE
    include
)