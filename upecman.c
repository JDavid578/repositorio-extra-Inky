/***************************************************************************
 *   upecman.c                                Version 20160529.013231      *
 *                                                                         *
 *   Pacman Ncurses                                                        *
 *   Copyright (C) 2016         by Ruben Carlo Benante                     *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
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
/**
 * @file upecman.c
 * @ingroup Inky
 * @brief Pacman Ncurses
 * @details Jogo do Pacman do grupo inky!
 * @version 20240308.173914
 * @date 2024-03-08
 * @author Ruben Carlo Benante <<rcb@beco.cc>>
 * @author Gustavo Felipe Correia Da Silva Sntos <<gfcss@poli.br>>
 * @author Kaic Marcelo De Brito Silva <<kmbs@poli.br>>
 * @author Leonardo Correia Santos Galvao <<lcsg@poli.br>>
 * @author Vinicius Guilherme Coelho De Sant Ana <<vgcsa@poli.br>>
 * @author Vinicius Dias Novo Braga <<vdnb@poli.br>>
 * @author Jose Gabriel Queiroz De Melo <jgqm@poli.br>>
 * @par Webpage
 * <<a href="http://www.beco.cc">www.beco.cc</a>>
 * @copyright (c) 2016 GNU GPL v2
 * @note This program is free software: you can redistribute it
 * and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA. 02111-1307, USA.
 * Or read it online at <<http://www.gnu.org/licenses/>>.
 *
 */

/*
 * Instrucoes para compilar:
 *      $make
 * ou
 *      $gcc upecman.c -o upecman.x -Wall -lncurses -g -Og
 *          -Wextra -ansi -pedantic-errors -DDEBUG=1
 */

/* ---------------------------------------------------------------------- */
/* includes devem estar no header file (.h) */
#include "upecman.h" /* To be created for this template if needed */

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This is the main event of the evening
 * @details Ladies and Gentleman... It's tiiiime!
 * Fightiiiiing at the blue corner,
 * he, who has compiled more C code than any other adversary in the history,
 * he, who has developed UNIX and Linux, and is an inspiration to maaany languages
 * and compilers, the GNU C Compiler, GCC!
 * Fightiiiiing at the red corner, the challenger, in his first fight, lacking of any
 * valid experience but angrily, blindly, and no doubtfully, will try to
 * compile this program without errors. He, the student, the apprentice,
 * the developer, beco!!
 *
 * @param[in] argc Argument counter
 * @param[in] argv Argument strings (argument values)
 *
 * @retval 0 If succeed (EXIT_SUCCESS).
 * @retval 1 Or another error code if failed.
 *
 * @par Example
 * @code
 *    $./upecman -h
 * @endcode
 *
 * @warning   Be carefull with...
 * @bug There is a bug with...
 * @todo Need to do...
 * @note You can read more about it at <<a href="http://www.beco.cc">www.beco.cc</a>>
 * @author Ruben Carlo Benante
 * @version 20160529.013231
 * @date 2016-05-29
 *
 */
int main(int argc, char *argv[])
{
    srand(time(NULL));
    IFDEBUG("main()\n");
    int opt; /* return from getopt() */
    t_game g; /* the game */
    int kin;/* keyboard input */
    int elapsed_seconds; /* calcula tempo do jogo */
    /* ----------- INICIO - variaveis de ajuste de FPS */
    struct timeval w0, w1; /* ajusta FPS do jogo -- modo mais simples */
    long td = 180000; /* time delay - ajuste de FPS */
    float ofps; /* FPS real -> a ser ajustado para ficar no desejado */
    long dt; /* diff time */
    int deltatd = 1000; /* quanto subtrai por rodada com decaimento */
    /* ----------- FIM - variaveis de ajuste de FPS */

    IFDEBUG("Starting optarg loop...\n");
    /* getopt() configured options:
     *        -h  help
     *        -c  copyright & version
     *        -v  verbose
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "hvc")) != EOF)
        switch(opt)
        {
            case 'h':
                help();
                break;
            case 'v':
                verb++;
                break;
            case 'c':
                copyr();
                break;
            case '?':
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    if(verb)
        printf("Verbose level set at: %d\n", verb);

    IFDEBUG("Starting the game now...\n");

    g = upecman_init(); // inicia o jogo
    g = menu(g); // imprime o menu
    g.timer.start_time = time(NULL); //define o tempo inicial do jogo para zero
    printlab(g); // imprime o labirinto
    printlife(g); // imprime numero de vidas
    refresh();

    do
    {
        gettimeofday(&w0, NULL);
        /* tempo a partir do inicio do jogo */
        g.timer.current_time = time(NULL);
        elapsed_seconds = difftime(g.timer.current_time, g.timer.start_time);

        usleep(1667);
        kin = getch(); /* deveria ser o unico getch do programa todo */
        if(kin == 'p')
        {
            g = menupause(g);
            printlab(g);
        }
        /* caso queria ter o valor do  ffps impreso na tela tirar termos do comentario
        mvprintw(5, 22, "|DEBUG: kin = %d|", kin);
        mvprintw(6, 22, "|DEBUGi: offps = %f|", ofps); */
        mvprintw(9, 22, "*-----------------------*");
        mvprintw(10, 22, "|Pressione p para pausar|");
        mvprintw(11, 22, "*-----------------------*");

        // Movimentação Personagens
        g = blinkymove(g, elapsed_seconds);
        g = pinkymove(g, elapsed_seconds);
        g = inkymove(g, elapsed_seconds);
        g = clydemove(g, elapsed_seconds);
        g = respawn_morte(g, elapsed_seconds);
        g = respawn_ghost(g);
        g = pacmove(g, kin);
        //ghostinit(&g);
        g = respawn_morte(g, elapsed_seconds);
        g = imprime_placar(g, elapsed_seconds);
        //t_game imprime_nivel(t_game g);
        //t_game proximo_nivel(t_game g);
        g = respawn_ghost(g);
        //tempo(g);
        if(g.pacman.life <= 0)
            g = game_over(g);
        flushinp();
        /* ------------------------ INICIO - ajuste de FPS */
        usleep(td);
        gettimeofday(&w1, NULL);
        dt = tdus(w0, w1);
        ofps = 1000000.0 / dt;
        if(ofps > FPS)
            td += deltatd;
        else
            td -= deltatd;
        if(deltatd > 100)
            deltatd--;
        /* ------------------------ FIM - ajuste de FPS */
    }
    while(kin != 'q');

    /* write your code here */

    endwin();
    return EXIT_SUCCESS;
}

