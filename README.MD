# My First Project with Raylib in C++ (Linux)

This project is a basic example of creating and deploying a native window in **Linux** using **C++** and the **Raylib** graphics library.

---

## 📋 Prerequisites

Make sure you have a C++ compiler (`g++`), `cmake`, and the necessary system dependencies installed.


```bash
sudo apt update
sudo apt install build-essential cmake git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```
## Download graphic library
```
git clone --depth 1 [https://github.com/raysan5/raylib.git](https://github.com/raysan5/raylib.git) raylib
cd raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
sudo make install
sudo ldconfig
```
## Compilation
```
g++ main.cpp -o my_program -lraylibe
```