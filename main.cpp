#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <iostream>

#define SNAKE_COLOR 1
#define FOOD_COLOR 2

struct Position
{
  int x;
  int y;
  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }
};

struct Snake
{
  Position position;
  int length;
  std::vector<Position> previousPositions;
};

void gameover(int score, int winx, int winy, bool* running)
{
  *running = false;
  mvaddstr(winy / 2, winx / 2, "Game over!");
  mvaddstr(winy / 2 + 1, winx / 2, "Score: ");
      printw("%d", score);
  
}
int score;
bool hasColor;
int main()
{
  srand(time(NULL));
  // initialize ncurses
  WINDOW* win = initscr();
  keypad(win, true);
  nodelay(win, true);
  curs_set(0);
  // get window size
  int wx, wy = 0;
  getmaxyx(win, wy, wx);
  // snake stuff
  Snake snake;
  snake.position.x = 0;
  snake.position.y = 0;
  snake.length = 1;
  Position food;
  food.x = rand() % wx;
  food.y = rand() % wy;
  int dirX = 1;
  int dirY = 0;
  bool running = true;
  // initialize colors
  hasColor = has_colors();
  if(hasColor)
  {
    start_color();
    init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(FOOD_COLOR, COLOR_RED, COLOR_BLACK);
  }
  while(running)
  {
    snake.previousPositions.push_back(snake.position); 
    int pressed = wgetch(win);
    switch(pressed)
    {
      case KEY_LEFT:
        {
          if(snake.length >= 2 && dirX == 1) break;
          dirX = -1;
          dirY = 0;
        } break;
      case KEY_RIGHT:
        {
          if(snake.length >= 2 && dirX == -1) break;
          dirX = 1;
          dirY = 0;
        } break;
      case KEY_UP:
        {
          if(snake.length >= 2 && dirY == 1) break;
          dirX = 0;
          dirY = -1;
        } break;
      case KEY_DOWN:
        {
          if(snake.length >= 2 && dirY == -1) break;
          dirX = 0;
          dirY = 1;
        } break;
    }
    snake.position.x += dirX;
    snake.position.y += dirY;
    
    erase();
    if(snake.previousPositions.size() > snake.length)
    {
      snake.previousPositions.erase(snake.previousPositions.begin());
    }
    score = snake.length;
    if((snake.position.x > wx ||
        snake.position.x < 0 || 
        snake.position.y > wy || 
        snake.position.y < 0) ||
        (std::find(snake.previousPositions.begin(), snake.previousPositions.end(), snake.position) != snake.previousPositions.end()))
    {
      gameover(snake.length, wx, wy, &running);
    }
    
    if(hasColor) attron(COLOR_PAIR(SNAKE_COLOR));
    for(Position p : snake.previousPositions)
    {  
      mvaddstr(p.y, p.x, "~");
    }
    if(hasColor) {
      attroff(COLOR_PAIR(SNAKE_COLOR));
      attron(COLOR_PAIR(FOOD_COLOR));
    }
    mvaddstr(food.y, food.x, "@");
    if(hasColor) attroff(COLOR_PAIR(FOOD_COLOR));
    if((snake.position.x == food.x) && (snake.position.y == food.y))
    {
      snake.length++;
      food.x = rand() % wx;
      food.y = rand() % wy;
    }
    usleep(100000);
  }
  
  refresh();
  usleep(1000000 * 5);
  endwin();
  std::cout << "You lost!\nYour score was " << score << std::endl;
  return 0;
}
