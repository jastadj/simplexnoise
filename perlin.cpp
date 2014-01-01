#include "perlin.hpp"


inline double findnoise2(double x,double y)
{
 int n=(int)x+(int)y*57;
 n=(n<<13)^n;
 int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
 //int nn=(n*(n*n*60493+19990303)+1376312589);
 return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x)
{
 double ft=x * 3.1415927;
 double f=(1.0-cos(ft))* 0.5;
 return a*(1.0-f)+b*f;
}

double noise(double x,double y)
{
 double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
 double floory=(double)((int)y);
 double s,t,u,v;//Integer declaration
 s=findnoise2(floorx,floory);
 t=findnoise2(floorx+1,floory);
 u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
 v=findnoise2(floorx+1,floory+1);
 double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
 double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
 return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

//w, h, and zoom are obvious
// p = persistence; the roughness of the noise
// o = octaves

float getNoise(float octaves, float persistence, float zoom, float x, float y)
{
    //int octaves=o; //default : 2

    double getnoise = 0;

    for(int a=0; a < octaves-1; a++)//This loops trough the octaves.
    {
        //double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
        double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
        double amplitude = pow(persistence,a);//This decreases the amplitude with every loop of the octave.

        //getnoise += noise(((double)x+abs(nx))*frequency/zoom,((double)y+abs(ny))/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
        getnoise += noise(((double)x)*frequency/zoom,((double)y)/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
    }//											It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1

    int color= (int)((getnoise*128.0)+128.0);//Convert to 0-256 values.

    if(color>255) color=255;

    if(color<0) color=0;

    return color;
}

std::vector< std::vector<int> > genHeightMapV(int w,int h, int nx, int ny, double zoom,double p, int o)//w and h speak for themselves, zoom wel zoom in and out on it, I usually
{//										   use 75. P stands for persistence, this controls the roughness of the picture, i use 1/2

    std::vector< std::vector<int> > heightMap;

    int octaves=o; //default : 2

    //format vector for width/height to create 2d array
    heightMap.resize(h);
    for(int i = 0; i < h; i++) heightMap[i].resize(w);


    for(int y=0; y < h; y++)
    {
        for(int x=0; x < w; x++)
        {
            double getnoise =0;

            for(int a=0; a < octaves-1; a++)//This loops trough the octaves.
            {
                //double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
                double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
                double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.

                //getnoise += noise(((double)x+abs(nx))*frequency/zoom,((double)y+abs(ny))/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
                getnoise += noise(((double)x + nx)*frequency/zoom,((double)y + ny)/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
            }//											It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1

            int color= (int)((getnoise*128.0)+128.0);//Convert to 0-256 values.

            if(color>255) color=255;

            if(color<0) color=0;

            heightMap[y][x] = color;
            //SetPixel(ret,x,y,(int)((r/255.0)*(double)color),(int)((g/255.0)*(double)color),(int)((b/255.0)*(double)color));//This colours the image with the RGB values
      }//														   given at the beginning in the function.
 }

 return heightMap;
}

