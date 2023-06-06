// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/tron.h"

#define NONE 0
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define GRIDSQUARE 32 // length of a side of a gridsquare in bytes
#define GRIDXLIMIT (SCREEN_PIXEL_WIDTH / GRIDSQUARE)
#define GRIDYLIMIT (SCREEN_PIXEL_HEIGHT / GRIDSQUARE)
#define TRUE 1
#define FALSE 0
#define COLORP1 0xFF5782
#define COLORP2 0x57FF62
#define GRIDCOLOR 0x656565
#define BLACK 0x00
#define CANTPLAYERS 2 // amount of players
#define CANTGAMES 3   // amount of games

typedef struct player {
  int x;
  int y;
} player;

// 30x22 grid
static int taken[GRIDXLIMIT][GRIDYLIMIT] = {{0}};
static int wins[CANTPLAYERS];
static int played_games = 0;

void load_game_board();
void draw_grid();
void move_players(player *p1, player *p2);
void clear_taken();

int p1YFlag = UP, p2YFlag = DOWN, p1XFlag = NONE,
    p2XFlag = NONE; // in what x and y direction each player
                    // will move to each refresh
// initially moving up

void tron() {
  call_to_set_size(DEFAULT_TEXT_SIZE);
  if (played_games == 0) {
    tron_menu();
    wins[0] = 0;
    wins[1] = 0;
  }
  hvd_clear();
  printf("     TRON");
  print_score(wins[0], wins[1]);

  load_game_board();

  // Arbitrary starting positions:
  player p1 = {16 * GRIDSQUARE, SCREEN_PIXEL_HEIGHT - 4 * GRIDSQUARE};

  player p2 = {16 * GRIDSQUARE, 3 * GRIDSQUARE};

  int whoHit = 0;
  while (!(whoHit = hit_player(p1.x, p1.y, p2.x, p2.y))) {
    call_to_fillrectangle(p1.x, p1.y, COLORP1, GRIDSQUARE, GRIDSQUARE);
    call_to_fillrectangle(p2.x, p2.y, COLORP2, GRIDSQUARE, GRIDSQUARE);

    int tickAnt = call_to_getticks();

    while (call_to_getticks() <= tickAnt + 1) {
      char c = get_char();
      if (c == 'Q') { // quit game
        end_game();
        return;
      }
      if (c != 0) {
        tron_char(c); // change directions flags based on
                      // received keys
      }
    }

    // move both players in their flag direction
    move_players(&p1, &p2);
  }
  game_over(whoHit); // prints winner of a game of 3

  clear_taken();            // set taken array as 0's for next game
  p1XFlag = p2XFlag = NONE; // set default starting positions
  p1YFlag = UP;
  p2YFlag = DOWN;
  played_games++;
  if (played_games < CANTGAMES) { // play again
    tron();
  } else {
    print_winner();
    end_game();
  }
}
void end_game() {
  clear_taken();
  p1XFlag = p2XFlag = NONE; // set default starting direction
  p1YFlag = UP;
  p2YFlag = DOWN;
  played_games = 0;
  wins[0] = 0;
  wins[1] = 0;
  return_to_shell();
  call_to_clear_buffer();
}
void clear_taken() {
  for (int i = 0; i < GRIDXLIMIT; i++)
    for (int j = 0; j < GRIDYLIMIT; j++)
      taken[i][j] = NONE;
}

void print_score(int p1, int p2) {
  call_to_setptry(0);
  call_to_setptrx(SCREEN_PIXEL_WIDTH / 2);
  printf("P1: %d     P2: %d", p1, p2);
}

void print_winner() {
  hvd_clear();
  call_to_setptrx(SCREEN_PIXEL_WIDTH / 2 - GRIDSQUARE * 3);
  call_to_setptry(SCREEN_PIXEL_HEIGHT / 2);
  if (wins[0] > wins[1]) {
    printf("Player 1 won!");
    victory(); // sound
  } else if (wins[0] < wins[1]) {
    printf("Player 2 won!");
    victory();
  } else {
    printf("Its a tie!");
    mario(); // sound
  }
  sleep(3);
}

void move_players(player *p1, player *p2) {
  p1->x += p1XFlag * GRIDSQUARE;
  p1->y += p1YFlag * GRIDSQUARE;
  p2->x += p2XFlag * GRIDSQUARE;
  p2->y += p2YFlag * GRIDSQUARE;
}

void load_game_board() {
  for (int j = 0; j < GRIDYLIMIT; j++) {
    taken[0][j] = 1;
    taken[GRIDXLIMIT - 1][j] = 1;
  }
  // set top and bottom borders as 1
  for (int i = 0; i < GRIDXLIMIT; i++) {
    taken[i][0] = 1;
    taken[i][GRIDYLIMIT - 1] = 1;
  }
  draw_grid();
}

