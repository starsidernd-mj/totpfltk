cmake_minimum_required(VERSION 3.0)
project(totpfltk)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

#Find FLTK
find_package(FLTK REQUIRED)

# Add your source files
set(SOURCES
    src/InputWindow.cpp
    src/Timer.cpp
    src/TotpTable.cpp
    src/TOTP.cpp
    main.cpp
    # Add more source files here if needed
)

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-comment")

# Include source headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create executable
add_executable(totpfltk ${SOURCES})

# Link ncurses
target_link_libraries(totpfltk OpenSSL::SSL OpenSSL::Crypto)

