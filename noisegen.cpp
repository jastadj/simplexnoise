#include "noisegen.hpp"

NoiseGen::NoiseGen()
{

    //init seed
    srand( time(NULL) );

    IMAGE_SIZE = 256;
    IMAGE_SCALE = 2;

    terrainmode = false;
    //paramvalues.resize(9);
    terSlider = NULL;

    //load settings
    defaultSettings(); // load hardcoded defaults first
    loadSettings(); // load settings file, if one is not found, create one

    //init screen
    screen = new sf::RenderWindow( sf::VideoMode(IMAGE_SIZE*IMAGE_SCALE+RIGHT_MARGIN_WIDTH,IMAGE_SIZE*IMAGE_SCALE+BOTTOM_MARGIN_HEIGHT,32), "Simplex Noise");
    screen->setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
    mapTexture = new sf::RenderTexture;
    mapTexture->create(IMAGE_SCALE*IMAGE_SIZE, IMAGE_SCALE*IMAGE_SIZE);

    //load font
    font.loadFromFile("font.ttf");



    //init terrain colors
    /*
    terraincolors.push_back(sf::Color(0,0,120)); // deep water
    terraincolors.push_back(sf::Color(0,0,220)); // shallow water
    terraincolors.push_back(sf::Color(200,200,0)); // sand
    terraincolors.push_back(sf::Color(200,120,50)); // dirt
    terraincolors.push_back(sf::Color(0,220,0)); // low grass
    terraincolors.push_back(sf::Color(0,120,0)); // high grass
    terraincolors.push_back(sf::Color(120,120,120)); // low mtn
    terraincolors.push_back(sf::Color(200,200,200)); // high mtn
    terraincolors.push_back(sf::Color(245,245,245)); // mtn tops
    */

    //other object inits
    initSliders();
    initTerrainSlider();
    initButtons();

    //start main loop
    mainLoop();
}

///////////////////////////////////////////////////////////////////////////
//      INIT
void NoiseGen::loadSettings()
{
    std::ifstream lfile;

    lfile.open("settings.txt");

    //if file does not exist, save current settings to file
    if(!lfile.is_open())
    {
        saveSettings();
        return;
    }

    //clear terrain parameters
    terrainsegs.clear();


    while(!lfile.eof())
    {
        //create line buffer storage and load line into buffer
        char buf[200];
        lfile.getline(buf,200);

        //if reading a line that is for comments, a return line, a space, or anything like that , just ignore currently
        //read line
        if(buf[0] == '#' || buf[0] == '\n' || buf[0] == ' ' || buf[0] == '\0') continue;


        //create strings to parse line commands and parameters
        //NOTE : a command starts with :
        std::string bstring(buf);
        std::string cmd( bstring.substr(0, bstring.find_first_of(':')) );
        std::string param( bstring.substr(bstring.find_first_of(':')+1, bstring.size()-1));

        //get commands, then check parameters for given command
        if(cmd == "IMAGE_SIZE") IMAGE_SIZE = atoi(param.c_str());
        else if(cmd == "IMAGE_SCALE") IMAGE_SCALE = atoi(param.c_str());
        else if(cmd == "PERSISTENCE") persistence = atof(param.c_str());
        else if(cmd == "OCTAVES") octaves = atoi(param.c_str());
        else if(cmd == "ZOOM") zoom = atof(param.c_str());
        else if(cmd == "X_POS") xpos = atoi(param.c_str());
        else if(cmd == "Y_POS") ypos = atoi(param.c_str());
        else if(cmd == "NOISE_MODE")
        {
            if(param == "SIMPLEX") N_MODE = SIMPLEX;
            else if(param == "PERLIN") N_MODE = PERLIN;
            else N_MODE = 0;
        }
        else if(cmd == "TERRAIN_MODE")
        {
            if(param == "ON") terrainmode = true;
            else terrainmode = false;
        }
        else if(cmd == "TERRAIN_SEG_VAL")
        {

            segment newseg;
            newseg.value = atoi(param.c_str());
            terrainsegs.push_back(newseg);


        }
        else if(cmd == "TERRAIN_SEG_COLOR")
        {

            //int it = 0;
            std::string red;
            std::string green;
            std::string blue;


            red = param.substr(0, param.find(','));
            param.erase(0, param.find(',')+1 );
            green = param.substr(0, param.find(','));
            param.erase(0, param.find(',')+1);
            blue = param;

            std::cout << "RGB = " << red << "," << green << "," << blue << std::endl;

            terrainsegs.back().red = atoi(red.c_str());
            terrainsegs.back().green = atoi(green.c_str());
            terrainsegs.back().blue = atoi(blue.c_str());



        }



    }



}

