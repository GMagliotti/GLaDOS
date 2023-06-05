// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <tron.h>

#define NONE 0
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define GRIDSQUARE 32 // largo de un lado de cuadrado de grilla en bytes
#define GRIDXLIMIT (SCREEN_PIXEL_WIDTH / GRIDSQUARE)
#define GRIDYLIMIT (SCREEN_PIXEL_HEIGHT / GRIDSQUARE)
#define TRUE 1
#define FALSE 0
#define COLORP1 0xFF5782
#define COLORP2 0x57FF62
#define GRIDCOLOR 0x656565
#define BLACK 0x00
#define CANTPLAYERS 2 // cantidad de jugadores
#define CANTGAMES 3   // cantidad de partidos

typedef struct player {
  int x;
  int y;
} player;

// grilla de 30x22
static int taken[GRIDXLIMIT][GRIDYLIMIT] = {{0}};
static int wins[CANTPLAYERS];
static int played_games = 0;

void load_game_board();
void draw_grid();
void move_players(player *p1, player *p2);
void clear_taken();

int p1YFlag = UP, p2YFlag = DOWN, p1XFlag = NONE,
    p2XFlag = NONE; // en que direccion de x e y se moverá cada jugador en cada
                    // refresh
// inicialmente se mueven para arriba

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

  // Posiciones de comienzo arbitrarias:
  player p1 = {16 * GRIDSQUARE, SCREEN_PIXEL_HEIGHT - 4 * GRIDSQUARE};
  // player p2 =
  // {SCREEN_PIXEL_WIDTH-12*GRIDSQUARE,SCREEN_PIXEL_HEIGHT-6*GRIDSQUARE};
  player p2 = {16 * GRIDSQUARE, 3 * GRIDSQUARE};

  int whoHit = 0;
  while (!(whoHit = hit_player(p1.x, p1.y, p2.x, p2.y))) {
    call_to_fillrectangle(p1.x, p1.y, COLORP1, GRIDSQUARE, GRIDSQUARE);
    call_to_fillrectangle(p2.x, p2.y, COLORP2, GRIDSQUARE, GRIDSQUARE);

    int tickAnt = call_to_getticks();

    while (call_to_getticks() <= tickAnt + 1) {
      char c = get_char();
      if (c == 'Q') { // tecla para salir de juego
        end_game();
        return; // salgo de juego
      }
      if (c != 0) {
        tron_char(c); // en base al caracter que recibo, cambia los flags de
                      // direcciones
      }
    }

    // movemos a ambos en la direccion que dice su flag
    move_players(&p1, &p2);
    // chequeamos que la posicion a la que quiere ir no lo haria perder (en ese
    // caso no dibujo el cuadrado, end game)
  }
  game_over(whoHit); // imprime ganador de UNA partida (de 3)
                     // volvemos a bash:
  clear_taken();     // seteo array de taken en 0 para proxima vez que se juegue
  p1XFlag = p2XFlag = NONE; // seteo direccion default de comienzo (para arriba)
  p1YFlag = UP;
  p2YFlag = DOWN;
  played_games++;
  if (played_games < CANTGAMES) { // jugar de nuevo
    // llamo devuelta al tron para jugar otro partido
    tron();
  } else {
    print_winner(); // imprime ganador (de las 3 partidas)
    end_game();
  }
}
void end_game() {
  clear_taken();
  p1XFlag = p2XFlag = NONE; // seteo direccion default de comienzo (para arriba)
  p1YFlag = UP;
  p2YFlag = DOWN;
  played_games = 0;
  wins[0] = 0;
  wins[1] = 0;
  return_to_shell();
  call_to_clear_buffer();
}
void clear_taken() {
  for (int i = 0; i < GRIDXLIMIT;
       i++) // seteo array de taken en 0 para proxima vez que se juegue
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
    // ganó el primer jugador
    printf("Player 1 won!");
    victory(); // sonido
  } else if (wins[0] < wins[1]) {
    // gano el segundo jugador
    printf("Player 2 won!");
    victory(); // sonido
  } else {
    // empate!
    printf("Its a tie!");
    mario(); // sonido
  }
  sleep(3);
}