/* ---------------------------------------------------------------------- */
t_game menu(t_game g)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();

    int altura, largura, ymx, xmx;
    getmaxyx(stdscr, ymx, xmx);

    altura = 10;
    largura = 120;
    WINDOW *jan = newwin(altura, largura, (ymx / 4), xmx = 0);
    refresh();
    /*--------------------------------------logo----------------------------*/
    //u
    wattron(jan, COLOR_PAIR(7));
    mvwprintw(jan, 1, 0, "   ");
    mvwprintw(jan, 2, 0, "   ");
    mvwprintw(jan, 3, 0, "   ");
    mvwprintw(jan, 4, 0, "   ");
    mvwprintw(jan, 5, 0, "   ");
    mvwprintw(jan, 6, 0, "             ");
    mvwprintw(jan, 7, 3, "       ");
    mvwprintw(jan, 1, 10, "   ");
    mvwprintw(jan, 2, 10, "   ");
    mvwprintw(jan, 3, 10, "   ");
    mvwprintw(jan, 4, 10, "   ");
    mvwprintw(jan, 5, 10, "   ");
    wattroff(jan, COLOR_PAIR(7));
    wattron(jan, COLOR_PAIR(8));
    //p
    mvwprintw(jan, 1, 15, "   ");
    mvwprintw(jan, 2, 15, "   ");
    mvwprintw(jan, 3, 15, "   ");
    mvwprintw(jan, 4, 15, "   ");
    mvwprintw(jan, 5, 15, "   ");
    mvwprintw(jan, 6, 15, "   ");
    mvwprintw(jan, 7, 15, "   ");
    mvwprintw(jan, 1, 16, "          ");
    mvwprintw(jan, 4, 16, "          ");
    mvwprintw(jan, 2, 24, "  ");
    mvwprintw(jan, 3, 24, "  ");
    //e
    mvwprintw(jan, 1, 28, "         ");
    mvwprintw(jan, 2, 28, "         ");
    mvwprintw(jan, 3, 28, "   ");
    mvwprintw(jan, 4, 28, "       ");
    mvwprintw(jan, 5, 28, "   ");
    mvwprintw(jan, 6, 28, "         ");
    mvwprintw(jan, 7, 28, "         ");
    wattroff(jan, COLOR_PAIR(8));
    //c
    wattron(jan, COLOR_PAIR(9));
    mvwprintw(jan, 3, 39, "  ");
    mvwprintw(jan, 4, 39, "  ");
    mvwprintw(jan, 5, 39, "  ");
    mvwprintw(jan, 2, 41, "  ");
    mvwprintw(jan, 3, 41, "  ");
    mvwprintw(jan, 4, 41, "  ");
    mvwprintw(jan, 5, 41, "  ");
    mvwprintw(jan, 6, 41, "  ");
    mvwprintw(jan, 1, 43, "      ");
    mvwprintw(jan, 2, 43, "    ");
    mvwprintw(jan, 3, 43, "  ");
    mvwprintw(jan, 5, 43, "  ");
    mvwprintw(jan, 6, 43, "    ");
    mvwprintw(jan, 7, 43, "      ");
    wattroff(jan, COLOR_PAIR(9));
    //M
    wattron(jan, COLOR_PAIR(8));
    mvwprintw(jan, 1, 51, "             ");
    mvwprintw(jan, 2, 51, "             ");
    mvwprintw(jan, 3, 51, "   ");
    mvwprintw(jan, 4, 51, "   ");
    mvwprintw(jan, 5, 51, "   ");
    mvwprintw(jan, 6, 51, "   ");
    mvwprintw(jan, 7, 51, "   ");
    mvwprintw(jan, 1, 63, "   ");
    mvwprintw(jan, 2, 63, "   ");
    mvwprintw(jan, 3, 63, "   ");
    mvwprintw(jan, 4, 63, "   ");
    mvwprintw(jan, 5, 63, "   ");
    mvwprintw(jan, 6, 63, "   ");
    mvwprintw(jan, 7, 63, "   ");
    mvwprintw(jan, 3, 57, "   ");
    mvwprintw(jan, 4, 57, "   ");
    mvwprintw(jan, 5, 57, "   ");
    //A
    mvwprintw(jan, 1, 69, "           ");
    mvwprintw(jan, 2, 68, "   ");
    mvwprintw(jan, 3, 68, "   ");
    mvwprintw(jan, 4, 68, "          ");
    mvwprintw(jan, 5, 68, "   ");
    mvwprintw(jan, 6, 68, "   ");
    mvwprintw(jan, 7, 68, "   ");
    mvwprintw(jan, 2, 78, "   ");
    mvwprintw(jan, 3, 78, "   ");
    mvwprintw(jan, 4, 78, "   ");
    mvwprintw(jan, 5, 78, "   ");
    mvwprintw(jan, 6, 78, "   ");
    mvwprintw(jan, 7, 78, "   ");
    //N
    mvwprintw(jan, 1, 83, "      ");
    mvwprintw(jan, 2, 83, "        ");
    mvwprintw(jan, 3, 83, "   ");
    mvwprintw(jan, 3, 89, "   ");
    mvwprintw(jan, 1, 89, " ");
    mvwprintw(jan, 4, 83, "   ");
    mvwprintw(jan, 5, 83, "   ");
    mvwprintw(jan, 6, 83, "   ");
    mvwprintw(jan, 7, 83, "   ");
    mvwprintw(jan, 4, 89, "   ");
    mvwprintw(jan, 5, 91, "   ");
    mvwprintw(jan, 6, 91, "   ");
    mvwprintw(jan, 7, 93, "   ");
    mvwprintw(jan, 6, 94, "   ");
    mvwprintw(jan, 1, 95, "   ");
    mvwprintw(jan, 2, 95, "   ");
    mvwprintw(jan, 3, 95, "   ");
    mvwprintw(jan, 4, 95, "   ");
    mvwprintw(jan, 5, 95, "   ");
    mvwprintw(jan, 6, 95, "   ");
    mvwprintw(jan, 7, 95, "   ");
    wattroff(jan, COLOR_PAIR(8));
    //subtexto
    wattron(jan, A_BOLD);
    wattron(jan, COLOR_PAIR(6));
    mvwprintw(jan, 9, 40, " INKY ");
    wattroff(jan, COLOR_PAIR(6));

    wattron(jan, COLOR_PAIR(1));
    mvwprintw(jan, 9, 45, " EDITION ");
    wattroff(jan, COLOR_PAIR(1));
    wattroff(jan, A_BOLD);
    wrefresh(jan);
    refresh();
    getch();
    /*--------------------------------------menu------------------------------------*/

    int i = 0;
    int destaque = 0;
    int escolha = 0;
    int ymax, xmax;
    char opcoes[3][10] = {"Jogar", "Tutorial", "Sair"};

    WINDOW *janela = NULL;

    while(1)
    {
        getmaxyx(stdscr, ymax, xmax);
        if(janela != NULL)
            delwin(janela);
        janela = newwin(7, (xmax - 6), (ymax - 9), 3);
        box(janela, 0, 0);
        keypad(janela, true);
        werase(janela);
        box(janela, 0, 0);
        wrefresh(janela);

        for(i = 0; i < 3; i++)
        {
            mvwprintw(janela, 1, 1, "Escolha uma opção:");
            if(i == destaque) wattron(janela, A_REVERSE);
            mvwprintw(janela, i + 2, 1, "%s", opcoes[i]);
            wattroff(janela, A_REVERSE);
        }

        escolha = wgetch(janela);
        switch(escolha)
        {
            case KEY_UP:
                destaque--;
                if(destaque == -1)
                    destaque = 0;
                break;

            case KEY_DOWN:
                destaque++;
                if(destaque == 3)
                    destaque = 2;
                break;

            default:
                break;
        }

        if(escolha == 10 && destaque == 0)
        {
            clear();
            g = upecman_init();
            g.timer.start_time = time(NULL);
            return g;
        }

        if(escolha == 10 && destaque == 2)
        {
            endwin();
            exit(0);
        }

        if(escolha == 10 && destaque == 1)
        {
            clear();

            mvprintw(4, (xmax / 2) - 15, "Bem-vindo ao Tutorial!");
            mvchgat(4, (xmax / 2) - 15, -1, A_BOLD, 6, NULL);
            attron(COLOR_PAIR(6));

            // Seção de Objetivos do Jogo
            mvprintw(7, (xmax / 2) - 40, "Objetivo: Ajude o Pacman (@) a comer todas as pastilhas do labirinto.");

            // Seção de Mecânicas do Jogo
            mvprintw(9, (xmax / 2) - 40, "Evite os fantasmas (I, P, B, C) e colete pastilhas para ganhar pontos.");
            mvprintw(11, (xmax / 2) - 40, "Coma pílulas para temporariamente ganhar o poder de comer fantasmas.");


            // Seção de Pontuação e Vidas
            mvprintw(11, (xmax / 2) - 40, "Pontuação: Pastilhas (10 pontos), Pílulas (50 pontos), Fantasmas (500 pontos).");
            mvprintw(13, (xmax / 2) - 40, "Vidas: Você tem 3 vidas. Evite ser pego pelos fantasmas a qualquer custo!");

            // Controles
            mvprintw(15, (xmax / 2) - 40, "Controles: Use as setas do teclado para mover o Pacman.");

            //desenhos
            mvprintw(17, (xmax / 2) - 30, "  _____  ____   ____  _________   ____  ______ ");
            mvprintw(18, (xmax / 2) - 30, " | '_ | / _  | / ___| |_  v   _| /  _`| | '_' |");
            mvprintw(19, (xmax / 2) - 30, " | |_)| |(_| | |(___  |  |--|  | | (_|| | | | | ");
            mvprintw(20, (xmax / 2) - 30, " | .__/ |____| |____| |__|  |__| |____| |_| |_|");
            mvprintw(21, (xmax / 2) - 30, " |_| ");
            mvprintw(22, (xmax / 2) - 20, " Made by bagriel KaIaKo Lcsg");

            attroff(COLOR_PAIR(6));

            refresh();
            getch();

        }
        mvprintw(2, 7, "Você escolheu: %s", opcoes[destaque]);

        endwin();
    }
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints help information and exit
 * @details Prints help information (usually called by opt -h)
 * @return Void
 * @author Ruben Carlo Benante
 * @version 20160529.013231
 * @date 2016-05-29
 *
 */
