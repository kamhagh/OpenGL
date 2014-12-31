#!/bin/bash

echo "Enter programname.cpp: "
read programname         
echo "Compiling it.."
g++ -c -g $programname.cpp -std=c++11
g++ $programname.o -o $programname.exec -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lSOIL -lassimp
echo "Done, executing..."
./$programname.exec

