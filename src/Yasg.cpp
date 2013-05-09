/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  Johannes Hackel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Yasg.h"

using namespace yasg;

Yasg::Yasg()
{
    int bubblcount = 5;
    int bubblecolumn = 20;
    int bubblerow = 10;
    int bubblesize = 30;
    Screencfg screencfg;
    screencfg.height = bubblerow * bubblesize + 40;
    screencfg.width = bubblecolumn * bubblesize;
    screencfg.bpp = 32;
    
    engine = new Engine(screencfg);
    engine->init();
    
    int font = engine->loadFont("../fonts/data-latin.ttf", 50);
    
    int blueIndex = engine->loadImage("../img/blue.png");
    int greenIndex = engine->loadImage("../img/green.png");
    int redIndex = engine->loadImage("../img/red.png");
    int pinkIndex = engine->loadImage("../img/pink.png");
    int yellowIndex = engine->loadImage("../img/yellow.png");
    
    playground = new Playground(engine, bubblcount, bubblecolumn, bubblerow, bubblesize, font);
    playground->registerColor(0, blueIndex);
    playground->registerColor(1, greenIndex);
    playground->registerColor(2, redIndex);
    playground->registerColor(3, pinkIndex);
    playground->registerColor(4, yellowIndex);
    
    engine->addWindowEventHandler(playground);
    
    menu = new Menu(engine, screencfg.width, screencfg.height, font);
    engine->addWindowEventHandler(menu);
    
    status = MENU;
    
    engine->addMouseEventHandler(this);
    engine->addRenderFunction(new SpecificFunctor<Yasg>(this, &Yasg::renderFunction));
}

Yasg::~Yasg()
{
    engine->run();
    engine->quit();
}

void Yasg::run()
{

}


void Yasg::renderFunction()
{
    switch(status) {
        case MENU:
            menu->render();
            break;
        case INGAME:
            playground->render();
            break;
        case NEWRESULT:
            //TODO
            break;
        case SCORES:
            //TODO
            break;
    }
}

void Yasg::updateME(MouseEvent mouseEvent)
{
    switch(status) {
        case MENU:
            switch(menu->updateME(mouseEvent)) {
                case NEWGAME:
                    status = INGAME;
                    playground->startNewGame();
                    break;
                case RESUMEGAME:
                    status = INGAME;
                    break;
                case SHOWSCORES:
                    status = SCORES;
                    //TODO
                    break;
                case EXIT:
                    //TODO
                    break;
            }
            break;
        case INGAME:
            switch(playground->updateME(mouseEvent)) {
                case RUNNING:
                    break;
                case PAUSED:
                    status = MENU;
                    break;
                case GAMEOVER:
                    status = NEWRESULT;
                    break;
            }
            break;
        case NEWRESULT:
            //TODO
            break;
        case SCORES:
            //TODO
            break;
    }
}