void help(void)
{
    IFDEBUG("help()");
    printf("%s - %s\n", "upecman", "Pacman Ncurses");
    printf("\nUsage: ./%s [-h|-v|-c]\n", "upecman");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --copyright, --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    /* add more options here */
    printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    printf("\nTodo:\n\tLong options not implemented yet.\n");
    printf("\nAuthor:\n\tWritten by %s <%s>\n\n", "Ruben Carlo Benante", "rcb@beco.cc");
    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints version and copyright information and exit
 * @details Prints version and copyright information (usually called by opt -V)
 * @return Void
 * @author Ruben Carlo Benante
 * @version 20160529.013231
 * @date 2016-05-29
 *
 */
void copyr(void)
{
    IFDEBUG("copyr()");
    printf("%s - Version %s\n", "upecman", VERSION);
    printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 <http://gnu.org/licenses/gpl.html>. This  is  free  software:  you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2016, "Ruben Carlo Benante", "rcb@beco.cc");
    if(verb > 3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This function initializes some operations before start
 * @details Details to be written
 * @return Void
 * @todo Need to implement it. Its empty now.
 * @author Ruben Carlo Benante
 * @version 20160530.224016
 * @date 2016-05-30
 *
 */
t_game upecman_init(void)
{
    IFDEBUG("init()");
    /* initialization */
    t_game g;
    int f, y;

    for(y = 0; y < LABL; y++)
        strcpy(g.lab[y], labmodel[y]); /* copy the labyrinth (lines of strings) */

    g.pacman.pos.y = 17; /* pacman start position (line) */
    g.pacman.pos.x = 9; /* pacman start position (column) */
    g.pacman.dir = left; /* pacman start direction */
    g.pacman.life = 3; /* 3 lifes */
    g.pacman.score = 0; /* 0 points */
    g.pacman.nivel = 1;
    g.score = 0;
    g.nivel = 1;
    g.tempo = 0;
    g.pacman.contador_pilulas = 183;
    g.ghost[0].start_time = 0; /* Blinky sempre esta se movendo */
    g.ghost[1].start_time = 10; /* Pinky se move apos 10 segundos */
    g.ghost[2].start_time = 20; /* Inky se move apos 20 segundos */
    g.ghost[3].start_time = 30; /* Clyde se move apos 30 segundos */
    g.ghost[0].conteudo_destino = ' ';
    g.ghost[1].conteudo_destino = ' ';
    g.ghost[2].conteudo_destino = ' ';
    g.ghost[3].conteudo_destino = ' ';
    g.timer.start_time = time(NULL);
    g.lab[7][9] = ' ';
    g.lab[9][10] = ' ';
    g.lab[10][10] = ' ';
    g.lab[11][10] = ' ';
    g.lab[17][9] = ' ';

    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        switch(f)
        {
            case blinky:
                g.ghost[f].pos.y = 7; /* blinky start position (line) */
                g.ghost[f].pos.x = 9; /* blinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case pinky:
                g.ghost[f].pos.y = 9; /* pinky start position (line) */
                g.ghost[f].pos.x = 10; /* pinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
                break;
            case inky:
                g.ghost[f].pos.y = 10; /* inky start position (line) */
                g.ghost[f].pos.x = 10; /* inky start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case clyde:
                g.ghost[f].pos.y = 11; /* clyde start position (line) */
                g.ghost[f].pos.x = 10; /* clyde start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
        }
        g.ghost[f].dir = left; /* start direction and future direction: left */
        g.ghost[f].mode = chase;
    }

    initscr(); /* start ncurses mode screen */
    cbreak(); /* stop line buffering */
    keypad(stdscr, TRUE); /* grab extended keys */
    noecho(); /* stop echoing chars */
    timeout(0); /* do not wait */
    start_color(); /* initialize the color functionality */

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK); /* Blinky */
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); /* Pinky */
    init_pair(3, COLOR_CYAN, COLOR_BLACK); /* Inky */
    init_pair(4, COLOR_GREEN, COLOR_BLACK); /* Clyde doesn't have orange :( */
    init_pair(5, COLOR_BLUE, COLOR_BLACK); /* Afraid mode: blue with A_BLINK */
    init_pair(6, COLOR_YELLOW, COLOR_BLACK); /* Pacman */
    init_pair(7, COLOR_RED, COLOR_RED);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);
    init_pair(9, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(10, COLOR_WHITE, COLOR_RED);
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    return g;
}
/* -----------------------------Movimentação Do Pacman----------------------------------- */

t_game pacmove(t_game g, int kin)
{
    int direction = kin; // Define a direção inicial com base na entrada
    static int premove = 0;
    int prev_y, prev_x;
    /* loop infinito para movimento contínuo até que uma direção diferente seja selecionada */
    prev_y = g.pacman.pos.y;
    prev_x = g.pacman.pos.x;
    /* calcula a proxima posicao com base na direcao atual */
    int next_y = g.pacman.pos.y;
    int next_x = g.pacman.pos.x;

    if(direction == KEY_UP)
        premove = up;
    if(direction == KEY_DOWN)
        premove = down;
    if(direction == KEY_LEFT)
        premove = left;
    if(direction == KEY_RIGHT)
        premove = right;

    /* // Se a direção anterior for ' up down left right' e a próxima posição não for uma parede, define a direção do Pacman no mesmo sentido */
    if(premove == up && g.lab[next_y - 1][next_x] != '#' && g.lab[next_y - 1][next_x] != '-')
        g.pacman.dir = up;

    if(premove == down && g.lab[next_y + 1][next_x] != '#' && g.lab[next_y + 1][next_x] != '-')
        g.pacman.dir = down;

    if(premove == left && g.lab[next_y][next_x - 1] != '#' && g.lab[next_y][next_x - 1] != '-')
        g.pacman.dir = left;

    if(premove == right && g.lab[next_y][next_x + 1] != '#' && g.lab[next_y][next_x + 1] != '-')
        g.pacman.dir = right;

    /* Se a direção do Pacman for no sentido x e a próxima posição não for uma parede, move o Pacman na direção x */
    if(g.pacman.dir == up && g.lab[next_y - 1][next_x] != '#')
        next_y--;
    if(g.pacman.dir == down && g.lab[next_y + 1][next_x] != '#')
        next_y++;
    if(g.pacman.dir == left && g.lab[next_y][next_x - 1] != '#')
        next_x--;
    if(g.pacman.dir == right && g.lab[next_y][next_x + 1] != '#')
        next_x++;

    /* verifica se a proxima posicao nao e uma parede */
    /* move o Pacman para a próxima posição */
    g.pacman.pos.y = next_y;
    g.pacman.pos.x = next_x;

    /* atualiza a pontuação se o Pacman comer um ponto */
    if(g.lab[g.pacman.pos.y][g.pacman.pos.x] == '.')
    {
        g.lab[g.pacman.pos.y][g.pacman.pos.x] = ' ';
        g.pacman.score += 10;
        g.pacman.contador_pilulas--;

        // Se não há mais pastilhas, reinicia o nível e adiciona uma vida ao Pac-Man
        if(g.pacman.contador_pilulas == 0)
        {
            int score_atual = g.pacman.score;  // Salva a pontuação atual
            g = upecman_init();
            g.pacman.score = score_atual;
            g.pacman.life++;
            g.pacman.nivel++;
            clear();
            refresh();
            printlab(g);
            g.pacman.contador_pilulas = 183;
            return g;
        }
    }


    /*atualiza a pontuação se o pacman comer uma pilula */
    if(g.lab[g.pacman.pos.y][g.pacman.pos.x] == 'o')
    {
        g.lab[g.pacman.pos.y][g.pacman.pos.x] = ' ';
        g.pacman.score += 50;

        for(int n = 0; n < 4; n++)
        {
            g.ghost[n].mode = afraid;
            if(n == 3) // Se o fantasma é o Clyde
                init_pair(4, COLOR_BLUE, COLOR_BLACK);
        }
        time(&g.criterios.inicio_morte);
    }

    /* teleporta o Pacman se ele alcançar a borda da tela */
    if(g.pacman.pos.x == 0 && g.pacman.pos.y == 10)
        g.pacman.pos.x = 17;
    if(g.pacman.pos.x == 18 && g.pacman.pos.y == 10)
        g.pacman.pos.x = 1;

    /* atualiza a posição do pacman na tela */
    start_color();
    mvaddch(prev_y, prev_x, ' ');// apaga o caractere na posição anteri
    attron(COLOR_PAIR(6));
    mvaddch(g.pacman.pos.y, g.pacman.pos.x, '@');
    attroff(COLOR_PAIR(6));
    mvchgat(g.pacman.pos.y, g.pacman.pos.x, 1, A_BOLD, 6, NULL);
    refresh();
    return g;
}

t_game respawn_ghost(t_game g)
{

    int f;
    double diferença;

    time(&g.criterios.fim_morte);
    diferença = difftime(g.criterios.fim_morte, g.criterios.inicio_morte);

    if(diferença >= 5)
    {
        for(int o = 0; o < 4; o++)
        {
            switch(o)
            {
                case 0:
                    g.ghost[o].mode = chase;
                    g = chaseblinky(g);
                    init_pair(1, COLOR_RED, COLOR_BLACK);
                    break;
                case 1:
                    g.ghost[o].mode = chase;
                    g = chasepinky(g);
                    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
                    break;
                case 2:
                    g.ghost[o].mode = chase;
                    g = chaseinky(g);
                    init_pair(3, COLOR_CYAN, COLOR_BLACK);
                    break;
                case 3:
                    g.ghost[o].mode = chase;
                    g = chaseclyde(g);
                    init_pair(4, COLOR_GREEN, COLOR_BLACK);
                    break;

            }
        }
    }


    for(int m = 0; m < 4; m++)
    {
        if(g.ghost[m].mode == afraid || g.ghost[m].mode == scatter)
        {
            switch(m)
            {
                case 0:
                    init_pair(1, COLOR_BLUE, COLOR_BLACK);
                    g = scatterblinky(g);
                    break;

                case 1:
                    init_pair(2, COLOR_BLUE, COLOR_BLACK);
                    g = scatterpinky(g);
                    break;

                case 2:
                    init_pair(3, COLOR_BLUE, COLOR_BLACK);
                    g = scatterinky(g);
                    break;

                case 3:
                    init_pair(4, COLOR_BLUE, COLOR_BLACK);
                    g = scatterclyde(g);
                    break;

            }
        }
    }
    for(int i = 0; i <= 3; i++)
    {
        if(g.pacman.pos.y == g.ghost[i].pos.y && g.pacman.pos.x == g.ghost[i].pos.x && g.ghost[i].mode == afraid)
        {
            int prev_y = g.ghost[i].pos.y;
            int prev_x = g.ghost[i].pos.x;
            f = i;

            switch(f)
            {

                case 0:
                    g.ghost[f].pos.y = 7;
                    g.ghost[f].pos.x = 9;
                    g.ghost[f].starget.y = 0;
                    g.ghost[f].starget.x = 19;
                    g.ghost[f].conteudo_destino = ' ';
                    g.ghost[f].mode = chase;
                    init_pair(1, COLOR_RED, COLOR_BLACK);
                    g.pacman.score += 500;
                    if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                        mvaddch(prev_y, prev_x, ' ');
                    break;
                case 1:
                    g.ghost[f].pos.y = 9;
                    g.ghost[f].pos.x = 10;
                    g.ghost[f].starget.y = 0;
                    g.ghost[f].starget.x = 0;
                    g.ghost[f].conteudo_destino = ' ';
                    g.ghost[f].mode = chase;
                    g.pacman.score += 500;
                    if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                        mvaddch(prev_y, prev_x, ' ');
                    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
                    break;
                case 2:
                    g.ghost[f].pos.y = 10;
                    g.ghost[f].pos.x = 10;
                    g.ghost[f].starget.y = 22;
                    g.ghost[f].starget.x = 19;
                    g.ghost[f].conteudo_destino = ' ';
                    g.ghost[f].mode = chase;
                    g.pacman.score += 500;
                    if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                        mvaddch(prev_y, prev_x, ' ');
                    init_pair(3, COLOR_CYAN, COLOR_BLACK);
                    break;
                case 3:
                    g.ghost[f].pos.y = 11;
                    g.ghost[f].pos.x = 10;
                    g.ghost[f].starget.y = 22;
                    g.ghost[f].starget.x = 0;
                    g.ghost[f].conteudo_destino = ' ';
                    g.ghost[f].mode = chase;
                    init_pair(4, COLOR_GREEN, COLOR_BLACK);
                    g.pacman.score += 500;
                    if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                        mvaddch(prev_y, prev_x, ' ');
                    break;
            }
            g.ghost[f].dir = up;
            g.ghost[f].mode = chase;

        }

    }

    return g;
}


t_game pinkymove(t_game g, int elapsed_seconds)
{
    if(elapsed_seconds < g.ghost[1].start_time)
        return g;
    int prev_y, prev_x;
    int movev[2] = {up, down};
    int moveh[2] = {left, right};

    prev_y = g.ghost[1].pos.y;
    prev_x = g.ghost[1].pos.x;

    /* calcula a proxima posicao com base na direcao atual */
    int next_y = g.ghost[1].pos.y;
    int next_x = g.ghost[1].pos.x;

    if(g.lab[next_y - 1][next_x] == '-')
        g.ghost[1].dir = up;

    if(g.ghost[1].dir == up)
    {
        if(g.lab[next_y - 1][next_x] != '#')
            next_y--;
        else
        {
            g.ghost[1].dir = moveh[rand() % 2];
            if((g.ghost[1].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[1].dir == right && g.lab[next_y][next_x + 1] == '#'))
            {
                next_x--;
                g.ghost[1].dir = left;
            }
            else
            {
                next_x++;
                g.ghost[1].dir = right;
            }
        }
    }

    else
        if(g.ghost[1].dir == down)
        {
            if(g.lab[next_y + 1][next_x] != '#')
                next_y++;
            else
            {
                g.ghost[1].dir = moveh[rand() % 2];
                if((g.ghost[1].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[3].dir == right && g.lab[next_y][next_x + 1] == '#'))
                {
                    next_x--;
                    g.ghost[1].dir = left;
                }
                else
                {
                    next_x++;
                    g.ghost[1].dir = right;
                }
            }
        }

        else
            if(g.ghost[1].dir == left)
            {
                if(g.lab[next_y][next_x - 1] != '#')
                    next_x--;
                else
                {
                    g.ghost[1].dir = movev[rand() % 2];
                    if((g.ghost[1].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[1].dir == down && g.lab[next_y + 1][next_x] == '#'))
                    {
                        next_y--;
                        g.ghost[1].dir = up;
                    }
                    else
                    {
                        next_y++;
                        g.ghost[1].dir = down;
                    }
                }
            }

            else
                if(g.ghost[1].dir == right)
                {
                    if(g.lab[next_y][next_x + 1] != '#')
                        next_x++;
                    else
                    {
                        g.ghost[1].dir = movev[rand() % 2];
                        if((g.ghost[1].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[1].dir == down && g.lab[next_y + 1][next_x] == '#'))
                        {
                            next_y--;
                            g.ghost[1].dir = up;
                        }
                        else
                        {
                            next_y++;
                            g.ghost[1].dir = down;
                        }
                    }
                }
    if(g.lab[next_y][next_x] != '#')
    {
        g.ghost[1].pos.y = next_y;
        g.ghost[1].pos.x = next_x;
    }

    /* teleporta o fantasma se ele alcançar a borda da tela */
    if(g.ghost[1].pos.x == 0 && g.ghost[1].pos.y == 10)
        g.ghost[1].pos.x = 17;
    if(g.ghost[1].pos.x == 18 && g.ghost[1].pos.y == 10)
        g.ghost[1].pos.x = 1;

    /* atualiza a posição do pacman na tela */
    if(prev_y != g.ghost[1].pos.y || prev_x != g.ghost[1].pos.x)
    {
        start_color();
        mvaddch(prev_y, prev_x, g.lab[prev_y][prev_x]);
        attron(COLOR_PAIR(2));
        mvaddch(g.ghost[1].pos.y, g.ghost[1].pos.x, 'P');
        attroff(COLOR_PAIR(2));
        mvchgat(g.ghost[1].pos.y, g.ghost[1].pos.x, 1, A_BOLD, 2, NULL);
    }
    refresh();
    return g;
}

/* chase pinky */
t_game chasepinky(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int ypink = g.ghost[1].pos.y, xpink = g.ghost[1].pos.x;

    /* Calcula a distancia entre Pinky e Pac-Man */
    int distance = abs(ypac - ypink) + abs(xpac - xpink);

    /* Se a distancia for maior que 5, Pinky persegue antecipadamente, caso contrario, persegue como Blinky */
    if(distance > 5)
    {
        /* Determinar o alvo antecipado de Pinky */
        int targetY, targetX;

        switch(g.pacman.dir)
        {
            case up:
                targetY = ypac - 4;
                targetX = xpac;
                break;
            case down:
                targetY = ypac + 4;
                targetX = xpac;
                break;
            case left:
                targetY = ypac;
                targetX = xpac - 4;
                break;
            case right:
                targetY = ypac;
                targetX = xpac + 4;
                break;
            default:
                /* Caso padrao: seguir o Pac-Man */
                targetY = ypac;
                targetX = xpac;
                break;
        }

        /* Mover Pinky em direcao ao alvo antecipado */
        if(ypink <= targetY && ypink >= targetY - 5 && g.lab[targetY - 1][xpink] != '#' && xpink == targetX)
            g.ghost[1].dir = up;

        else
            if(ypink >= targetY && ypink <= targetY + 5 && g.lab[targetY + 1][xpink] != '#' && xpink == targetX)
                g.ghost[1].dir = down;

            else
                if(xpink >= targetX - 5 && xpink <= targetX && g.lab[ypink][targetX - 1] != '#' && ypink == targetY)
                    g.ghost[1].dir = left;

                else
                    if(xpink >= targetX && xpink <= targetX + 5 && g.lab[ypink][targetX + 1] != '#' && ypink == targetY)
                        g.ghost[1].dir = right;
    }
    else
    {
        /* Se a distancia for 5 ou menos, Pinky persegue como Blinky */
        if(ypac <= ypink && ypac >= ypink - 5 && g.lab[ypink - 1][xpink] != '#' && xpac == xpink)
            g.ghost[1].dir = up;

        else
            if(ypac >= ypink && ypac <= ypink + 5 && g.lab[ypink + 1][xpink] != '#' && xpac == xpink)
                g.ghost[1].dir = down;

            else
                if(xpac >= xpink - 5 && xpac <= xpink && g.lab[ypink][xpink - 1] != '#' && ypac == ypink)
                    g.ghost[1].dir = left;

                else
                    if(xpac >= xpink && xpac <= xpink + 5 && g.lab[ypink][xpink + 1] != '#' && ypac == ypink)
                        g.ghost[1].dir = right;
    }

    return g;
}

//GHOST INKY
t_game inkymove(t_game g, int elapsed_seconds)
{
    if(elapsed_seconds < g.ghost[2].start_time)
        return g;
    int prev_y, prev_x;
    int movev[2] = {up, down};
    int moveh[2] = {left, right};

    prev_y = g.ghost[2].pos.y;
    prev_x = g.ghost[2].pos.x;

    /* calcula a proxima posicao com base na direcao atual */
    int next_y = g.ghost[2].pos.y;
    int next_x = g.ghost[2].pos.x;

    if(g.lab[next_y - 1][next_x] == '-')
        g.ghost[2].dir = up;

    if(g.ghost[2].dir == up)
    {
        if(g.lab[next_y - 1][next_x] != '#')
            next_y--;
        else
        {
            g.ghost[2].dir = moveh[rand() % 2];
            if((g.ghost[2].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[2].dir == right && g.lab[next_y][next_x + 1] == '#'))
            {
                next_x--;
                g.ghost[2].dir = left;
            }
            else
            {
                next_x++;
                g.ghost[2].dir = right;
            }
        }
    }

    else
        if(g.ghost[2].dir == down)
        {
            if(g.lab[next_y + 1][next_x] != '#')
                next_y++;
            else
            {
                g.ghost[2].dir = moveh[rand() % 2];
                if((g.ghost[2].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[2].dir == right && g.lab[next_y][next_x + 1] == '#'))
                {
                    next_x--;
                    g.ghost[2].dir = left;
                }
                else
                {
                    next_x++;
                    g.ghost[2].dir = right;
                }
            }
        }

        else
            if(g.ghost[2].dir == left)
            {
                if(g.lab[next_y][next_x - 1] != '#')
                    next_x--;
                else
                {
                    g.ghost[2].dir = movev[rand() % 2];
                    if((g.ghost[2].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[2].dir == down && g.lab[next_y + 1][next_x] == '#'))
                    {
                        next_y--;
                        g.ghost[2].dir = up;
                    }
                    else
                    {
                        next_y++;
                        g.ghost[2].dir = down;
                    }
                }
            }

            else
                if(g.ghost[2].dir == right)
                {
                    if(g.lab[next_y][next_x + 1] != '#')
                        next_x++;
                    else
                    {
                        g.ghost[2].dir = movev[rand() % 2];
                        if((g.ghost[2].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[2].dir == down && g.lab[next_y + 1][next_x] == '#'))
                        {
                            next_y--;
                            g.ghost[2].dir = up;
                        }
                        else
                        {
                            next_y++;
                            g.ghost[2].dir = down;
                        }
                    }
                }
    if(g.lab[next_y][next_x] != '#')
    {
        g.ghost[2].pos.y = next_y;
        g.ghost[2].pos.x = next_x;
    }

    /* teleporta o fantasma se ele alcançar a borda da tela */
    if(g.ghost[2].pos.x == 0 && g.ghost[2].pos.y == 10)
        g.ghost[2].pos.x = 17;
    if(g.ghost[2].pos.x == 18 && g.ghost[2].pos.y == 10)
        g.ghost[2].pos.x = 1;

    /* atualiza a posição do pacman na tela */
    if(prev_y != g.ghost[2].pos.y || prev_x != g.ghost[2].pos.x)
    {
        start_color();
        mvaddch(prev_y, prev_x, g.lab[prev_y][prev_x]);
        attron(COLOR_PAIR(3));
        mvaddch(g.ghost[2].pos.y, g.ghost[2].pos.x, 'I');
        attroff(COLOR_PAIR(3));
        mvchgat(g.ghost[2].pos.y, g.ghost[2].pos.x, 1, A_BOLD, 3, NULL);
    }
    refresh();
    return g;
}

t_game chaseinky(t_game g)
{
    int target_x = g.pacman.pos.x + 4;
    int target_y = g.pacman.pos.y + 4;

    int dy = abs(target_y - g.ghost[2].pos.y);
    int dx = abs(target_x - g.ghost[2].pos.x);

    if(g.ghost[2].mode == afraid)
    {
        dy = abs(g.ghost[2].pos.y - target_y);
        dx = abs(g.ghost[2].pos.x - target_x);
    }

    if(dx > dy)
    {
        if(target_x < g.ghost[2].pos.x) //horizontal
            g.ghost[2].dir = left;
    }
    else
    {
        //vertical
        if(target_y < g.ghost[2].pos.y)
            g.ghost[2].dir = up;
    }
    return g;
}


//CLYDE GHOST
t_game clydemove(t_game g, int elapsed_seconds)
{
    if(elapsed_seconds < g.ghost[3].start_time)
        return g;
    int prev_y, prev_x;
    int movev[2] = {up, down};
    int moveh[2] = {left, right};

    prev_y = g.ghost[3].pos.y;
    prev_x = g.ghost[3].pos.x;

    /* calcula a proxima posicao com base na direcao atual */
    int next_y = g.ghost[3].pos.y;
    int next_x = g.ghost[3].pos.x;

    if(g.lab[next_y - 1][next_x] == '-')
        g.ghost[3].dir = up;

    if(g.ghost[3].dir == up)
    {
        if(g.lab[next_y - 1][next_x] != '#')
            next_y--;
        else
        {
            g.ghost[3].dir = moveh[rand() % 2];
            if((g.ghost[3].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[3].dir == right && g.lab[next_y][next_x + 1] == '#'))
            {
                next_x--;
                g.ghost[3].dir = left;
            }
            else
            {
                next_x++;
                g.ghost[3].dir = right;
            }
        }
    }

    else
        if(g.ghost[3].dir == down)
        {
            if(g.lab[next_y + 1][next_x] != '#')
                next_y++;
            else
            {
                g.ghost[3].dir = moveh[rand() % 2];
                if((g.ghost[3].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[3].dir == right && g.lab[next_y][next_x + 1] == '#'))
                {
                    next_x--;
                    g.ghost[3].dir = left;
                }
                else
                {
                    next_x++;
                    g.ghost[3].dir = right;
                }
            }
        }

        else
            if(g.ghost[3].dir == left)
            {
                if(g.lab[next_y][next_x - 1] != '#')
                    next_x--;
                else
                {
                    g.ghost[3].dir = movev[rand() % 2];
                    if((g.ghost[3].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[3].dir == down && g.lab[next_y + 1][next_x] == '#'))
                    {
                        next_y--;
                        g.ghost[3].dir = up;
                    }
                    else
                    {
                        next_y++;
                        g.ghost[3].dir = down;
                    }
                }
            }

            else
                if(g.ghost[3].dir == right)
                {
                    if(g.lab[next_y][next_x + 1] != '#')
                        next_x++;
                    else
                    {
                        g.ghost[3].dir = movev[rand() % 2];
                        if((g.ghost[3].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[3].dir == down && g.lab[next_y + 1][next_x] == '#'))
                        {
                            next_y--;
                            g.ghost[3].dir = up;
                        }
                        else
                        {
                            next_y++;
                            g.ghost[3].dir = down;
                        }
                    }
                }

    g.ghost[3].pos.y = next_y;
    g.ghost[3].pos.x = next_x;

    /* teleporta o fantasma se ele alcançar a borda da tela */
    if(g.ghost[3].pos.x == 0 && g.ghost[3].pos.y == 10)
        g.ghost[3].pos.x = 17;
    if(g.ghost[3].pos.x == 18 && g.ghost[3].pos.y == 10)
        g.ghost[3].pos.x = 1;

    /* atualiza a posição do pacman na tela */
    if(prev_y != g.ghost[3].pos.y || prev_x != g.ghost[3].pos.x)
    {
        start_color();
        mvaddch(prev_y, prev_x, g.lab[prev_y][prev_x]);
        attron(COLOR_PAIR(4));
        mvaddch(g.ghost[3].pos.y, g.ghost[3].pos.x, 'C');
        attroff(COLOR_PAIR(4));
        mvchgat(g.ghost[3].pos.y, g.ghost[3].pos.x, 1, A_BOLD, 4, NULL);
    }
    refresh();
    return g;
}

t_game chaseclyde(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int yghost = g.ghost[3].pos.y, xghost = g.ghost[3].pos.x;

    if(ypac <= yghost - 8 && ypac >= 0 && g.lab[yghost - 1][xghost] != '#' && xpac == xghost)
        g.ghost[3].dir = up;

    else
        if(ypac >= yghost + 8 && ypac <= LABL && g.lab[yghost + 1][xghost] != '#' && xpac == xghost)
            g.ghost[3].dir = down;

        else
            if(xpac <= xghost - 8 && xpac >= 0 && g.lab[yghost][xghost - 1] != '#' && ypac == yghost)
                g.ghost[3].dir = left;

            else
                if(xpac <= LABC && xpac >= xghost + 8 && g.lab[yghost][xghost + 1] != '#' && ypac == yghost)
                    g.ghost[3].dir = right;

    return g;
}

t_game imprime_placar(t_game g, int elapsed_seconds)
{
    attron(COLOR_PAIR(11));
    mvprintw(1, 22, "|Score: %d|", g.pacman.score);
    mvprintw(3, 22, "|Tempo: %d segundos|", elapsed_seconds);
    attroff(COLOR_PAIR(11));
    refresh();
    return g;
}

t_game imprime_nivel(t_game g)
{
    attron(COLOR_PAIR(11));
    mvprintw(4, 22, "|NIVEL: %d|", g.pacman.nivel);
    attroff(COLOR_PAIR(11));
    refresh();
    return g;
}

//menu de pausa
t_game menupause(t_game g)
{
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(false);


    time_t pause_start_time = time(NULL);
    int choice = 14; // escolha
    int highlight = 15; // decisão
    int c;

    while(1)
    {
        for(int i = 15; i <= 17; i++)
        {
            if(i == highlight)
                attron(A_REVERSE);
            mvprintw(i, 27, "%s", (i == 15) ? "CONTINUAR" : (i == 16) ? "REINICIAR" : "SAIR"); // opções de escolha que aparece na tela para o usúario
            attroff(A_REVERSE);
            mvprintw(14, 26, "%s", "Jogo Pausado:\n");
            mvprintw(13, 25, "%s", "-----------------");
            mvprintw(18, 25, "%s", "-----------------");
            mvprintw(13, 25, "%s", "|");
            mvprintw(14, 25, "%s", "|");
            mvprintw(15, 25, "%s", "|");
            mvprintw(16, 25, "%s", "|");
            mvprintw(17, 25, "%s", "|");
            mvprintw(18, 25, "%s", "|");
            mvprintw(13, 40, "%s", "|");
            mvprintw(14, 40, "%s", "|");
            mvprintw(15, 40, "%s", "|");
            mvprintw(16, 40, "%s", "|");
            mvprintw(17, 40, "%s", "|");
            mvprintw(18, 40, "%s", "|");
        }
        c = getch();
        switch(c)
        {
            case KEY_UP:
                if(highlight > 15)
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight < 17)
                    highlight++;
                break;
            case 10: // quando se preciona a tecla ENTER
                choice = highlight;
                break;
        }
        if(choice != 14)
            break;
    }
    if(choice == 15)
    {
        g.timer.start_time += time(NULL) - pause_start_time; /* Atualiza o tempo de início para incluir o tempo de pausa */
        clear();
        refresh();
    }
    else
        if(choice == 16)
        {
            g = upecman_init();
            g.timer.start_time = time(NULL); /* Reinicie o tempo de início do jogo */
            clear();
            refresh();
        }
        else
            if(choice == 17)
            {
                g = upecman_init();
                g.timer.start_time = time(NULL);
                clear();
                menu(g);
            }

    getch();
    endwin();
    return g;
}

