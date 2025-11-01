#include "include/block.h"

Block::Block (Vector2 position) 
: position(position) {}

void Block::Draw() {
  DrawRectangle(position.x, position.y, 3, 3, {243, 216, 63, 255});
}