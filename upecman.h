/***************************************************************************
 *   upecman.h                                Version 20160529.013231      *
 *                                                                         *
 *   Pacman Ncurses                                                        *
 *   Copyright (C) 2016         by Ruben Carlo Benante                     *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *   To contact the author, please write to:                               *
 *   Ruben Carlo Benante                                                   *
 *   Email: rcb@beco.cc                                                    *
 *   Webpage: http://www.beco.cc                                           *
 *   Phone: +55 (81) 3184-7555                                             *
 ***************************************************************************/

/* ---------------------------------------------------------------------- */
/* includes */
#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Miscellaneous functions (rand, malloc, srand)*/
#include <ncurses.h> /* Screen handling and optimisation functions */
#include <getopt.h> /* get options from system argc/argv */
#include <string.h> /* Strings functions definitions */
#include <assert.h> /* Verify assumptions with assert */
#include <unistd.h> /* UNIX standard function */
#include <time.h> /* Time and date functions */
#include <sys/time.h> /* Time and date functions */
#include <math.h> /* Mathematics functions */
#include <signal.h> /* alarm */


/* ---------------------------------------------------------------------- */
/* definitions */

#ifndef VERSION
#define VERSION "20160529.013231" /**< Version Number */
#endif

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) fprintf(stderr, "[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define SBUFF 256 /**< String buffer */
#define LABL 23 /**< Number of lines in labyrinth */
#define LABC 21 /**< Number of columns in labyrinth */
#define AFRAID_DURATION 5
#define NUM_SAMPLES 1000000
#define FPS 6 /* jogo roda na velocidade de N frames por segundo */

/* ---------------------------------------------------------------------- */
/* globals */
static int verb = 0; /**< verbose level, global within the file */
/* volatile int ffps; /1* flag frame disponivel *1/ */

const char labmodel[LABL][LABC] =
{
    /*01234567890123456789*/
    {"####################"},    /* 0*/
    {"#........##........#"},    /* 1*/
    {"#o##.###.##.###.##o#"},    /* 2*/
    {"#..................#"},    /* 3*/
    {"#.##.#.######.#.##.#"},    /* 4*/
    {"#....#...##...#....#"},    /* 5*/
    {"####.###.##.###.####"},    /* 6*/
    {"   #.#...B....#.#   "},    /* 7*/
    {"   #.#.##--##.#.#   "},    /* 8*/
    {"####.#.#  P #.#.####"},    /* 9*/
    {"    ...#  I #...    "},    /*10*/
    {"####.#.#  C #.#.####"},    /*11*/
    {"   #.#.######.#.#   "},    /*12*/
    {"   #.#........#.#   "},    /*13*/
    {"####.#.######.#.####"},    /*14*/
    {"#........##........#"},    /*15*/
    {"#o##.###.##.###.##o#"},    /*16*/
    {"#..#.....@......#..#"},    /*17*/
    {"##.#.#.######.#.#.##"},    /*18*/
    {"#....#...##...#....#"},    /*19*/
    {"#.######.##.######.#"},    /*20*/
    {"#..................#"},    /*21*/
    {"####################"}     /*22*/
};


typedef enum e_ghostmode {chase, scatter, afraid, dead} t_ghostmode;
typedef enum e_direction {up, left, down, right} t_direction;
typedef enum e_ghosts {blinky, pinky, inky, clyde} t_ghosts;

typedef struct st_position
{
    int y; /* line */
    int x; /* column */
} t_pos;

typedef struct st_criterios
{
    time_t inicio_morte, fim_morte;

} t_criterios;

typedef struct st_timer
{
    time_t start_time; /* tempo assim que programa e executado */
    time_t current_time; /* tempo do loop do jogo */
    int elapsed_seconds; /* tempo decorrido */

} t_timer;

typedef struct st_pacman
{
    t_pos pos; /* current pacman position */
    t_direction dir; /* direction */
    int life; /* how many lifes pacman has */
    int score; /* dot = 10 point; pill = 50 points; ghost = 750 points; cherry = 500 */
    int nivel; /* witch level you are in pacman */
    int contador_pilulas;
} t_pacman;

typedef struct st_ghost
{
    t_pos pos; /* ghost position */
    t_direction dir; /* ghost current direction */
    t_pos starget; /* ghost scatter preferred corner */
    t_ghostmode mode; /* chase, scatter, afraid or dead */
    char conteudo_destino;
    int start_time; /* tempo de início para o fantasma começar a se mover */
} t_ghost;

typedef struct st_pacdata
{
    char lab[LABL][LABC]; /* the labyrinth map */
    t_pacman pacman; /* pacman data */
    t_ghost ghost[4];/* ghost[blinky], ghost[pinky], ghost[inky], ghost[clyde] */
    t_criterios criterios;
    t_timer timer;
    int score;
    int tempo;
    int nivel;
} t_game;


/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
t_game upecman_init(void); /* initialize game variables */
void printlab(t_game g); /* print the labyrinth */
t_game menu(t_game g); /* exibe o menu */
t_game pacmove(t_game g, int kin);
t_game inkymove(t_game g, int elapsed_seconds);
t_game chaseinky(t_game g);
t_game clydemove(t_game g, int elapsed_seconds); //movimento do clyde
t_game chaseclyde(t_game g);
t_game imprime_placar(t_game g, int elapsed_seconds);
void timer();
void init_timer();
void att_alvos(t_game *g, t_ghost *ghost);
void atualizaghost(t_game *g);
void ghostinit(t_game *g);
t_game printlife(t_game g);
t_game respawn_morte(t_game g, int elapsed_seconds);
bool zona_respawn(int y, int x); //Define a area de respawn
bool corredor_esquerdo(int y, int x); // Define o corredor da esquerda
bool corredor_direita(int y, int x); // Define o corredor da direita
bool not_up_re1(int y, int x); //Condição que defini area proibida de fantasma subir
bool not_up_re2(int y, int x); //Condição que defini area proibida de fantasma subir
t_game pinkymove(t_game g, int elapsed_seconds);
t_game chasepinky(t_game g);
t_game blinkymove(t_game g, int elapsed_seconds);
t_game chaseblinky(t_game g);
t_game scatterpinky(t_game g);
t_game scatterinky(t_game g);
t_game scatterblinky(t_game g);
t_game scatterclyde(t_game g);
t_game menupause(t_game g);
t_game respawn_ghost(t_game g);
long tdus(struct timeval w0, struct timeval w1); /* calcula diferenca entre dois tempos w em microsegundos */
t_game game_over(t_game g);
//void tempo(t_game g);
/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20160612.142044      */