//--------------------------Proximo-Nivel----------------------------------------------------------------------------------//
t_game proximo_nivel(t_game g)
{
    int contador_pilulas = 185;
    if(contador_pilulas == 0)
    {
        g = upecman_init();
        g.pacman.life++;
        clear();
        refresh();

        contador_pilulas = 185;
    }
    return g;
}


//--------------------------TELA-DE-GAMEOVER----------------------------------------------------------------------------------------//
t_game game_over(t_game g)
{
    int ch; /* leitura do teclado */
    clear();
    wattron(stdscr, COLOR_PAIR(1));
    mvprintw(1, 1, "   ____      _      __  __  U _____ u       U  ___ u__     __ U _____ u   ____");
    mvprintw(2, 1, "U /\"___|uU  /\"\\  uU|' \\/ '|u\\| ___\"|/        \\/\"_ \\/\\ \\   /\"/u\\| ___\"|/U |  _\"\\ u");
    mvprintw(3, 1, "\\| |  _ / \\/ _ \\/ \\| |\\/| |/ |  _|\"          | | | | \\ \\ / //  |  _|\"   \\| |_) |/");
    mvprintw(4, 1, " | |_| |  / ___ \\  | |  | |  | |___      .-,_| |_| | /\\ V /_,-.| |___    |  _ <");
    mvprintw(5, 1, "  \\____| /_/   \\_\\ |_|  |_|  |_____|      \\_)-\\___/ U  \\_/-(_/ |_____|   |_| \\_\\");
    mvprintw(6, 1, "  _)(|_   \\\\    >><<,-,,-.   <<   >>           \\\\     //       <<   >>   //   \\_");
    mvprintw(7, 1, " (__)__) (__)  (__)(./  \\.) (__) (__)         (__)   (__)     (__) (__) (__)  (__)");
    wattroff(stdscr, COLOR_PAIR(1));
    mvprintw(10, 10, "OBRIGADO POR JOGAR!!!!");
    mvprintw(11, 10, "Sua pontuação foi de: %d", g.pacman.score);
    mvprintw(12, 10, "Aperte Q para Sair do Jogo");
    mvprintw(13, 10, "Para jogar novamente Segure R");
    while(true)
    {
        flushinp();
        usleep(150000);
        ch = getch();
        if(ch == 'Q' || ch == 'q')
            break;
        else
            if(ch == 'R' || ch == 'r')
            {
                //Reinicia o Jogo
                clear();
                g = upecman_init();
                printlife(g);
                printlab(g);
                refresh();
                return g;
            }
    }
    clear();
    endwin();
    exit(0);

    return g;
}


