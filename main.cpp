#include <iostream>

#include "src/Engine.h"
#include "src/Playground.h"

using namespace yasg;

int main(int argc, char **argv) {
    try {
        int bubblcount = 5;
        int bubblecolumn = 20;
        int bubblerow = 10;
        int bubblesize = 30;
        Screencfg screencfg;
        screencfg.height = bubblerow * bubblesize + 40;
        screencfg.width = bubblecolumn * bubblesize;
        screencfg.bpp = 32;
        
        Engine eng = Engine(screencfg);
        eng.init();
        int blueIndex = eng.loadImage("../blue.png");
        int greenIndex = eng.loadImage("../green.png");
        int redIndex = eng.loadImage("../red.png");
        int violetIndex = eng.loadImage("../violet.png");
        int yellowIndex = eng.loadImage("../yellow.png");
        Playground* playground = new Playground(&eng, bubblcount, bubblecolumn, bubblerow, bubblesize);
        playground->registerColor(0, blueIndex);
        playground->registerColor(1, greenIndex);
        playground->registerColor(2, redIndex);
        playground->registerColor(3, violetIndex);
        playground->registerColor(4, yellowIndex);
        eng.addWindowEventHandler(playground);
        eng.addMouseEventHandler(playground);
        eng.addRenderFunction(new SpecificFunctor<Playground>(playground, &Playground::renderFunction));
        playground->startNewGame();
        eng.run();
        eng.quit();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
