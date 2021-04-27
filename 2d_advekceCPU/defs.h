#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include <iostream>
#include <omp.h>
#include <vector>
#include <limits>
#include <chrono>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/wait.h>

//oblast:
#define size_x 128
#define size_y 128

#define dw 1.0 //domain width
#define dh 1.0 //domain height

//parametry vypoctu:
#define a 1.0
#define b 2.0

#define initvalue 0
#define initvalueX 1
#define initvalueY -1

//okrajove podminky
#define bcy0 (1.0)
#define bcx0 (-1.0)

#define t_fin 0.25 //nefunguje, zadava se primo v jadru
#define dt 0.0025

#define SQ(x) ((x) * (x))
#define NORM(x, y) sqrt(SQ(x) + SQ(y))

#define POS(x,y,X,Y) (y+(x)*(Y))
#define pos(x,y) (y+(x)*(size_y))

//nastaveni SP, DP
using real = double;