//area que fica acima do respawn
bool not_up_re1(int y, int x)
{
    int X_MIN = 8;
    int X_MAX = 11;
    int Y_MIN = 6;
    int Y_MAX = 6;

    return (X_MIN <= x && x <= X_MAX) && (Y_MIN <= y && y <= Y_MAX);

}

//area que fica abaixo do respawn
bool not_up_re2(int y, int x)
{
    int X_MIN = 8;
    int X_MAX = 11;
    int Y_MIN = 16;
    int Y_MAX = 16;

    return (X_MIN <= x && x <= X_MAX) && (Y_MIN <= y && y <= Y_MAX);
}

//area de respawn
bool zona_respawn(int y, int x)
{
    int X_MIN = 7;
    int X_MAX = 12;
    int Y_MIN = 8;
    int Y_MAX = 12;

    return (X_MIN <= x && x <= X_MAX) && (Y_MIN <= y && y <= Y_MAX);
}

//corredor esquerdo
bool corredor_esquerdo(int y, int x)
{
    int X_MIN = 0;
    int X_MAX = 4;
    int Y_MIN = 10;
    int Y_MAX = 10;

    return (X_MIN <= x && x <= X_MAX) && (Y_MIN <= y && y <= Y_MAX);
}

//corredor direito
bool corredor_direita(int y, int x)
{
    int X_MIN = 16;
    int X_MAX = 19;
    int Y_MIN = 10;
    int Y_MAX = 10;

    return (X_MIN <= x && x <= X_MAX) && (Y_MIN <= y && y <= Y_MAX);

}

