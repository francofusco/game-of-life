#pragma once

#include <piksel/baseapp.hpp>
#include <vector>

class GameOfLife : public piksel::BaseApp {
public:
  static constexpr unsigned int DEFAULT_SIZE = 31; ///< Default grid size, if not given to the constructor.
  static constexpr float PIXEL_SIZE = 16.0f; ///< Size of a cell.
  static constexpr int MAX_AGE = 6; ///< Used for coloring purposes.
  static const glm::vec4 CELL_COLOR; ///< Color of alive cells.

  /// Initialize an empty grid of given size.
  GameOfLife(unsigned int width, unsigned int height);
  /// Initialize an empty squared grid of given size.
  GameOfLife(unsigned int size) : GameOfLife(size,size) {}
  /// Initialize an empty grid with default size.
  GameOfLife() : GameOfLife(DEFAULT_SIZE) {}

  /// Called once when starting the app.
  void setup() override;
  /// Called at every frame update.
  void draw(piksel::Graphics& g) override;
  /// Called when a button on the keyboard is pressed.
  void keyPressed(int key) override;
  /// Called whenever a mouse button is pressed.
  void mousePressed(int button) override;
  /// Called whenever the mouse is moved.
  void mouseMoved(int x, int y) override;

private:
  /// Changes color to be used to draw a cell.
  static void setColor(int age, piksel::Graphics& g);
  /// Helper method that converts a pair (row,col) into a grid index.
  int gridIndex(int row, int col) const;
  /// Helper method that "shifts" integers according to modulo arithmetic.
  static int modulo(int n, int mod);
  /// Helper method to access a grid cell.
  inline const int& cell(int row, int col) const { return grid_.at(gridIndex(row,col)); }
  /// Helper method to modify a grid cell.
  inline int& cell(int row, int col) { return grid_.at(gridIndex(row,col)); }
  // Helper method that "inverts" the value of a grid spot.
  inline void toggle(int row, int col) { toggle(gridIndex(row,col)); };
  // Helper method that "inverts" the value of a grid spot.
  void toggle(int i);

  // Count the (alive) neighbours of a given cell.
  int aliveNeighbours(int row, int col);

  /// Life finds a way.
  void updateGrid();

  // grid-related variables
  const unsigned int WIDTH; ///< Width of the grid.
  const unsigned int HEIGHT; ///< Height of the grid.
  const unsigned int& ROWS; ///< Number of grid rows. It is an alias of HEIGHT.
  const unsigned int& COLS; ///< Number of grid columns. It is an alias of WIDTH.
  std::vector<int> grid_; ///< Occupancy grid (`0` means the cell is free, other values give the "age" of a cell).

  // behavior-related variables
  int update_counter_; ///< When it reaches zero, the grid is updated.
  int max_update_counter_; ///< Value at which the counter should be reset everytime.
  int mouse_x_; ///< Store current mouse X position.
  int mouse_y_; ///< Store current mouse Y position.

  // text-related variables
  bool show_text_{true};

};