void move_players(player *p1, player *p2) {
  // movemos a ambos en la direccion que dice su flag
  p1->x += p1XFlag * GRIDSQUARE; // si es 1 se suma 32, si es -1 se resta 32
  p1->y += p1YFlag * GRIDSQUARE;
  p2->x += p2XFlag * GRIDSQUARE;
  p2->y += p2YFlag * GRIDSQUARE;
  // chequeamos que la posicion a la que quiere ir no lo haria perder (en ese
  // caso no dibujo el cuadrado, end game)
}

void load_game_board() {
  for (int j = 0; j < GRIDYLIMIT; j++) {
    taken[0][j] = 1;
    taken[GRIDXLIMIT - 1][j] = 1;
  }
  // seteo bordes de arriba y abajo horizontales en 1
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

// funcion que retorna 1 se jugadores chocaron o alguno llego al borde de grilla
int hit_player(uint32_t p1_currX, uint32_t p1_currY, uint32_t p2_currX,
               uint32_t p2_currY) {
  // vemos qué cuadrado de la grilla ocupa cada jugador (coordenada x e y)
  int p1SqX = p1_currX / GRIDSQUARE, p1SqY = p1_currY / GRIDSQUARE,
      p2SqX = p2_currX / GRIDSQUARE, p2SqY = p2_currY / GRIDSQUARE;
  int p1hit = 0, p2hit = 0;

  // chequeo que posiciones de jugadores no sea posicion ya recorrida por ellos
  // mismos o el otro jugador, o esté en zona fuera de grilla de juego
  if (taken[p1SqX][p1SqY])
    p1hit = 1;
  else
    taken[p1SqX][p1SqY] = 1;

  if (taken[p2SqX][p2SqY])
    p2hit = 2;
  else
    taken[p2SqX][p2SqY] = 1;
  return p1hit + p2hit;
  // ninguno choco = 0 ; choco solo p1 = 1; choco solo p2 = 2 ; chocaron p1 y p2
  // = 3
}

void tron_char(uint8_t c) {
  // si quieren moverse en mismo eje, sentido opuesto que ya se estaban
  // moviendo, no se modifica nada
  if (c == 'W' && p1YFlag != DOWN) { // arriba jugador 1
    p1YFlag = UP;
    p1XFlag = NONE;
  }
  if (c == 'S' && p1YFlag != UP) { // abajo jugador 1
    p1YFlag = DOWN;
    p1XFlag = NONE;
  }
  if (c == 'A' && p1XFlag != RIGHT) { // izquierda jugador 1
    p1XFlag = LEFT;
    p1YFlag = NONE;
  }
  if (c == 'D' && p1XFlag != LEFT) { // derecha jugador 1
    p1XFlag = RIGHT;
    p1YFlag = NONE;
  }
  if (c == 'I' && p2YFlag != DOWN) { // arriba jugador 2
    p2YFlag = UP;
    p2XFlag = NONE;
  }
  if (c == 'K' && p2YFlag != UP) { // abajo jugador 2
    p2YFlag = DOWN;
    p2XFlag = NONE;
  }
  if (c == 'L' && p2XFlag != LEFT) { // derecha jugador 2
    p2XFlag = RIGHT;
    p2YFlag = NONE;
  }
  if (c == 'J' && p2XFlag != RIGHT) { // izquierda jugador 2
    p2XFlag = LEFT;
    p2YFlag = NONE;
  }
}

void game_over(int whoHit) {
  // los siguientes valores de x,y, width y height son arbitrarios
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
    wins[1]++; // aumento la cantidad de partidas ganadas que tiene el jugador 2
    break;
  case (2):
    printf("PLAYER 1 WINS!");
    wins[0]++; // aumento la cantidad de partidas ganadas que tiene el jugador 1
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
