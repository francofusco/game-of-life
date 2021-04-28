#include "game_of_life.hpp"
#include "debug.hpp"
#include <memory>

int main(int argc, char** argv) {
  std::unique_ptr<GameOfLife> app;
  if(argc == 1) {
    DEBUG("No arguments given: using default grid size");
    app = std::make_unique<GameOfLife>();
  }
  else if(argc == 2) {
    DEBUG("One argument given: " << argv[1] << " - it will be used as size of the grid");
    int size = std::atoi(argv[1]);
    app = std::make_unique<GameOfLife>(size);
  }
  else {
    // two arguments: one for width, one for height
    int width = std::atoi(argv[1]);
    int height = std::atoi(argv[2]);
    DEBUG("Two arguments given: " << argv[1] << ", " << argv[2] << " - they will be used as width and height of the grid");
    app = std::make_unique<GameOfLife>(width, height);
  }
  // run the app
  app->start();
  return EXIT_SUCCESS;
}