void NoiseGen::saveSettings()
{
    std::ofstream sfile;

    sfile.open("settings.txt");

    //if unable to save settings file
    if(!sfile.is_open())
    {
        std::cout << "Error saving to settings.txt" << std::endl;
        return;
    }

    //save settings
    sfile << "IMAGE_SIZE:" << IMAGE_SIZE << std::endl;
    sfile << "IMAGE_SCALE:" << IMAGE_SCALE << std::endl;
    sfile << std::endl << "PERSISTENCE:" << persistence << std::endl;
    sfile << "ZOOM:" << zoom << std::endl;
    sfile << "OCTAVES:" << octaves << std::endl;
    sfile << "X_POS:" << xpos << std::endl;
    sfile << "Y_POS:" << ypos << std::endl;
    sfile << "NOISE_MODE:";
        if(N_MODE == SIMPLEX) sfile << "SIMPLEX" << std::endl;
        else if(N_MODE == PERLIN) sfile << "PERLIN" << std::endl;
        else sfile << "UNK" << std::endl;
    sfile << "TERRAIN_MODE:";
        if(terrainmode) sfile << "ON" << std::endl;
        else sfile << "OFF" << std::endl;
    sfile << std::endl << "#terrain levels" << std::endl;

    for(int i = 0; i < int(terrainsegs.size()); i++)
    {
        sfile << "TERRAIN_SEG_VAL:" << terrainsegs[i].value << std::endl;
        sfile << "TERRAIN_SEG_COLOR:" << terrainsegs[i].red << "," << terrainsegs[i].green << "," << terrainsegs[i].blue << std::endl;
        sf::Color test;
    }

    /*
    for(int i = 0; i < int(paramvalues.size()); i++)
    {
        sfile << "TERRAIN_SEG:" << paramvalues[i] << std::endl;
    }
    */
    sfile.close();


}


void NoiseGen::defaultSettings()
{
    persistence = 0.7;
    zoom = 0.005;
    octaves = 6;
    xpos = 0;
    ypos = 0;

    N_MODE = SIMPLEX;

    //simple water default
    //simplewater = 128;
    //terrainsimple = false;

    //configure terrain values
    /*
    paramvalues.clear();
    paramvalues.resize(9);
    paramvalues[0] = 90;
    paramvalues[1] = 128;
    paramvalues[2] = 140;
    paramvalues[3] = 150;
    paramvalues[4] = 180;
    paramvalues[5] = 200;
    paramvalues[6] = 220;
    paramvalues[7] = 250;
    paramvalues[8] = 255;
    */

    //clear and populate terrain segments
    terrainsegs.clear();
    segment *newseg = new segment;
    newseg->value = 90;
    newseg->red = 0;
    newseg->green = 0;
    newseg->blue = 120;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 128;
    newseg->red = 0;
    newseg->green = 0;
    newseg->blue = 220;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 140;
    newseg->red = 200;
    newseg->green = 200;
    newseg->blue = 0;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 150;
    newseg->red = 200;
    newseg->green = 120;
    newseg->blue = 50;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 180;
    newseg->red = 0;
    newseg->green = 220;
    newseg->blue = 0;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 200;
    newseg->red = 0;
    newseg->green = 120;
    newseg->blue = 0;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 220;
    newseg->red = 120;
    newseg->green = 120;
    newseg->blue = 120;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 250;
    newseg->red = 200;
    newseg->green = 200;
    newseg->blue = 200;
    terrainsegs.push_back(*newseg);
    delete newseg;

    newseg = new segment;
    newseg->value = 255;
    newseg->red = 245;
    newseg->green = 245;
    newseg->blue = 245;
    terrainsegs.push_back(*newseg);
    delete newseg;


}

void NoiseGen::reloadDefaultSettings()
{
    defaultSettings();
    std::cout << "loaded default settings\n";
    initTerrainSlider(); // memory leak?
    std::cout << "initialized terrain slider\n";
    updateSliders(NULL);
    std::cout << "updated sliders\n";
    //refresh = true;
}

