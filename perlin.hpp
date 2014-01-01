#ifndef PERLIN_H
#define PERLIN_H

#include <cstdlib>
#include <vector>
#include <cmath>

inline double findnoise2(double x,double y);
inline double interpolate(double a,double b,double x);

double noise(double x,double y);

float getNoise(float octaves, float persistence, float zoom, float x, float y);
std::vector< std::vector<int> > genHeightMapV(int w,int h,int nx, int ny, double zoom,double p, int o);

#endif // PERLIN_H