void draw_grid() {
  for (int y = GRIDSQUARE; y < SCREEN_PIXEL_HEIGHT - GRIDSQUARE; y++) {
    for (int x = GRIDSQUARE; x < SCREEN_PIXEL_WIDTH; x += GRIDSQUARE)
      call_to_putpixel(x, y, GRIDCOLOR);
  }
  for (int x = GRIDSQUARE; x < SCREEN_PIXEL_WIDTH - GRIDSQUARE; x++) {
    for (int y = GRIDSQUARE; y < SCREEN_PIXEL_HEIGHT; y += GRIDSQUARE)
      call_to_putpixel(x, y, GRIDCOLOR);
  }
}

// returns 1 if players hit each other or borders
int hit_player(uint32_t p1_currX, uint32_t p1_currY, uint32_t p2_currX,
               uint32_t p2_currY) {

  int p1SqX = p1_currX / GRIDSQUARE, p1SqY = p1_currY / GRIDSQUARE,
      p2SqX = p2_currX / GRIDSQUARE, p2SqY = p2_currY / GRIDSQUARE;
  int p1hit = 0, p2hit = 0;

  // check if player positions havent already been crossed by themselves
  // or the other player, or if theyre out of bounds
  if (taken[p1SqX][p1SqY])
    p1hit = 1;
  else
    taken[p1SqX][p1SqY] = 1;

  if (taken[p2SqX][p2SqY])
    p2hit = 2;
  else
    taken[p2SqX][p2SqY] = 1;
  return p1hit + p2hit;
}

void tron_char(uint8_t c) {
  if (c == 'W' && p1YFlag != DOWN) { // up player 1
    p1YFlag = UP;
    p1XFlag = NONE;
  }
  if (c == 'S' && p1YFlag != UP) { // down player 1
    p1YFlag = DOWN;
    p1XFlag = NONE;
  }
  if (c == 'A' && p1XFlag != RIGHT) { // left player 1
    p1XFlag = LEFT;
    p1YFlag = NONE;
  }
  if (c == 'D' && p1XFlag != LEFT) { // right player 1
    p1XFlag = RIGHT;
    p1YFlag = NONE;
  }
  if (c == 'I' && p2YFlag != DOWN) { // up player 2
    p2YFlag = UP;
    p2XFlag = NONE;
  }
  if (c == 'K' && p2YFlag != UP) { // down player 2
    p2YFlag = DOWN;
    p2XFlag = NONE;
  }
  if (c == 'L' && p2XFlag != LEFT) { // right player 2
    p2XFlag = RIGHT;
    p2YFlag = NONE;
  }
  if (c == 'J' && p2XFlag != RIGHT) { // left player 2
    p2XFlag = LEFT;
    p2YFlag = NONE;
  }
}

void game_over(int whoHit) {
  call_to_fillrectangle(SCREEN_PIXEL_WIDTH / 2 - GRIDSQUARE * 4, 352, BLACK, 96,
                        16);
  call_to_fillrectangle(SCREEN_PIXEL_WIDTH / 2 - GRIDSQUARE, 352, BLACK, 96,
                        16); // x=anterior+GRIDSQUARE*3
  call_to_fillrectangle(SCREEN_PIXEL_WIDTH / 2 + GRIDSQUARE * 2, 352, BLACK, 96,
                        16); // x=anterior+GRIDSQUARE*3
  call_to_setptrx(SCREEN_PIXEL_WIDTH / 2 - GRIDSQUARE * 3);
  call_to_setptry(SCREEN_PIXEL_HEIGHT / 2);
  switch (whoHit) {
  case (1):
    printf("PLAYER 2 WINS!");
    wins[1]++; // increment games won for player 2
    break;
  case (2):
    printf("PLAYER 1 WINS!");
    wins[0]++; // increment games won for player 1
    break;
  case (3):
    printf("ITS A TIE!");
    break;
  default:
    break;
  }
  call_to_setptrx(SCREEN_PIXEL_WIDTH / 2 - GRIDSQUARE * 3);
  call_to_setptry(SCREEN_PIXEL_HEIGHT / 2 + 32);
  printf("press any key");
  while (get_char() == 0)
    ;
}

void tron_menu() {
  hvd_clear();
  call_to_setptrx(400);
  call_to_setptry(200);
  char *msg = "Welcome to TRON";
  while (*msg != 0) {
    putc(*msg++);
    sleepms(30);
  }
  putc('\n');
  putc('\n');
  call_to_setptrx(170);
  printf("Player 1 moves: W(UP) A(LEFT) S(DOWN) D(RIGHT)\n");
  call_to_setptrx(170);
  printf("Player 2 moves: I(UP) J(LEFT) K(DOWN) L(RIGHT)\n");
  putc('\n');
  call_to_setptrx(280);
  printf("Press 'Q' to quit during gameplay");
  putc('\n');
  putc('\n');
  call_to_setptrx(300);
  printf("Press any key to begin playing");

  while (get_char() == 0)
    ;
}
