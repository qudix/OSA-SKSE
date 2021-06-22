@echo off
cd %~dp0..\\
git submodule update --init --recursive
cmake -B "build" -S "."
pause