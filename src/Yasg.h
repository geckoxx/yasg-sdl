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

#ifndef YASG_YASG_H
#define YASG_YASG_H

#include "Engine.h"
#include "MouseEvent.h"
#include "WindowEvent.h"
#include "Menu.h"
#include "Playground.h"

namespace yasg {


class Yasg  : public MouseEventHandler
{
private:
    Engine* engine;
    Menu* menu;
    Playground* playground;
    
    enum Status {
       MENU, SCORES, INGAME, NEWRESULT
    } status;


public:
    Yasg();
    ~Yasg();
    
    void run();

    void renderFunction();
    virtual void updateME(MouseEvent mouseEvent);
};
}

#endif // YASG_YASG_H
