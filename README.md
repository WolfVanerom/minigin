# Prog4Game Engine - Wolf Vanerom

This is a rendition of the original minigin project. It is currently being adapted into a game engine to create a game project later on.

[![Build Status](https://github.com/WolfVanerom/minigin/actions/workflows/cmake.yml/badge.svg)](https://github.com/avadae/cmake/actions)
[![Build Status](https://github.com/WolfVanerom/minigin/actions/workflows/emscripten.yml/badge.svg)](https://github.com/avadae/emscripten/actions)

# Goal

Creating an engine that is sufficent to create the game digger.

## Windows version

Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake 
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.

Or
- Use whatever editor you like :)

## Emscripten (web) version

### On windows

For installing all of the needed tools on Windows I recommend using [Chocolatey](https://chocolatey.org/). You can then run the following in a terminal to install what is needed:

    choco install -y cmake
    choco install -y emscripten
    choco install -y ninja
    choco install -y python

In a terminal, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.

### On OSX

On Mac you can use homebrew

    brew install cmake
    brew install emscripten
    brew install python

In a terminal on OSX, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake .. -DCMAKE_OSX_ARCHITECTURES=""
    emmake make

To be able to see the webpage you can start a python webserver in the build_web folder

    python3 -m http.server

Then browse to http://localhost:8000 and you're good to go.

## Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://avadae.github.io/minigin/). 
  - The url of that page will be `https://<username>.github.io/<repository>/`
- You can embed this page with 

```<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://<username>.github.io/<repository>/" loading="lazy"></iframe>```