//////////////////////////////////////////////////////////////////////////////
//      EXPORT FUNCTIONS

void NoiseGen::exportToTXT()
{
    std::ofstream ofile;

    ofile.open("export.txt");

    //create vector to story array heightmap in
    std::vector< std::vector<int> > height_map;

    //set vector dimensions
    height_map.resize(IMAGE_SIZE);
    for(int i = 0; i < IMAGE_SIZE; i++) height_map[i].resize(IMAGE_SIZE);

    //get noise for heightmap
    for(int i = 0; i < IMAGE_SIZE; i++)
    {
        for(int n = 0; n < IMAGE_SIZE; n++)
        {
            //these were copied and modified from the createmap function
            if(N_MODE == SIMPLEX) height_map[i][n] = scaled_octave_noise_2d(octaves,persistence,zoom,0,255,n + xpos,i + ypos);
            else if(N_MODE == PERLIN) height_map[i][n] = getNoise(octaves, persistence, zoom, n + xpos, i + ypos);

            if(n == IMAGE_SIZE-1) ofile << height_map[i][n] << std::endl;
            ofile << height_map[i][n] << ",";

        }
    }

    std::cout << "height map exported to export.txt\n";

    ofile.close();

}

void NoiseGen::exportToIMAGE()
{
    screen->clear();
    screen->display();

    sf::Image mapExport = mapTexture->getTexture().copyToImage();


    mapExport.saveToFile("export.bmp");

}


//////////////////////////////////////////////////////////////////////////////
//      MAIN LOOP

void NoiseGen::mainLoop()
{
    bool quit = false;
    bool refresh = true;
    //int cursor = 0;
    //bool viewcontrol = false;
//    int viewcontrolunits = 12; // how many units moved when in view control
    bool mapDrag = false;
    sf::Vector2i mapDragOffset;


    //debug
    //std::cout << "SCREEN :" << IMAGE_SIZE*IMAGE_SCALE+RIGHT_MARGIN_WIDTH << "," << IMAGE_SIZE*IMAGE_SCALE+BOTTOM_MARGIN_HEIGHT << std::endl;

   while(!quit)
    {

        sf::Event event;

        screen->clear();

        //check for right mouse button release if dragging
        if(mapDrag && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            mapDrag = false;
            xpos = xpos - (sf::Mouse::getPosition(*screen).x - mapDragOffset.x);
            ypos = ypos + sf::Mouse::getPosition(*screen).y - mapDragOffset.y;

            refresh = true;
        }
        else if(mapDrag) refresh = true;

        //create, populate, and display map image
        if(refresh)
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && mapDrag)
            {
                int x_off = sf::Mouse::getPosition(*screen).x - mapDragOffset.x;
                int y_off = sf::Mouse::getPosition(*screen).y - mapDragOffset.y;

                createMapImage(xpos - x_off, ypos + y_off);

            }
            else createMapImage(xpos, ypos);
            refresh = false;
        }

        //DRAW OBJECTS
        drawMap();
        drawSliders();
        drawButtons();
        drawCoordinates( sf::Vector2i(IMAGE_SCALE*IMAGE_SIZE + 12, 300));

        //debug draw mouse coordinates
        /*
        std::stringstream mpos;
        mpos << "(" << sf::Mouse::getPosition(*screen).x << "," << sf::Mouse::getPosition(*screen).y << ")";
        sf::Text mposTXT(mpos.str(), font, 12);
        mposTXT.setColor(sf::Color(200,0,0));
        screen->draw(mposTXT);
        */

        //handle input
        while(screen->pollEvent(event))
        {
            //if resize event happened
            if(event.type == sf::Event::Resized)
            {
                screen->setSize( sf::Vector2u(IMAGE_SCALE*IMAGE_SIZE + RIGHT_MARGIN_WIDTH, IMAGE_SCALE*IMAGE_SIZE + BOTTOM_MARGIN_HEIGHT));
                continue;
            }

            //pass event to sliders
            if(updateSliders(&event)) refresh = true;


            //WINDOW CLOSE EVENT
            if(event.type == sf::Event::Closed) quit = true;

            // MOUSE EVENTS
            if(event.type == sf::Event::MouseButtonPressed)
            {
                //LEFT MOUSE BUTTON
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    for(int i = 0; i < int(buttons.size()); i++)
                    {
                        if(buttons[i]->mouseIn())
                        {
                            switch(i)
                            {
                            case 0:
                                N_MODE = SIMPLEX;
                                break;
                            case 1:
                                N_MODE = PERLIN;
                                break;
                            case 2:
                                if(terrainmode) terrainmode = false;
                                else terrainmode = true;
                                break;
                            case 3:
                                exportToTXT();
                                break;
                            case 4:
                                exportToIMAGE();
                                break;
                            case 5:
                                randomPosition();
                                break;
                            case 6:
                                /*
                                defaultSettings();
                                updateSliders(NULL);
                                refresh = true;
                                */
                                reloadDefaultSettings();
                                break;
                            default:
                                break;
                            }

                            refresh = true;

                        }
                    }

                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i m_pos = sf::Mouse::getPosition(*screen);

                    if(m_pos.x >= 0 && m_pos.x <= mapTexture->getSize().x && m_pos.y >= 0 && m_pos.y <= mapTexture->getSize().y)
                    {
                        mapDrag = true;
                        mapDragOffset = m_pos;
                        refresh = true;
                    }

                }

            }

            // KEYBOARD EVENTS
            if(event.type == sf::Event::KeyPressed)
            {
                //escape to quit
                if(event.key.code == sf::Keyboard::Escape)
                {
                    quit = true;
                }
                else if(event.key.code == sf::Keyboard::D)
                {
                    /*
                    defaultSettings();
                    std::cout << "loaded default settings\n";
                    initTerrainSlider(); // memory leak?
                    std::cout << "initialized terrain slider\n";
                    updateSliders(NULL);
                    std::cout << "updated sliders\n";
                    */
                    reloadDefaultSettings();
                    refresh = true;
                }
                else if(event.key.code == sf::Keyboard::F1)
                {
                    segment newseg;
                    newseg.value = 254;
                    newseg.red = rand()%256;
                    newseg.green = rand()%256;
                    newseg.blue = rand()%256;

                    terSlider->addSegment(&newseg);
                }


            }
        }

        //draw screen
        screen->display();


    }

    //save settings after mainloop quit
    saveSettings();
}

