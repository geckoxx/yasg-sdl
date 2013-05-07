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

#ifndef YASG_BUTTON_H
#define YASG_BUTTON_H

#include <string>
#include <SDL/SDL.h>

#include "Engine.h"
#include "MouseEvent.h"

namespace yasg {
    
class Button
{
private:
    int x, y;
    int width, height;
    
    std::string text;
    int font;
    SDL_Color color;
    
public:
    Button(int x, int y, int width, int height, std::string text, int font, SDL_Color color);
    ~Button();
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setText(std::string text);
    void setFont(int font);
    void setColor(SDL_Color color);
    
    bool clicked(MouseEvent mouseEvent);
    void render(Engine* engine);

};

}
#endif // YASG_BUTTON_H
