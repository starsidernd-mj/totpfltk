cmake_minimum_required(VERSION 3.0)
project(totpfltk)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

#Find FLTK
find_package(FLTK REQUIRED)

# Find X11 (required for FLTK on Linux)
find_package(X11 REQUIRED)

# Add your source files
set(SOURCES
    src/Entry.cpp
    src/FileHandler.cpp
    src/InputWindow.cpp
    src/Timer.cpp
    src/TotpTable.cpp
    src/TOTP.cpp
    src/DeleteWindow.cpp
    src/ModifyWindow.cpp
    main.cpp
    # Add more source files here if needed
)
add_subdirectory(src)
add_subdirectory(tests)

if(WIN32)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif()

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-comment -Wno-reorder -Wno-sign-compare -Wno-address -Wno-unused-variable -Wno-dev")

# Include source headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create executable
add_executable(totpfltk ${SOURCES})

# Link libraries
target_link_libraries(totpfltk fltk ${X11_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)

# Installation paths
set(CMAKE_INSTALL_PREFIX /usr/local)
set(TOTPFLTK_ETC_DIR /etc/totpfltk)

# Install the binary
install(TARGETS totpfltk RUNTIME DESTINATION bin)

# Define installation paths
set(ICON_DESTINATION share/icons/hicolor/256x256/apps)
set(DESKTOP_DESTINATION share/applications)

# Install the icon
install(FILES icon_256x256.png DESTINATION ${ICON_DESTINATION} RENAME totpfltk.png)

# Install the desktop entry
install(FILES totpfltk.desktop DESTINATION ${DESKTOP_DESTINATION})

# Install pkexec policy
install(FILES org.pkexec.totpfltk.policy DESTINATION /usr/share/polkit-1/actions/)

# Install pkexec script to run totpfltk as sudo
install(PROGRAMS totpfltk-pkexec DESTINATION /usr/local/bin)

# Create /etc/totpfltk and create keys
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TOTPFLTK_ETC_DIR})")
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${TOTPFLTK_ETC_DIR}/keys)")
install(CODE "execute_process(COMMAND /bin/chmod 600 ${TOTPFLTK_ETC_DIR}/keys)")

