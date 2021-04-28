#include "game_of_life.hpp"
#include "debug.hpp"
#include <piksel/ext/rng.hpp>


const glm::vec4 GameOfLife::CELL_COLOR = glm::vec4(0.2,1.0,0.2,1);


GameOfLife::GameOfLife(
  unsigned int width,
  unsigned int height
)
: piksel::BaseApp(width*PIXEL_SIZE, height*PIXEL_SIZE)
, WIDTH(width)
, HEIGHT(height)
, ROWS(HEIGHT)
, COLS(WIDTH)
{
  // further construction if needed
}


void GameOfLife::setup()
{
  // initialize the counter
  update_counter_ = -1;
  max_update_counter_ = 5;
  // initialize the grid
  grid_ = std::vector<int>(ROWS*COLS, 0);
  auto r = ROWS/2 + 3;
  auto c = COLS/2;

  // glider
  // toggle(r-1,c+1);
  // toggle(r  ,c+1);
  // toggle(r+1,c+1);
  // toggle(r+1,c  );
  // toggle(r  ,c-1);

  // tumbler
  toggle(r  ,c+1);
  toggle(r  ,c+2);
  toggle(r+1,c+2);
  toggle(r+2,c+1);
  toggle(r+3,c+2);
  toggle(r+4,c+3);
  toggle(r+3,c+4);
  toggle(r+2,c+4);
  toggle(r  ,c-1);
  toggle(r  ,c-2);
  toggle(r+1,c-2);
  toggle(r+2,c-1);
  toggle(r+3,c-2);
  toggle(r+4,c-3);
  toggle(r+3,c-4);
  toggle(r+2,c-4);
}


int GameOfLife::gridIndex(
  int row,
  int col
) const
{
  return modulo(row,ROWS) * COLS + modulo(col,COLS);
}


int GameOfLife::modulo(
  int n,
  int mod
)
{
  // do the "regular" modulo
  n = n%mod;
  // if n is negative, "shift" it in the interval [0,mod)
  if(n < 0)
    n += mod;
  // return the result
  return n;
}


void GameOfLife::setColor(
  int age,
  piksel::Graphics& g
)
{
  // force age to be in the interval [1,MAX_AGE]
  age = std::max(1, std::min(MAX_AGE, age));
  // interpolation factor such that rho(1)=0.0 and rho(MAX_AGE)=1
  auto rho = (age-1.0)/(MAX_AGE-1.0);
  auto color = static_cast<float>(1-0.4*rho) * CELL_COLOR;
  // apply the color
  g.fill(color);
  g.stroke(color);
}


void GameOfLife::draw(
  piksel::Graphics& g
)
{
  // black background
  g.background(glm::vec4(0,0,0,1));

  // Print explanation if needed
  if(show_text_) {
    constexpr float textcolor = 1.f;
    constexpr float textstroke = 0.95f;
    g.fill(glm::vec4(textcolor,textcolor,textcolor,1));
    g.stroke(glm::vec4(textstroke,textstroke,textstroke,1));
    g.textSize(30.);
    g.text("Game of Life", 120, 50);
    g.textSize(18.);
    g.text(
      "Mouse click :  toggle cell\n"\
      "   space :  play/pause\n"\
      "   +/- :  change speed\n"\
      "      C :  clear\n"\
      "      R :  randomize",
      90, 100
    );

  }

  // Apply some scaling to make it easier to see the points
  g.scale(PIXEL_SIZE, PIXEL_SIZE);

  // print the current grid state
  for(unsigned int row=0; row<ROWS; row++) {
    for(unsigned int col=0; col<COLS; col++) {
      if(cell(row,col)) {
        // set color by age
        setColor(cell(row,col), g);
        // NOTE: the point is centered in the given coordinates, so we need to
        // shift them by half a pixel
        g.point(0.5+col,0.5+row);
      }
    }
  }

  // update the grid when needed
  if(update_counter_ == 0) {
    // update the grid content (according to Conway's rules)
    updateGrid();
    // reset the counter
    update_counter_ = max_update_counter_;
  }
  else if(update_counter_ > 0) {
    // reduce the time by 1
    update_counter_--;
  }
}


