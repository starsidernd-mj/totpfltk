# totpfltk

totpfltk is an FLTK-based application that allows the user to add and observe TOTP passcodes.
I got tired of having to always pull out my phone and look up these passcodes, especially when I was in a different room.
It became more convenient to create my own application that allowed me the same usability as popular Android apps while maintaining security.

![Screenshot missing](https://github.com/starsidernd-mj/totpfltk/blob/master/screenshot.png "Main screen")

## Building

I have included the CMakeLists.txt script for building it yourself.
You will likely need to install libssl and fltk for it to build.

```
mkdir build
cd build
cmake ..
make
```

## Installing

Installing the application will copy over the icon, desktop file, execution script, and key file. Since it controls sensitive information, it should be no surprise that sudo is required.

```
sudo make install
```
