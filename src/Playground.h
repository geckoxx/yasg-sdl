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


#ifndef YASG_PLAYGROUND_H
#define YASG_PLAYGROUND_H

#include <random>
#include <vector>

#include "Engine.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

namespace yasg {

struct position {
    int x;
    int y;
};

class Playground : public MouseEventHandler, public WindowEventHandler
{
private:
    Engine* engine;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

    int colors;
    int* colorIndeces;
    int* colorCounts;
    int rows, columns;
    int** bubbles;
    
    int bubbleSize;
    int width, height;
    const int buttomRowHeight = 40;
    
    int score;
    bool gameOver;
    
    int selectedType;
    std::vector<position> selectedBubbles;
    
    int lastType;
    std::vector<position> lastBubbles;
    
    int font;

public:
    Playground(Engine* engine, int colorCount, int columns, int rows, int bubbleSize);
    virtual ~Playground();
    
    void registerColor(int color, int surfaceIndex);
    void startNewGame();
    void handleClick(int x, int y);
    void oneStepBack();
    
    void renderFunction();
    virtual void updateME(MouseEvent mouseEvent);
    virtual void updateWE(WindowEvent windowEvent);
    
private:
    void fillBubbles();
    void createBlock(int column, int row);
    void checkBubble(int column, int row);
    void refresh();
    bool floodMoveCheck(int column, int row, int type);
};

}

#endif // YASG_PLAYGROUND_H
