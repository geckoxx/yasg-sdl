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

#include "Button.h"

using namespace yasg;

Button::Button(int x, int y, int width, int height, std::string text, int font, SDL_Color color)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->text = text;
    this->font = font;
    this->color = color;
}

Button::~Button()
{
}

void Button::setX(int x)
{
    this->x = x;
}

void Button::setY(int y)
{
    this->y = y;
}

void Button::setWidth(int width)
{
    this->width = width;
}

void Button::setHeight(int height)
{
    this->height = height;
}

void Button::setText(std::string text)
{
    this->text = text;
}

void Button::setFont(int font)
{
    this->font = font;
}

void Button::setColor(SDL_Color color)
{
    this->color = color;
}


bool Button::clicked(MouseEvent mouseEvent)
{
    if(mouseEvent.x > x && mouseEvent.x < (x + width) && mouseEvent.y > y && mouseEvent.y < (y + height))
        return true;
    return false;
}

void Button::render(Engine* engine)
{
    engine->renderText(x, y, width, height, text, font, color);
}
