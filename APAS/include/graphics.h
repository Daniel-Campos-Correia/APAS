#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

//using namespace std;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void wait_for_key ();
int plotGrafics(int number, vector<string> xValue, vector<string> yValue, vector<string> x2Value, vector<string> y2Value, string legendX, string legendY1, string legendY2, string title);

#endif // GRAPHICS_H
