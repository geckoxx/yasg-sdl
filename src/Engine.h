/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  johannes <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef YASG_ENGINE_H
#define YASG_ENGINE_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Functor.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

namespace yasg {

struct Screencfg {
    int width;
    int height;
    int bpp;
};

struct Coordinates {
    int x;
    int y;
};

class Engine
{
private:
    const GLuint videoModeFlags = SDL_OPENGL | SDL_RESIZABLE;
    bool inited;
    Screencfg scrcfg;
    SDL_Surface* screen; 
    std::vector<GLuint> textures;
    std::vector<TTF_Font*> fonts;
    
    std::vector<MouseEventHandler*> mouseEventHandler;
    std::vector<WindowEventHandler*> windowEventHandler;
    std::vector<Functor*> renderFunctions;

public:
    Engine();
    Engine(Screencfg config);
    virtual ~Engine();
    
    void init();
    void run();
    void quit();
    
    int loadImage(std::string filename);
    int loadFont(std::string filename, int size);
    int loadText(std::string text, int font, SDL_Color color);
    GLuint getTextTexture(std::string text, int font, SDL_Color color);
    void renderImage(int x, int y, int width, int height, int texture);
    void renderText(int x, int y, int width, int height, std::string text, int font, SDL_Color color);
    void addMouseEventHandler(MouseEventHandler* meHandler);
    void addWindowEventHandler(WindowEventHandler* weHandler);
    void addRenderFunction(Functor* function);
    
private:
    bool update();
    void render();
    void renderBackground();
    void renderTexture(int x, int y, int width, int height, GLuint texture);
};

}

#endif // YASG_ENGINE_H
