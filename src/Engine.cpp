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


#include "Engine.h"
#include "YasgException.h"
#include "const.h"

#include <math.h>
#include <iostream>

using namespace yasg;

Engine::Engine()
{
    inited = false;
    this->scrcfg.width = 600;
    this->scrcfg.height = 400;
    this->scrcfg.bpp = 32;
}

Engine::Engine(Screencfg config)
{
    inited = false;
    this->scrcfg = config;
}

Engine::~Engine()
{
    quit();
}

void Engine::init()
{
    if(inited)
        return;
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        throw YasgException(sdlInitFail);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    screen = SDL_SetVideoMode(scrcfg.width, scrcfg.height, scrcfg.bpp, videoModeFlags);
    if( screen == nullptr )
        throw YasgException(sdlScreenFail);
    if( TTF_Init() == -1 )
        throw YasgException("True Type Font Initation failed!");
    SDL_WM_SetCaption(WINDOWTITLE, nullptr);
    //glClear( GL_COLOR_BUFFER_BIT );

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable( GL_TEXTURE_2D ); // Need this to display a texture
    //glDisable(GL_DEPTH_TEST);
    //glClearColor( 0, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    glViewport( 0, 0, scrcfg.width, scrcfg.height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, scrcfg.width, scrcfg.height, 0, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    //glLoadIdentity();

    inited = true;
}

void Engine::run()
{
    Uint32 minFrameTime = 10000 / 60;
    Uint32 startTicks;
    Uint32 offset;
    while(true) {
        startTicks = SDL_GetTicks();
        if(update())
            break;
        render();
        offset = minFrameTime - (SDL_GetTicks() - startTicks);
        if(offset > 0) {
            SDL_Delay(offset);
        }
    }
}

void Engine::quit()
{
    if(!inited)
        return;
    for(int i = 0; i<textures.size(); i++)
        glDeleteTextures( 1, &(textures[i]) ); 
    for(int i = 0; i<fonts.size(); i++)
        TTF_CloseFont(fonts[i]);
    TTF_Quit();
    SDL_Quit();
    inited = false;
}

int Engine::loadImage(std::string filename)
{
    if(!inited)
        return -1;
    SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
    if(tmpSurface == nullptr)
        throw YasgException(sdlImgLoadFail);
    SDL_Surface* optSurface = SDL_DisplayFormatAlpha(tmpSurface);
    SDL_FreeSurface(tmpSurface);
    GLenum texture_format;
    GLint nofcolors = optSurface->format->BytesPerPixel;
    if(nofcolors==4)
    {
        if(optSurface->format->Rmask==0x000000ff)
            texture_format=GL_RGBA;
        else
            texture_format=GL_BGRA;
    }
    else if(nofcolors==3) //no alpha channel
    {
        if(optSurface->format->Rmask==0x000000ff)
            texture_format=GL_RGB;
        else
            texture_format=GL_BGR;
    }
    else
    {
        const char* warning = "warning: the image is not truecolor...this will break ";
        throw YasgException(warning);
    }
    GLuint texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, nofcolors, optSurface->w, optSurface->h, 0, texture_format, GL_UNSIGNED_BYTE, optSurface->pixels );

    textures.push_back(texture);
    SDL_FreeSurface(optSurface);
    return textures.size() - 1;
}

int Engine::loadFont(std::string filename, int size)
{
    TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
    fonts.push_back(font);
    return fonts.size() - 1;
}


int Engine::loadText(std::string text, int font, SDL_Color color)
{
    GLuint texture = this->getTextTexture(text, font, color);
    textures.push_back(texture);
    return textures.size() - 1;
}

GLuint Engine::getTextTexture(std::string text, int font, SDL_Color color)
{
    
    SDL_Surface* tmpSurface;
    SDL_Surface* optSurface;
    int w,h;
    GLuint texture;
    
    /* Use SDL_TTF to render our text */
    //tmpSurface = TTF_RenderText_Blended(fonts[font], text.c_str(), color);
    //tmpSurface = TTF_RenderText_Solid(fonts[font], text.c_str(), color);
    tmpSurface = TTF_RenderText_Shaded(fonts[font], text.c_str(), color, {0,0,0});
    
    /* Convert the rendered text to a known format */
    w = round(pow(2,ceil(log(tmpSurface->w) / log(2))));
    h = round(pow(2,ceil(log(tmpSurface->h) / log(2))));
    
    optSurface = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_BlitSurface(tmpSurface, 0, optSurface, 0);
    
    /* Tell GL about our new texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, optSurface->pixels );
    
    /* GL_NEAREST looks horrible, if scaled... */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(tmpSurface);
    SDL_FreeSurface(optSurface);
    return texture;
}

