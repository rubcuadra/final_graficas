#!/usr/local/bin/fish
#brew install png

cd loader
gcc -DHAVE_CONFIG_H -g -O2 -c *.c
ar cru libglm.a *.o
mv libglm.a ..
cd ..
g++ -DHAVE_CONFIG_H -g -O2 -I/usr/local/include -c -std=c++11 *.cpp
g++ *.o libglm.a -L/usr/local/lib -framework OpenGL -framework GLUT -ljpeg -lpng -lIL -lm
./a.out
rm loader/*.o
rm a.out