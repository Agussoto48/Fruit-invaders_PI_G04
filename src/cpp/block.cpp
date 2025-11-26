#include "include/block.h"

extern "C" void asm_block_draw(float x, float y);
extern "C" void asm_block_get_rect(float x, float y, Rectangle* out);

Block::Block(Vector2 position)
    : position(position) {}

void Block::Draw()
{
  asm_block_draw(position.x, position.y);
}

Rectangle Block::getRect()
{
  Rectangle r;
  asm_block_get_rect(position.x, position.y, &r);
  return r;
}
