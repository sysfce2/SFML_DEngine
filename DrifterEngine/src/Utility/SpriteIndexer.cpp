#include "../pch.h"
#include "SpriteIndexer.h"

using namespace drft::util;

sf::IntRect SpriteIndexer::get(unsigned int index)
{
    unsigned int row = 0;
    unsigned int col = 0;

    row = index / 10; // 10 = width of texture in # of sprites
    col = index % 10; 

    return sf::IntRect(col*16,row*16,16,16); // 16 px is the dimension of all sprites
}