t_game blinkymove(t_game g, int elapsed_seconds)
{
    if(elapsed_seconds < g.ghost[0].start_time)
        return g;

    int prev_y, prev_x;
    int movev[2] = {up, down};
    int moveh[2] = {left, right};

    prev_y = g.ghost[0].pos.y;
    prev_x = g.ghost[0].pos.x;

    /* calcula a proxima posicao com base na direcao atual */
    int next_y = g.ghost[0].pos.y;
    int next_x = g.ghost[0].pos.x;

    if(g.lab[next_y - 1][next_x] == '-')
        g.ghost[0].dir = up;

    if(g.ghost[0].dir == up)
    {
        if(g.lab[next_y - 1][next_x] != '#')
            next_y--;
        else
        {
            g.ghost[0].dir = moveh[rand() % 2];
            if((g.ghost[0].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[0].dir == right && g.lab[next_y][next_x + 1] == '#'))
            {
                next_x--;
                g.ghost[0].dir = left;
            }
            else
            {
                next_x++;
                g.ghost[0].dir = right;
            }
        }
    }

    else
        if(g.ghost[0].dir == down)
        {
            if(g.lab[next_y + 1][next_x] != '#')
                next_y++;
            else
            {
                g.ghost[0].dir = moveh[rand() % 2];
                if((g.ghost[0].dir == left && g.lab[next_y][next_x - 1] != '#') || (g.ghost[0].dir == right && g.lab[next_y][next_x + 1] == '#'))
                {
                    next_x--;
                    g.ghost[0].dir = left;
                }
                else
                {
                    next_x++;
                    g.ghost[0].dir = right;
                }
            }
        }

        else
            if(g.ghost[0].dir == left)
            {
                if(g.lab[next_y][next_x - 1] != '#')
                    next_x--;
                else
                {
                    g.ghost[0].dir = movev[rand() % 2];
                    if((g.ghost[0].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[0].dir == down && g.lab[next_y + 1][next_x] == '#'))
                    {
                        next_y--;
                        g.ghost[0].dir = up;
                    }
                    else
                    {
                        next_y++;
                        g.ghost[0].dir = down;
                    }
                }
            }

            else
                if(g.ghost[0].dir == right)
                {
                    if(g.lab[next_y][next_x + 1] != '#')
                        next_x++;
                    else
                    {
                        g.ghost[0].dir = movev[rand() % 2];
                        if((g.ghost[0].dir == up && g.lab[next_y - 1][next_x] != '#') || (g.ghost[0].dir == down && g.lab[next_y + 1][next_x] == '#'))
                        {
                            next_y--;
                            g.ghost[0].dir = up;
                        }
                        else
                        {
                            next_y++;
                            g.ghost[0].dir = down;
                        }
                    }
                }
    if(g.lab[next_y][next_x] != '#')
    {
        g.ghost[0].pos.y = next_y;
        g.ghost[0].pos.x = next_x;
    }

    /* teleporta o fantasma se ele alcançar a borda da tela */
    if(g.ghost[0].pos.x == 0 && g.ghost[0].pos.y == 10)
        g.ghost[0].pos.x = 17;
    if(g.ghost[0].pos.x == 18 && g.ghost[0].pos.y == 10)
        g.ghost[0].pos.x = 1;

    /* atualiza a posição do pacman na tela */
    if(prev_y != g.ghost[0].pos.y || prev_x != g.ghost[0].pos.x)
    {
        start_color();
        mvaddch(prev_y, prev_x, g.lab[prev_y][prev_x]);
        attron(COLOR_PAIR(1));
        mvaddch(g.ghost[0].pos.y, g.ghost[0].pos.x, 'B');
        attroff(COLOR_PAIR(1));
        mvchgat(g.ghost[0].pos.y, g.ghost[0].pos.x, 1, A_BOLD, 1, NULL);
    }
    refresh();
    return g;
}

