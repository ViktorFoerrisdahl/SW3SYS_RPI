#include "MemSimulator.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  MemSimulator mem(100);

  mem.showVisual();

  mem.allocate(19);
  mem.showVisual();

  mem.allocate(11);
  mem.showVisual();

  mem.allocate(16);
  mem.showVisual();

  mem.allocate(20);
  mem.showVisual();

  mem.allocate(15);
  mem.showVisual();

  mem.allocate(13);
  mem.showVisual();

  mem.allocate(6);
  mem.showVisual();

  mem.deallocate(1);
  mem.showVisual();

  mem.deallocate(5);
  mem.showVisual();

  mem.deallocate(3);
  mem.showVisual();


  return 0;
}