void Engine::renderTexture(int x, int y, int width, int height, GLuint texture)
{
    glBindTexture( GL_TEXTURE_2D, texture );

    glBegin( GL_QUADS );

    glTexCoord2i( 0, 0 );
    glVertex3f( x, y, 0);

    glTexCoord2i( 1, 0 );
    glVertex3f( x + width, y, 0);

    glTexCoord2i( 1, 1 );
    glVertex3f( x + width, y + height, 0);

    glTexCoord2i( 0, 1 );
    glVertex3f( x, y + height, 0);
    glEnd();
}

void Engine::renderImage(int x, int y, int width, int height, int texture)
{
    this->renderTexture(x, y, width, height, textures[texture]);
}

void Engine::renderText(int x, int y, int width, int height, std::string text, int font, SDL_Color color)
{
    GLuint texture = this->getTextTexture(text, font, color);
    this->renderTexture(x, y, width, height, texture);
    glDeleteTextures( 1, &texture );
}

void Engine::addMouseEventHandler(MouseEventHandler* meHandler)
{
    mouseEventHandler.push_back(meHandler);
}

void Engine::addWindowEventHandler(WindowEventHandler* weHandler)
{
    windowEventHandler.push_back(weHandler);
}

void Engine::addRenderFunction(Functor* function)
{
    renderFunctions.push_back(function);
}

bool Engine::update()
{
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        if(event.type == SDL_QUIT)
            return true;
        else if(event.type == SDL_MOUSEBUTTONUP) {
            if( event.button.button == SDL_BUTTON_LEFT ) {
                MouseEvent me;
                me.x = event.button.x;
                me.y = event.button.y;
                for(int i = 0; i < mouseEventHandler.size(); i++)
                    mouseEventHandler[i]->updateME(me);
            }
        } else if(event.type == SDL_VIDEORESIZE) {
            WindowEvent we;
            we.w = event.resize.w;
            we.h = event.resize.h;
            scrcfg.width = event.resize.w;
            scrcfg.height = event.resize.h;
            screen = SDL_SetVideoMode(scrcfg.width, scrcfg.height, scrcfg.bpp, videoModeFlags);
            glViewport( 0, 0, scrcfg.width, scrcfg.height );
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glOrtho( 0, scrcfg.width, scrcfg.height, 0, -1, 1 );
            glMatrixMode( GL_MODELVIEW );
            //glLoadIdentity();
            if( screen == nullptr )
                throw YasgException(sdlScreenFail);
            for(int i = 0; i < windowEventHandler.size(); i++)
                windowEventHandler[i]->updateWE(we);
        }
    }
    return false;
}

void Engine::render()
{
    //if( SDL_Flip( screen ) == -1 )
    //throw YasgException(sdlScreenFlipFail);
    glClearColor( 0.5f, 0.5f, 0.5f, 0.f );
    glClear( GL_COLOR_BUFFER_BIT );
    //renderBackground();
    for(int i = 0; i < renderFunctions.size(); i++)
        renderFunctions[i]->Call();
    glLoadIdentity();
    SDL_GL_SwapBuffers();
}

void Engine::renderBackground()
{
    
    glColor3f( 1.0f, 0.0f, 0.0f);
    glBegin( GL_QUADS );
    //glColor3f( 0.0f, 0.0f, 0.0f);
    glVertex3f( 0, 0, -1 );
    //glColor3f( 0.0f, 0.0f, 0.0f);
    glVertex3f( scrcfg.width, 0, -1 );
    //glColor3f( 0.0f, 0.0f, 0.0f);
    glVertex3f( scrcfg.width, scrcfg.height, -1 );
    //glColor3f( 0.0f, 0.0f, 0.0f);
    glVertex3f( 0, scrcfg.height, -1 );
    glEnd() ;
}