t_game chaseblinky(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int yblink = g.ghost[0].pos.y, xblink = g.ghost[0].pos.x;

    if(ypac <= yblink && ypac >= yblink - 7 && g.lab[yblink - 1][xblink] != '#' && xpac == xblink)
        g.ghost[0].dir = up;

    else
        if(ypac >= yblink && ypac <= yblink + 7 && g.lab[yblink + 1][xblink] != '#' && xpac == xblink)
            g.ghost[0].dir = down;

        else
            if(xpac >= xblink - 7 && xpac <= xblink && g.lab[yblink][xblink - 1] != '#' && ypac == yblink)
                g.ghost[0].dir = left;

            else
                if(xpac >= xblink && xpac <= xblink + 7 && g.lab[yblink][xblink + 1] != '#' && ypac == yblink)
                    g.ghost[0].dir = right;

    return g;
}

t_game scatterblinky(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int yblink = g.ghost[0].pos.y, xblink = g.ghost[0].pos.x;

    if(ypac <= yblink && ypac >= yblink - 5 && g.lab[yblink + 1][xblink] != '#' && xpac == xblink && g.ghost[0].dir != up)
        g.ghost[0].dir = down;

    else
        if(ypac >= yblink && ypac <= yblink + 5 && g.lab[yblink - 1][xblink] != '#' && xpac == xblink && g.ghost[0].dir != down)
            g.ghost[0].dir = up;
        else
            if(xpac >= xblink - 5 && xpac <= xblink && g.lab[yblink][xblink + 1] != '#' && ypac == yblink && g.ghost[0].dir != left)
                g.ghost[0].dir = right;

            else
                if(xpac >= xblink && xpac <= xblink + 5 && g.lab[yblink][xblink - 1] != '#' && ypac == yblink && g.ghost[0].dir != right)
                    g.ghost[0].dir = left;

    return g;
}

