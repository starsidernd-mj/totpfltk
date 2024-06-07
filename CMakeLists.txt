cmake_minimum_required(VERSION 3.0)
project(totpfltk)

# Find ncurses

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add your source files
set(SOURCES
    src/InputWindow.cpp
    src/Timer.cpp
    src/TOTP.cpp
    src/TotpTable.cpp
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
target_link_libraries(totpcmd OpenSSL::SSL OpenSSL::Crypto)

