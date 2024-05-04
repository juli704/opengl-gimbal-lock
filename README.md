# OpenGL Gimbal Lock

![opengl-gimbal-lock-demo.gif](demo/opengl-gimbal-lock-demo.gif)

A graphical user interface for controlling the rings of a gimbal lock system

Tech stack:
- C++
- OpenGL 3.3
- Qt 6.7.0 (open source version)
- Qt Creator (Qmake) 13.0.0

# Getting started

## Install Qt Creator

Download the online installer for Qt Creator from [here](https://www.qt.io/download-qt-installer-oss?utm_referrer=https%3A%2F%2Fwww.qt.io%2F) and follow the installation dialog

## Install further dependencies

Install [Assimp](https://github.com/assimp/assimp) and [OpenGL](https://www.opengl.org/):

`sudo apt update -y && sudo apt install -y libassimp-dev libgl-dev`

## Compile and launch

Open this project in Qt Creator and launch it by clicking on the green play button located on the lower left of the GUI

# Known issues

The following issues were noticed during development using Ubuntu 22.04.4 LTS. Please feel free to add further issues here or fixing them directly via pull requests.

- Application crashes in release mode as soon as the first gl function is called
- Rings get bigger when increasing the window's size vertically