void NoiseGen::randomPosition()
{
    xpos = rand()%10000 - 5000;
    ypos = rand()%10000 - 5000;
}

//////////////////////////////////////////////////////////////////////////////
//      DRAWING


void NoiseGen::createMapImage(int nx, int ny)
{

    mapTexture->clear();

    for(int i = 0; i < IMAGE_SIZE; i++)
    {
        for(int n = 0; n < IMAGE_SIZE; n++)
        {

            //note , the zoom factor for simplex differs from perlin, perlin > value zooms in, simplex < value zooms in
            if(N_MODE == SIMPLEX && !terrainmode) drawPixel(mapTexture, n, i, scaled_octave_noise_2d(octaves,persistence,zoom,0,255,n + nx,i + ny), IMAGE_SCALE);
            else if(N_MODE == SIMPLEX && terrainmode) drawPixelTerrain(mapTexture, n, i, scaled_octave_noise_2d(octaves,persistence,zoom,0,255,n + nx,i + ny), IMAGE_SCALE);
            else if(N_MODE == PERLIN && !terrainmode) drawPixel(mapTexture, n, i, getNoise(octaves, persistence, zoom, n + nx, i + ny), IMAGE_SCALE);
            else if(N_MODE == PERLIN && terrainmode) drawPixelTerrain(mapTexture, n, i, getNoise(octaves, persistence, zoom, n + nx, i + ny), IMAGE_SCALE);
        }
    }
}

void NoiseGen::drawMap()
{
    sf::Sprite mapSprite;
    mapSprite.setTexture( mapTexture->getTexture());

    screen->draw(mapSprite);
}

void NoiseGen::drawPixelTerrain(sf::RenderTarget *target, int x, int y, int shade, int scale)
{
    sf::RectangleShape newshape( sf::Vector2f(scale,scale));

    newshape.setPosition(sf::Vector2f(x*scale, y*scale));


        for(int i = 0; i < int(terrainsegs.size()); i++)
        {
            if( shade < terrainsegs[i].value)
            {
                sf::Color tcolor;
                tcolor.r = terrainsegs[i].red;
                tcolor.g = terrainsegs[i].green;
                tcolor.b = terrainsegs[i].blue;

                newshape.setFillColor(tcolor);
                break;
            }
        }


   target->draw(newshape);


}

