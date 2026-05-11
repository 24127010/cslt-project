@echo off
setlocal enabledelayedexpansion

set MINGW=mingw64
set SRC=

for %%f in (src\*.cpp) do (
    set SRC=!SRC! %%f
)

%MINGW%\bin\g++.exe !SRC! ^
-Iinclude ^
-Llib ^
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ^
-static-libgcc -static-libstdc++ ^
-o USFighters.exe

pause