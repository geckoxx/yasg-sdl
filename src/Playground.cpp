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


#include "Playground.h"
#include <functional>
#include <iterator>
#include <iostream>

using namespace yasg;

Playground::Playground ( Engine* engine, int colorCount, int columns, int rows, int bubbleSize )
{
    this->engine = engine;
    font = engine->loadFont("../fonts/data-latin.ttf", 50);
    distribution = std::uniform_int_distribution<int>(0, colorCount - 1);
    colors = colorCount;
    colorIndeces = new int[colors];
    colorCounts = new int[colors];
    this->rows = rows;
    this->columns = columns;
    bubbles = new int*[columns];
    for(int i = 0; i < columns; ++i)
        bubbles[i] = new int[rows];
    this->bubbleSize = bubbleSize;
    score = 0;
    lastType = -1;
}

Playground::~Playground()
{

}

void Playground::registerColor ( int color, int surfaceIndex )
{
    if(color >= colors || color < 0)
        return;
    colorIndeces[color] = surfaceIndex;
}

void Playground::startNewGame()
{
    score = 0;
    for(int i = 0; i < colors; i++)
        colorCounts[i] = 0;
    fillBubbles();
    gameOver = false;
    lastType = -1;
}

void Playground::handleClick ( int x, int y )
{
    if(x < 0 || y < 0)
        return;
    if(gameOver)
        return;
    int column = x / bubbleSize;
    int row = y / bubbleSize;
    if(column >= columns || row >= rows)
        return;
    int typ = bubbles[column][row];
    if(typ == -1)
        return;
    selectedType = typ;
    selectedBubbles.clear();
    checkBubble(column, row);
    int selCount = selectedBubbles.size();
    if(selCount < 2)
        return;
    lastType = selectedType;
    lastBubbles = selectedBubbles;
    colorCounts[selectedType] -= selCount;
    score += (selCount - 1) * (selCount - 1);
    for(int i = 0; i < selCount; i++)
        bubbles[selectedBubbles[i].x][selectedBubbles[i].y] = -1;
    refresh();
    if(!floodMoveCheck(0, rows - 1, -1))
        gameOver = true;
}

void Playground::oneStepBack()
{
    if(lastType == -1)
        return;
}

void Playground::fillBubbles()
{
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
            createBlock(x, y);
}

void Playground::createBlock ( int column, int row )
{
    int typ = distribution(generator);
    colorCounts[typ]++;
    bubbles[column][row] = typ;
}

void Playground::checkBubble ( int column, int row )
{
    if(column < 0 || column >= columns || row < 0 || row >= rows)
        return;
    if(bubbles[column][row] != selectedType)
        return;
    else {
        position bubblePos;
        bubblePos.x = column;
        bubblePos.y = row;
        for(int i = 0; i < selectedBubbles.size(); i++) {
            if(selectedBubbles[i].x == bubblePos.x && selectedBubbles[i].y == bubblePos.y)
                return;
        }
        selectedBubbles.push_back(bubblePos);
        checkBubble(column - 1, row);
        checkBubble(column + 1, row);
        checkBubble(column, row - 1);
        checkBubble(column, row + 1);
    }
}

void Playground::refresh()
{
    int emptyColumn = 0;
    for(int x = columns - 1; x >= 0; x--) {
        int emptyElement = 0;
        for(int y = rows - 1; y >= 0; y--) {
            if(bubbles[x][y] == -1)
                emptyElement++;
            else if(emptyElement > 0){
                bubbles[x][y + emptyElement] = bubbles[x][y];
                bubbles[x][y] = -1;
            }
        }
        if(bubbles[x][rows - 1] == -1)
            emptyColumn++;
        else if(emptyColumn > 0) {
            for(int y = rows - 1; y >= 0; y--) {
                bubbles[x + emptyColumn][y] = bubbles[x][y];
                bubbles[x][y] = -1;
            }
        }
    }
    for(int x = emptyColumn - 1; x > -1; x--)
        for(int y = 0; y < rows; y++)
            createBlock(x, y);
}

bool Playground::floodMoveCheck ( int column, int row, int type )
{
    if(column < 0 || column >= columns || row < 0 || row >= rows)
        return false;
    int bubbleType = bubbles[column][row];
    if(bubbleType == -1)
        return false;
    if(bubbleType == type)
        return true;
    return floodMoveCheck(column + 1, row, bubbleType) || floodMoveCheck(column, row - 1, bubbleType);
}

void Playground::renderFunction()
{
    engine->renderText(0, bubbleSize * rows, strlen(std::to_string(score).c_str()) * 20, buttomRowHeight, std::to_string(score), font, { 100, 0, 0 });
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
            if(bubbles[x][y] != -1)
                engine->renderImage(x * bubbleSize, y * bubbleSize, bubbleSize, bubbleSize, colorIndeces[bubbles[x][y]]);
}

void Playground::updateME(MouseEvent mouseEvent)
{
    handleClick(mouseEvent.x, mouseEvent.y);
}

void Playground::updateWE(WindowEvent windowEvent)
{
    int widthSize = windowEvent.w / columns;
    int heightSize = (windowEvent.h - buttomRowHeight) / rows;
    bubbleSize = (widthSize < heightSize) ? widthSize : heightSize;
}

