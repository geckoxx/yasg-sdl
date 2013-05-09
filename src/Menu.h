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

#ifndef YASG_MENU_H
#define YASG_MENU_H

#include <string>

#include "Engine.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
#include "Button.h"

namespace yasg {
    
enum MenuChoose {
    NOTHING, NEWGAME, RESUMEGAME, SHOWSCORES, EXIT
};

class Menu : public WindowEventHandler
{
private:
    Engine* engine;
    SDL_Color fgColor;
    int textHeigth;
    int buttonDistance;
    bool inGame;
    
    std::string newGameStr = "Neues Spiel";
    std::string resumeGameStr = "Spiel fortsetzen";
    std::string scoresStr = "Scores";
    std::string exitStr = "Beenden";
    
    Button newGameBtn;
    Button resumeGameBtn;
    Button scoresBtn;
    Button exitBtn;
    
public:
    Menu(Engine* engine, int scrWidth, int scrHeight, int font);
    ~Menu();
    
    void render();
    MenuChoose updateME(MouseEvent mouseEvent);
    virtual void updateWE(WindowEvent windowEvent);

};
}

#endif // YASG_MENU_H
