# Fluid Fun #

Written in C, uses SDL and OpenGL for portable graphics. A fun graphical simulation of various materials using interacting 2d particles. Visit the [HowTo](http://code.google.com/p/fluidfun/wiki/HowTo) page for a quick run-through.

### Videos: ###
  * [Plantation](http://www.youtube.com/watch?v=pKmMtUXwUiE) by xavieran


### To build: ###
In unix terminal, type:
```
svn checkout http://fluidfun.googlecode.com/svn/trunk/ fluidfun
```
Navigate to folder, then type:
```
make
```
Execute program using:
```
./unix_fluid
```
### Required libraries: ###
  * SDL development files
  * SDL\_ttf development files
  * OpenGL: GL and GLU development files

Ubuntu GNU/Linux users can install the above dependencies using:
```
sudo apt-get install libsdl1.2-dev libsdl-ttf2.0-dev libgl1-mesa-dev -libglu1-mesa-dev
```