t_game scatterpinky(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int ypinky = g.ghost[1].pos.y, xpinky = g.ghost[1].pos.x;

    if(ypac <= ypinky && ypac >= ypinky - 5 && g.lab[ypinky + 1][xpinky] != '#' && xpac == xpinky && g.ghost[1].dir != up)
        g.ghost[1].dir = down;

    else
        if(ypac >= ypinky && ypac <= ypinky + 5 && g.lab[ypinky - 1][xpinky] != '#' && xpac == xpinky && g.ghost[1].dir != down)
            g.ghost[1].dir = up;

        else
            if(xpac >= xpinky - 5 && xpac <= xpinky && g.lab[ypinky][xpinky + 1] != '#' && ypac == ypinky && g.ghost[1].dir != left)
                g.ghost[1].dir = right;
            else
                if(xpac >= xpinky && xpac <= xpinky + 5 && g.lab[ypinky][xpinky - 1] != '#' && ypac == ypinky && g.ghost[1].dir != right)
                    g.ghost[1].dir = left;
    return g;
}

t_game scatterinky(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int yinky = g.ghost[2].pos.y, xinky = g.ghost[2].pos.x;

    if(ypac <= yinky && ypac >= yinky - 5 && g.lab[yinky + 1][xinky] != '#' && xpac == xinky && g.ghost[2].dir != up)
        g.ghost[2].dir = down;

    else
        if(ypac >= yinky && ypac <= yinky + 5 && g.lab[yinky - 1][xinky] != '#' && xpac == xinky && g.ghost[2].dir != down)
            g.ghost[2].dir = up;

        else
            if(xpac >= xinky - 5 && xpac <= xinky && g.lab[yinky][xinky + 1] != '#' && ypac == yinky && g.ghost[2].dir != left)
                g.ghost[2].dir = right;
            else
                if(xpac >= xinky && xpac <= xinky + 5 && g.lab[yinky][xinky - 1] != '#' && ypac == yinky && g.ghost[2].dir != right)
                    g.ghost[2].dir = left;

    return g;
}

t_game scatterclyde(t_game g)
{
    int ypac = g.pacman.pos.y, xpac = g.pacman.pos.x;
    int yclyde = g.ghost[3].pos.y, xclyde = g.ghost[3].pos.x;

    if(ypac <= yclyde && ypac >= yclyde - 5 && g.lab[yclyde + 1][xclyde] != '#' && xpac == xclyde && g.ghost[3].dir != up)
        g.ghost[3].dir = down;

    else
        if(ypac >= yclyde && ypac <= yclyde + 5 && g.lab[yclyde - 1][xclyde] != '#' && xpac == xclyde && g.ghost[3].dir != down)
            g.ghost[3].dir = up;

        else
            if(xpac >= xclyde - 5 && xpac <= xclyde && g.lab[yclyde][xclyde + 1] != '#' && ypac == yclyde && g.ghost[3].dir != left)
                g.ghost[3].dir = right;

            else
                if(xpac >= xclyde && xpac <= xclyde + 5 && g.lab[yclyde][xclyde - 1] != '#' && ypac == yclyde && g.ghost[3].dir != right)
                    g.ghost[3].dir = left;

    return g;
}

//codigo para colisão e respawn do pacman (falta ajustes)
t_game respawn_morte(t_game g, int elapsed_seconds)
{
    {
        int f;

        for(int i = 0; i < 4; i++)
        {
            if(g.pacman.pos.y == g.ghost[i].pos.y && g.pacman.pos.x == g.ghost[i].pos.x && g.ghost[i].mode != afraid)
            {
                g.pacman.life--;
                printlife(g);
                g.pacman.pos.x = 9;
                g.pacman.pos.y = 17;
                g.pacman.dir = left;
                init_pair(6, COLOR_YELLOW, COLOR_BLACK);
                attron(COLOR_PAIR(6));
                mvaddch(g.pacman.pos.y, g.pacman.pos.x, '@');
                attroff(COLOR_PAIR(6));
                g.pacman.dir = KEY_LEFT;
                for(f = blinky; f <= clyde; f++)
                {
                    int prev_y = g.ghost[f].pos.y;
                    int prev_x = g.ghost[f].pos.x;

                    switch(f)
                    {
                        case blinky:
                            g.ghost[f].pos.y = 7;
                            g.ghost[f].pos.x = 9;
                            g.ghost[f].starget.y = 0;
                            g.ghost[f].starget.x = 19;
                            if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                                mvaddch(prev_y, prev_x, ' ');
                            break;
                        case pinky:
                            g.ghost[f].pos.y = 9;
                            g.ghost[f].pos.x = 10;
                            g.ghost[f].starget.y = 0;
                            g.ghost[f].starget.x = 0;
                            if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                                mvaddch(prev_y, prev_x, ' ');
                            break;
                        case inky:
                            g.ghost[f].pos.y = 10;
                            g.ghost[f].pos.x = 10;
                            g.ghost[f].starget.y = 22;
                            g.ghost[f].starget.x = 19;
                            if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                                mvaddch(prev_y, prev_x, ' ');
                            break;
                        case clyde:
                            g.ghost[f].pos.y = 11;
                            g.ghost[f].pos.x = 10;
                            g.ghost[f].starget.y = 22;
                            g.ghost[f].starget.x = 0;
                            if(prev_y != g.ghost[f].pos.y || prev_x != g.ghost[f].pos.x)
                                mvaddch(prev_y, prev_x, ' ');
                    }
                    g.ghost[f].dir = left;
                    g.ghost[f].mode = chase;
                    if(elapsed_seconds >= g.ghost[i].start_time)
                        g.ghost[i].start_time = elapsed_seconds;
                    else
                        g.ghost[i].start_time = elapsed_seconds + i * 10;
                }
            }
        }
    }

    return g;
}

t_game printlife(t_game g)
{

    curs_set(false);
    attron(COLOR_PAIR(11));
    mvprintw(2, 22, "|Vidas: %d|", g.pacman.life);
    attroff(COLOR_PAIR(11));
    refresh();

    return g;

}


/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This function initializes some operations before start
 * @details Details to be written
 * @return Void
 * @todo Need to implement it. Its empty now.
 * @author Ruben Carlo Benante
 * @version 20160530.224016
 * @date 2016-05-30
 *
 */
void printlab(t_game g)
{
    IFDEBUG("printlab()");

    int y, a, f;
    clear();

    for(y = 0; y < LABL; y++)
    {
        for(a = 0; a < LABC; a++)
        {
            mvprintw(y, a, "%c", g.lab[y][a]);
            if(g.lab[y][a] == '#')
                mvchgat(y, a, -1, A_BOLD, 5, NULL);

            if(g.lab[y][a] == '%')
                mvchgat(y, a, -1, A_BOLD, 1, NULL);

            if(g.lab[y][a] == '.')
                mvchgat(y, a, -1, A_BOLD, 11, NULL);

            if(g.lab[y][a] == 'o')
                mvchgat(y, a, -1, A_BOLD, 11, NULL);
        }
    }

    for(f = blinky; f <= clyde; f++)
    {
        // blinky, pinky, inky and clyde
        mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, f + 1, NULL);
        mvchgat(g.pacman.pos.y, g.pacman.pos.x, 1, A_BOLD, 6, NULL);
    }

    printlife(g);
    imprime_nivel(g);
    refresh();
}


/* ---------------------------------------------------------------------- */
/* calcula diferenca entre dois tempos w em microsegundos */
long tdus(struct timeval w0, struct timeval w1)
{
    long ws, wu;
    ws = (w1.tv_sec - w0.tv_sec);
    wu = ((ws * 1000000) + w1.tv_usec) - w0.tv_usec;
    return wu;
}

/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20160612.142044      */

