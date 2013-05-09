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

#include "Menu.h"

using namespace yasg;

Menu::Menu(Engine* engine, int scrWidth, int scrHeight, int font)
{
    this->engine = engine;
    textHeigth = 50;
    buttonDistance = 40;
    inGame = false;
    fgColor = {200, 100, 100};
    int scrWidthHlf = scrWidth / 2;
    int baseBtnY = (scrHeight - (3 * textHeigth + 2 * buttonDistance)) / 2;
    
    int newGameWidth = strlen(newGameStr.c_str()) * 20;
    newGameBtn = Button(scrWidthHlf - (newGameWidth / 2), baseBtnY, newGameWidth, textHeigth, newGameStr, font, fgColor);
    
    int scoresWidth = strlen(scoresStr.c_str()) * 20;
    scoresBtn = Button(scrWidthHlf - (scoresWidth / 2), baseBtnY + textHeigth + buttonDistance, scoresWidth, textHeigth, scoresStr, font, fgColor);
    
    int exitWidth = strlen(exitStr.c_str()) * 20;
    exitBtn = Button(scrWidthHlf - (exitWidth / 2), baseBtnY + 2 * textHeigth + 2 * buttonDistance, exitWidth, textHeigth, exitStr, font, fgColor);
}

Menu::~Menu()
{

}

void Menu::render()
{
    newGameBtn.render(engine);
    scoresBtn.render(engine);
    exitBtn.render(engine);
}

MenuChoose Menu::updateME(MouseEvent mouseEvent)
{
    if(newGameBtn.clicked(mouseEvent))
        return NEWGAME;
    if(scoresBtn.clicked(mouseEvent))
        return SHOWSCORES;
    if(exitBtn.clicked(mouseEvent))
        return EXIT;
    return NOTHING;
}

void Menu::updateWE(WindowEvent windowEvent)
{

}
