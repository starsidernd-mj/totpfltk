# Include directories
#include_directories(${CPPUNIT_INCLUDE_DIR})
#include_directories(/usr/include/cppunit/)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Link the library and the testing executable
add_executable(runUnitTests test_totp_algorithm.cpp)
target_link_libraries(runUnitTests cppunit TOTP OpenSSL::SSL OpenSSL::Crypto)

# Add a testing target
enable_testing()

add_test(NAME runUnitTests COMMAND runUnitTests)

