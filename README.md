# **GolfGL**
Cross-Platform Golf game in OpenGL. Faculty project for course Computer Graphics. <br>
Still in early stage.
------------------------------
# **Build**
> ### **Windows** 
> - Prerequisites: 
>     - [cmake](https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-windows-x86_64.msi)  binaries
>     - [Visual Studio](https://visualstudio.microsoft.com/downloads/#)
> ```
> > cmake -P configure.cmake  
> > msbuild build\GolfGL.sln
> > cd bin/GolfGL/$[Debug | Release]
> > GolfGL.exe
> ```
> ---
> ### **Linux (Debian based)**
> - Prerequisites:
> ```
> $ sudo apt install g++ cmake git build-essential libgl1-mesa-dev libsoil-dev libglm-dev libassimp-dev libglew-dev > libglfw3-dev libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev mesa-utils libxxf86vm-dev libfreetype6-dev
> ```
> ```
> $ cmake -P configure.cmake
> $ make -C build
> $ cd bin/GolfGL && ./GolfGL
> ```
> <br>
---------------------------
## **Commands**

| ***Key*** | ***Action*** |
| ----------- | ----------- |
| ***Camera Movement*** |
| W | Forward |
| S | Back |
| A | Left |
| D | Right |
| ***Ball*** |
| Space | Shoot |
|  ***General***  |
| Esc | Exit |