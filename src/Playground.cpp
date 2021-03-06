/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  Johannes Hackel

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
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace yasg;

Playground::Playground ( Engine* engine, int colorCount, int columns, int rows, int bubbleSize, int font )
{
    this->engine = engine;
    this->font = font;
    std::srand(std::time(0));
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
    int backWidth = strlen("Back") * 20;
    backButton = Button(0, rows * bubbleSize, backWidth, buttomRowHeight, "Back", font, {200, 0, 0});
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
    distribution = std::uniform_int_distribution<int>(0, colors - 1);
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
    lastPoints = (selCount - 1) * (selCount - 1);
    colorCounts[selectedType] -= selCount;
    score += lastPoints;
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
    for(std::map<int, std::map<int, position>>::iterator iter1 = prevPos.begin(); iter1 != prevPos.end(); iter1++) {
        for(std::map<int, position>::iterator iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++) {
            //colorCounts[bubbles[iter2->second.x][iter2->second.y]]--;
            bubbles[iter2->second.x][iter2->second.y] = bubbles[iter1->first][iter2->first];
            bubbles[iter1->first][iter2->first] = -1;
        }
    }
    prevPos.clear();
    for(int i = 0; i < lastBubbles.size(); i++) {
        bubbles[lastBubbles[i].x][lastBubbles[i].y] = lastType;
    }
    colorCounts[lastType] += lastBubbles.size();
    lastType = -1;
    score -= lastPoints; 
    lastBubbles.clear();
}

void Playground::fillBubbles()
{
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
            createBlock(x, y);
}

void Playground::createBlock ( int column, int row )
{
    //int typ = distribution(generator);
    int typ = std::rand() % colors; 
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
    prevPos.clear();
    int emptyColumn = 0;
    for(int x = columns - 1; x >= 0; x--) {
        int emptyElement = 0;
        for(int y = rows - 1; y >= 0; y--) {
            if(bubbles[x][y] == -1)
                emptyElement++;
            else if(emptyElement > 0) {
                position oldPos;
                oldPos.x = x;
                oldPos.y = y;
                prevPos[x][y + emptyElement] = oldPos;
                bubbles[x][y + emptyElement] = bubbles[x][y];
                bubbles[x][y] = -1;
            }
        }
        if(bubbles[x][rows - 1] == -1)
            emptyColumn++;
        else if(emptyColumn > 0) {
            for(int y = rows - 1; y >= 0; y--) {
                position oldPos;
                try {
                    oldPos = prevPos.at(x).at(y);
                    prevPos.at(x).erase(y);
                } catch (const std::out_of_range& oor) {
                    oldPos.x = x;
                    oldPos.y = y;
                }
                prevPos[x + emptyColumn][y] = oldPos;
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

void Playground::render()
{
    if(lastType != -1)
        backButton.render(engine);
    int scoreWidth = strlen(std::to_string(score).c_str()) * 20;
    engine->renderText(bubbleSize * columns - scoreWidth, bubbleSize * rows, scoreWidth, buttomRowHeight, std::to_string(score), font, { 200, 0, 0 });
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
            if(bubbles[x][y] != -1)
                engine->renderImage(x * bubbleSize, y * bubbleSize, bubbleSize, bubbleSize, colorIndeces[bubbles[x][y]]);
}

Gamestatus Playground::updateME(MouseEvent mouseEvent)
{
    handleClick(mouseEvent.x, mouseEvent.y);
    if(backButton.clicked(mouseEvent))
        this->oneStepBack();
    if(gameOver)
        return PAUSED;
    return RUNNING;
}

void Playground::updateWE(WindowEvent windowEvent)
{
    int widthSize = windowEvent.w / columns;
    int heightSize = (windowEvent.h - buttomRowHeight) / rows;
    bubbleSize = (widthSize < heightSize) ? widthSize : heightSize;
    backButton.setY(bubbleSize * rows);
}

