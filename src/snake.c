#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT
} Direction;

typedef struct Position {
  int x;
  int y;
  struct Position *next;
} Position;

Position *snake;
Position food;

int score = 0;
int max_x = 0;
int max_y = 0;

Position *create_pos(int x, int y)
{
  Position *pos = malloc(sizeof(*pos));

  pos->x = x;
  pos->y = y;
  pos->next = NULL;

  return pos;
}

Position *create_snake() 
{
  Position *a = create_pos(2, 3);
  Position *b = create_pos(2, 2);
  
  a->next = b;

  return a;
}

void add_food()
{
  food.x = (rand() % (max_x - 20)) + 10;
  food.y = (rand() % (max_y - 10)) + 5;
}

Direction get_next_dir(Direction prev) 
{
  int ch = getch();

  switch (ch) {
    case KEY_LEFT:
      if (prev != RIGHT) return LEFT;
    case KEY_RIGHT:
      if (prev != LEFT) return RIGHT;
    case KEY_UP:
      if (prev != DOWN) return UP;
    case KEY_DOWN:
      if (prev != UP) return DOWN;
    default:
      return prev;
  }
}

Position *next_pos(Position *snake, Direction dir) 
{
  int next_x = snake->x;
  int next_y = snake->y;

  switch (dir) {
    case LEFT:
      next_x--;
      break;
    case RIGHT:
      next_x++;
      break;
    case UP:
      next_y--;
      break;
    case DOWN:
      next_y++;
      break;
  }

  return create_pos(next_x, next_y);
}

void move_snake(Direction dir) 
{
  Position *snake_start = next_pos(snake, dir);
  
  if (snake_start->x == food.x && snake_start->y == food.y) {
    snake_start->next = snake;
    snake = snake_start;
    score += 50;
    add_food();
    return;
  }

  Position *snake_end = snake;  
  snake_start->next = snake;
  snake = snake_start;

  while (snake_end->next->next) {
    snake_end = snake_end->next;
  }

  free(snake_end->next);
  snake_end->next = NULL;
}

void draw_snake() 
{
  Position *pos = snake;

  while (pos) {
    mvaddch(pos->y, pos->x, 'O');
    pos = pos->next;
  }
}

int main(int argc, char *argv[]) 
{
  initscr();
  noecho();
  cbreak();
  timeout(10);
  keypad(stdscr, TRUE);
  curs_set(FALSE);

  getmaxyx(stdscr, max_y, max_x);

  Direction dir = RIGHT;
  snake = create_snake();
  add_food();

  while (1) {
    clear();

    draw_snake();
    mvaddch(food.y, food.x, 'X');
    mvprintw(1, 2, "Score: %i", score);

    refresh();

    dir = get_next_dir(dir);
    move_snake(dir);

    usleep(30000);
  }

  endwin();
  nocbreak();

  exit(EXIT_SUCCESS);
}