void NoiseGen::drawPixel(sf::RenderTarget *target, int x, int y, int shade, int scale)
{

    sf::RectangleShape newshape( sf::Vector2f(scale,scale));

    newshape.setPosition(sf::Vector2f(x*scale, y*scale));

    newshape.setFillColor( sf::Color(shade,shade,shade));

    target->draw(newshape);
}

void NoiseGen::drawMask()
{

}

void NoiseGen::drawCoordinates(sf::Vector2i ncoord)
{
    std::stringstream coordS;
    coordS << "[ X:" << xpos << ", Y:" << ypos << " ]";

    sf::Text coordt(coordS.str(), font, 12);
    coordt.setPosition( ncoord.x, ncoord.y);

    screen->draw(coordt);
}

////////////////////////////////////////////////////////////////////////////////////
//      SLIDERS

void NoiseGen::initSliders()
{
    //create sliders

    //octave slider
    Slider *nslider = new Slider(screen, 1, 16, 100, &octaves);
    nslider->setCenter( sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + RIGHT_MARGIN_WIDTH/2,25));
    nslider->setRounding(true);
    nslider->setName("OCTAVES");
    nslider->setRealtime(true);
    sliders.push_back(nslider);

    nslider = new Slider(screen, 0, 0.015, 100, &zoom);
    nslider->setCenter( sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + RIGHT_MARGIN_WIDTH/2,65));
    nslider->setName("ZOOM");
    nslider->setRealtime(true);
    sliders.push_back(nslider);

    nslider = new Slider(screen, 0, 2, 100, &persistence);
    nslider->setCenter( sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + RIGHT_MARGIN_WIDTH/2,105));
    nslider->setName("PERSISTENCE");
    nslider->setRealtime(true);
    sliders.push_back(nslider);

}

void NoiseGen::initTerrainSlider()
{
    //terrainslider
    if(terSlider != NULL) delete terSlider;

    terSlider = new MultiSlider(screen, 0, 254, IMAGE_SIZE*IMAGE_SCALE, &terrainsegs);
    terSlider->setCenterPosition( sf::Vector2f( (IMAGE_SIZE*IMAGE_SCALE + RIGHT_MARGIN_WIDTH)/2, (IMAGE_SIZE*IMAGE_SCALE) + BOTTOM_MARGIN_HEIGHT/2  ));
    //for(int i = 0; i < 9; i++) terSlider->addSegment( &paramvalues[i], terraincolors[i]);
}

void NoiseGen::drawSliders()
{
    for(int i = 0; i < int(sliders.size()); i++)
    {
        std::stringstream slids;
        slids << sliders[i]->getName() << " : " << sliders[i]->convertSliderPosition();

        sf::Text stext(slids.str(), font, 12);
        stext.setPosition( sliders[i]->getPosition().x, sliders[i]->getPosition().y-20);
        sliders[i]->draw();
        screen->draw(stext);
    }

    terSlider->draw();
}

bool NoiseGen::updateSliders(sf::Event *event)
{
    bool need_update = false;

    for(int i = 0; i < int(sliders.size()); i++)
    {
        if(sliders[i]->update(event)) need_update = true;
    }

    //if an update is not currently needed, check terrain slider too
    if(!need_update && event != NULL)
    {
        if(terrainmode) need_update = terSlider->update(event);
        else terSlider->update(event);
    }
    //else if update is already needed, go ahead and update the terrain slider anyways
    else if(event != NULL) terSlider->update(event);


    return need_update;
}

////////////////////////////////////////////////////////////////////////////////////
//      BUTTONS

void NoiseGen::initButtons()
{
    Button *nbutton = new Button(screen, "Simplex", sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2)-40,140));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Perlin", sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2)+40,140));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Toggle Terrain", sf::Vector2f(IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2),170));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Export to TXT", sf::Vector2f( IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2), 480));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Export to IMAGE", sf::Vector2f( IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2), 510));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Random Position", sf::Vector2f( IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2), 250));
    buttons.push_back(nbutton);

    nbutton = new Button(screen, "Default Settings", sf::Vector2f( IMAGE_SCALE*IMAGE_SIZE + (RIGHT_MARGIN_WIDTH/2), 380));
    buttons.push_back(nbutton);

}


void NoiseGen::drawButtons()
{
    for(int i = 0; i < int(buttons.size()); i++)
    {
        buttons[i]->updateDraw();
    }
}