/** The rules of the Game of Life are the following:
  * - Any cell with less than 2 neighbours die;
  * - Any cell with 2 or 3 neighbours survives;
  * - Any cell with more than 3 neighbours dies;
  * - An empty spot is populated if it has exactly 3 neighbours.
  */
void GameOfLife::updateGrid()
{
  // this list will contain the index of every cell that has to be changed
  std::vector<int> to_change;
  // while this keeps the list of cells whose age should be increased!
  std::vector<int> to_age;
  // check each grid location to see if we should update it
  for(unsigned int row=0; row<ROWS; row++) {
    for(unsigned int col=0; col<COLS; col++) {
      auto neighbours = aliveNeighbours(row,col);
      if(cell(row,col)) {
        // a cell with less than 2 or more than 3 neighbours will die
        if(neighbours < 2 || neighbours > 3)
          to_change.push_back(gridIndex(row,col));
        else
          to_age.push_back(gridIndex(row, col));
      }
      else {
        // an empty spot with 3 neighbours will populate
        if(neighbours == 3)
          to_change.push_back(gridIndex(row,col));
      }
    }
  }
  // apply all needed changes
  for(const auto& i : to_change) {
    toggle(i);
  }
  for(const auto& i : to_age) {
    grid_.at(i)++;
  }
}


int GameOfLife::aliveNeighbours(int row, int col) {
  // initialize the counter
  int alives = 0;
  // check all cases
  if(cell(row-1,col-1))
    alives++;
  if(cell(row-1,col))
    alives++;
  if(cell(row-1,col+1))
    alives++;
  if(cell(row,col-1))
    alives++;
  if(cell(row,col+1))
    alives++;
  if(cell(row+1,col-1))
    alives++;
  if(cell(row+1,col))
    alives++;
  if(cell(row+1,col+1))
    alives++;
  // return the final value
  return alives;
}


void GameOfLife::toggle(int i) {
  auto& c = grid_.at(i);
  if(c > 0)
    c = 0;
  else
    c = 1;
}


void GameOfLife::keyPressed(
  int key
)
{
  DEBUG("Pressed key with code " << key);
  show_text_ = false;

  // pressing space allows to pause/play the game
  if(key == ' ') {
    if(update_counter_ < 0) {
      DEBUG("Resuming game");
      update_counter_ = max_update_counter_;
    }
    else {
      DEBUG("Pausing game");
      update_counter_ = -1;
    }
  }

  // pressing "plus" speeds up the game
  if(key == 334) {
    if(max_update_counter_ > 0)
      max_update_counter_--;
    DEBUG("max_update_counter_ decremented to " << max_update_counter_);
  }

  // pressing "minus" slows down the game
  if(key == 333) {
    max_update_counter_++;
    DEBUG("max_update_counter_ incremented to " << max_update_counter_);
  }

  if(key == 'C') {
    DEBUG("Removing all cells");
    for(auto& c : grid_) {
      c = 0;
    }
  }

  if(key == 'R') {
    DEBUG("Adding random cells");
    for(int i=0; i<grid_.size(); i++) {
      if(piksel::Rng::getInstance().random(0,1) > 0.75) {
        toggle(i);
      }
    }
  }
}


void GameOfLife::mousePressed(
  int button
)
{
  DEBUG("Pressed mouse button with ID " << button);
  show_text_ = false;
  // If the game has been paused, allow to change the content of the grid
  if(update_counter_ < 0) {
    int row = std::floor(mouse_y_ / PIXEL_SIZE);
    int col = std::floor(mouse_x_ / PIXEL_SIZE);
    toggle(row,col);
    DEBUG("Toggled (" << row << "," << col << ")");
  }
}


void GameOfLife::mouseMoved(
  int x,
  int y
)
{
  DEBUG("Mouse moved at (" << x << "," << y << ")");
  mouse_x_ = x;
  mouse_y_ = y;
}
