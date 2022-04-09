# What is SimpleGameEngine
- Demo source code for C++ tutorial purpose

## How to Build 

### Build on Windows
```
install_vcpkg_packages.bat
gen_vs2022.bat
```
- Open Visual Studio project in `build/SimpleGameEngine-x64-windows`

### Build on Mac
```
sh install_vcpkg_packages.sh
sh gen_xcode.sh
```
- Open Xcode project in `build/SimpleGameEngine-xcode`

### Build on Ubuntu Linux
```
sh install_ubunta_packages.sh
sh install_vcpkg_packages.sh
make
make run
```

### Visual Studio Code + Cmake Tool (Works on Win/Mac/Linux)
- install vscode extension `CMake Tools` and `CMake`
- Build and Run from bottom status bar
