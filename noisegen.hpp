#ifndef CLASS_NOISEGEN
#define CLASS_NOISEGEN

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "simplexnoise.h"
#include "perlin.hpp"
#include <SFML\Graphics.hpp>

#include "button.hpp"
#include "slider.hpp"
#include "multislider.hpp"
#include "icon.hpp"


//#define IMAGE_SIZE 256
//#define IMAGE_SCALE 2

#define RIGHT_MARGIN_WIDTH 150
#define BOTTOM_MARGIN_HEIGHT 100



enum {SIMPLEX, PERLIN};

class NoiseGen
{
private:

    float multislidertest;


    //image dimensions (IMAGE_SIZE * IMAGE_SCALE)
    int IMAGE_SIZE;
    int IMAGE_SCALE;

    //init
    void defaultSettings();
    void reloadDefaultSettings();
    void loadSettings();
    void saveSettings();
    void initSliders();
    void initTerrainSlider();
    void initButtons();

    //camera/gen controls
    float persistence;
    float zoom;
    float octaves;
    int xpos;
    int ypos;
    void randomPosition();
    int N_MODE;

    //rendering objects
    sf::RenderWindow *screen;
    sf::RenderTexture *mapTexture;
    sf::Font font;

    //sliders
    std::vector< Slider* > sliders;
    bool updateSliders(sf::Event *event);
    MultiSlider *terSlider;

    //buttons
    std::vector< Button* > buttons;

    //terrain variables
    //std::vector< sf::Color > terraincolors;
    //std::vector<int> paramvalues;
    std::vector<segment> terrainsegs;
    //int simplewater;
    bool terrainmode;
    //bool terrainsimple;

    //drawing functions
    void createMapImage(int nx, int ny);
    void drawMap();
    //void drawPixelTerrain(sf::RenderTarget *target, int x, int y, int shade, int scale, std::vector<int> *params);
    void drawPixelTerrain(sf::RenderTarget *target, int x, int y, int shade, int scale);
    void drawPixel(sf::RenderTarget *target, int x, int y, int shade, int scale);
    void drawMask();
    void drawSliders();
    void drawButtons();
    void drawCoordinates(sf::Vector2i coord);

    //export
    void exportToTXT();
    void exportToIMAGE();

    //mask
    bool maskActive;
    std::vector< std::vector<int> > mask;

    //main loop
    void mainLoop();

public:
    NoiseGen();

};
#endif // CLASS_NOISEGEN
