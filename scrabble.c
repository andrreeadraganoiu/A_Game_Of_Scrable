#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "util/print_board.h"
#include "util/scrabble.h"

#define WORD_LENGTH 50

typedef struct {
    char w[WORD_LENGTH];    //w retine un cuvant (sir de caractere)
    int x[WORD_LENGTH];     //x[i] retine linia din tabla de joc unde este pozitionat caracterul w[i]
    int y[WORD_LENGTH];     //y[i] retine coloana din tabla unde este pozitionat caracterul w[i]
    int d;                  //d = directia cuvantului: 0 pt orizontala, 1 pt verticala
    int val;                //valoarea cuvantului: suma scorurilor fiecarei litere
    int bonus1;             //are valoarea 1 pt cuvintele care se califica la bonus1 si 0 in caz contrar
    int bonus2;             //are valoarea 1 pt cuvintele care se califica la bonus2 si 0 in caz contrar
} word;

typedef char game_board[BOARD_SIZE][BOARD_SIZE];        //tabla de joc

/*
    Initializez cu valori nule o variabila de tip word
*/
void init_word(word *cuv)
{
    strcpy(cuv->w, "");
    memset(cuv->x, -1, WORD_LENGTH * sizeof(int));       //completez tot vectorul x cu -1
    memset(cuv->y, -1, WORD_LENGTH * sizeof(int));       //completez tot vectorul y cu -1
    cuv->d = cuv->val = cuv->bonus1 = cuv->bonus2 = 0;
}

/*
    Initializeaza matricea de caractere ce reprezinta tabla de joc
*/
void init_board(game_board board)
{
    for(int i = 0; i < BOARD_SIZE; i++)
        for(int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = ' ';
}

/*
    Citeste un numar aflat pe o linie la input
*/
void read_number(int *n)
{
    char linie[5];
    fgets(linie, 5, stdin);
    *n = atoi(linie);
}

/*
    Citeste un sir de caractere aflat pe o linie la input
*/
void read_string(char linie[5])
{
    fgets(linie, 5, stdin);
    if (linie[strlen(linie) - 1] == '\n')
        linie[strlen(linie) - 1] = '\0';       //sterg enter-ul de la sfarsitul cuvantului
}

/*
    Returneaza pozitia unui cuvant trimis ca parametru, in vectorul predefinit de cuvinte - words,
    sau -1 daca nu se gaseste in vectorul predefinit
*/
int get_word_position(char cuv[WORD_LENGTH])
{
    for (int i = 0; i < NUM_WORDS; i++)
        if (strcmp(cuv, words[i]) == 0)
            return i;
    return -1;
}

/*
    Determina daca pentru cuvantul trimis ca parametru se poate aplica bonus1 respectiv bonus2
*/
void get_bonus(word *cuv, char subsir1[5], char subsir2[5])
{
    cuv->bonus1 = cuv->bonus2 = 0;
    if (strlen(subsir1) && strstr(cuv->w, subsir1))
       cuv->bonus1 = 1;

    if (strlen(subsir2))
    {
        char *s = strstr(cuv->w, subsir2);
        if (s && s - cuv->w + strlen(subsir2) == strlen(cuv->w))
            cuv->bonus2 = 1;
    }
}

/*
    Citeste o linie cu un cuvant de la tastatura. O linie este de forma Y1 X1 D1 WORD1;
    Marcheaza cuvantul ca folosit. Calculeaza daca se poate aplica bonus1 sau bonus2
*/
void read_one_word(word *cuv, int word_used[NUM_WORDS], char subsir1[5], char subsir2[5])
{
    int param;
    char linie[100], *p;

    param = 0;
    fgets(linie, 100, stdin);
    p = strtok(linie, " ");
    while (p){
        param++;
        switch(param){
            case 1:
                cuv->x[0] = atoi(p);
                break;
            case 2:
                cuv->y[0] = atoi(p);
                break;
            case 3:
                cuv->d = atoi(p);
                break;
            case 4:
                strcpy(cuv->w, p);
                if (cuv->w[strlen(p) - 1] == '\n')
                    cuv->w[strlen(p) - 1] = '\0';       //sterg enter-ul de la sfarsitul cuvantului daca nu e ultima linie

                int poz = get_word_position(cuv->w);
                if (poz > -1)
                    word_used[poz] = 1;                 //marchez cuvantul ca folosit
                get_bonus(cuv, subsir1, subsir2);       //calculez daca se poate aplica bonus1 sau bonus2

                break;
        }
        p = strtok(NULL, " ");
    }
}

/*
    Citeste N linii de cuvinte de la tastatura. O linie este de forma Y1 X1 D1 WORD1
    Returneaza un vector de elemente de tip word si dimensiunea acestuia (numarul de cuvinte citit)
*/
void read_words(word word_list[NUM_WORDS], int *n, int word_used[NUM_WORDS], char subsir1[5], char subsir2[5])
{
    int i;
    word cuv;

    read_number(n);
    for (i = 0; i < *n; i++){
        read_one_word(&cuv, word_used, subsir1, subsir2);
        word_list[i] = cuv;
    }
}

/*
    Plaseaza cuvantul trimis ca parametru pe tabla de joc
    Completeaza vectorii x si y din cadrul unui cuvant,
    ce retin coordonatele unei litere din cuvant in cadrul tablei de joc
*/
void place_one_word(game_board board, word *cuv)
{
    int x, y, n;

    n = strlen(cuv->w);
    x = cuv->x[0]; y = cuv->y[0];
    for (int j = 0; j < n; j++){
        board[x][y] = cuv->w[j];
        cuv->x[j] = x; cuv->y[j] = y;
        if (cuv->d == 0)
            y++;
        else
            x++;
    }
}

/*
    
    Primeste ca parametru o lista de cuvinte si le plaseaza pe tabla de joc
    Completeaza vectorii x si y din cadrul unui cuvant, ce retin coordonatele unei litere din cuvant
    
*/
void place_words(game_board board, word word_list[NUM_WORDS], int word_no)
{
    for (int i = 0; i < word_no; i++){
        place_one_word(board, &word_list[i]);
        
    }
}

/*
    Calculeaza scorul unui cuvant in functie de bonusuri (daca sunt setate)
*/
void word_value(word *cuv)
{
    //                      A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P   Q  R  S  T  U  V  W  X  Y   Z
    int letter_value[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int multiply1 = 1, multiply2 = 1;

    int n = strlen(cuv->w), value = 0;
    for (int i = 0; i < n; i++){
        value += letter_value[cuv->w[i] - 'A'];

        if(cuv->bonus1 == 1 && bonus_board[ cuv->x[i] ][ cuv->y[i] ] == 1)
            multiply1 *= 2;
        else
            if(cuv->bonus2 == 1 && bonus_board[ cuv->x[i] ][ cuv->y[i] ] == 2)
                multiply2 *= 3;
    }
    cuv->val = value * multiply1 * multiply2;
}

/*
    player = 1 pt. primul jucator (pozitii pare), 2 pentru al doilea jucator (pozitii impare)
*/
int get_player_score(word word_list[NUM_WORDS], int n, int player)
{
    int seed = 0, score = 0;
    if (player == 2)
        seed = 1;
    for (int i = seed; i < n; i += 2){
        word_value(&word_list[i]);
        score += word_list[i].val;
    }
    return score;
}

/*
    Returneaza 1 daca sunt libere urmatoarele "len" pozitii din tabla de joc incepand de la coordonatele x, y
    pe directia "dir"
*/
int check_free_positions(game_board board, int x, int y, int len, int dir)
{
    int i;
    if (dir == 0){
        for (i = y; i < y + len; i++)
            if (board[x][i] != ' ')
                return 0;
    } else {
        for (i = x; i < x + len; i++)
            if (board[i][y] != ' ')
                return 0;
    }
    return 1;
}

/*
    Sterg cuvantul de pe tabla de joc
*/
void clear_word(game_board board, word cuv)
{
    int n, x, y;

    n = strlen(cuv.w); x = cuv.x[0]; y = cuv.y[0];
    if(cuv.d == 0)
        for (int i = y + 1; i < y + n; i++)
            board[x][i] = ' ';
    else
        for (int i = x + 1; i < x + n; i++)
            board[i][y] = ' ';
}

/*
    Returneaza 1 daca un cuvant poate fi pozitionat pe orizontala incepand cu linia i si coloana j
*/
int check_h_position(game_board board, char c[WORD_LENGTH], int i, int j)
{
    if (j + strlen(c) - 1 < BOARD_SIZE && check_free_positions(board, i, j + 1, strlen(c) - 1, 0))
        return 1;
    return 0;
}

/*
    Returneaza 1 daca un cuvant poate fi pozitionat pe verticala incepand cu linia i si coloana j
*/
int check_v_position(game_board board, char c[WORD_LENGTH], int i, int j)
{
    if (i + strlen(c) - 1 < BOARD_SIZE && check_free_positions(board, i + 1, j, strlen(c) - 1, 1))
        return 1;
    return 0;
}

/*
    Returneaza pozitia primei litere din cuvant si directia acestui cuvant,
    de la coltul stanga sus, linie cu linie, pana la coltul dreapta jos
    al tablei de joc. Daca un cuvant poate fi pus in mai multe locuri, se alege pozitia cu linia cea mai mica,
    daca liniile sunt egale, se alege cea care are cea mai mica coloana
*/
void check_word(game_board board, word *cuv)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == cuv->w[0]){        //am gasit prima litera a cuvantului pe tabla
                                                  //incape in tabla pe orizontala si restul  pozitiilor sunt libere
                if ( check_h_position(board, cuv->w, i, j) ){
                    cuv->x[0] = i; cuv->y[0] = j; cuv->d = 0;
                    return;
                }
                else                              //incape in tabla pe verticala si restul  pozitiilor sunt libere
                    if (check_v_position(board, cuv->w, i, j)){
                        cuv->x[0] = i; cuv->y[0] = j; cuv->d = 1;
                        return;
                    }
            }
}

/*
    Caut primul cuvant ce se poate aseza pe tabla de joc.
    Daca se trimit ca parametri cele 2 subsiruri pentru bonus,
    se va cauta un cuvant cu punctajul cel mai mare (prima aparitie a acestuia)
*/
void find_next_word(game_board board, word *cuv, int word_used[NUM_WORDS])
{
    init_word(cuv);
    for (int i = 0 ; i < NUM_WORDS; i++){
        if (word_used[i] == 0){                 //cuvantul de pe pozitia i din vectorul predefinit words nu a fost folosit inca
            strcpy(cuv->w, words[i]);
            check_word(board, cuv);
            if (cuv->x[0] > -1 && cuv->y[0] > -1){
                word_used[i] = 1;
                return;
            }
        }
    }
}

void get_max_word(game_board board, word *cuvMax, word cuv, char subsir1[5], char subsir2[5])
{
    cuv.bonus1 = cuv.bonus2 = 0;
    get_bonus(&cuv, subsir1, subsir2);
    place_one_word(board, &cuv);
    word_value(&cuv);
    if (cuv.val > cuvMax->val){
        *cuvMax = cuv;
    }
    clear_word(board, cuv);
}

/*
    Returneaza pozitia optima unde poate fi asezat un cuvant in tabla de joc
*/
void check_optimal_word(game_board board, word *cuvMax, char subsir1[5], char subsir2[5])
{
    word cuv;

    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == cuvMax->w[0]){                           //am gasit prima litera a cuvantului pe tabla
                strcpy(cuv.w, cuvMax->w); cuv.x[0] = i; cuv.y[0] = j;
                if ( check_h_position(board, cuvMax->w, i, j) ){        //incape in tabla pe orizontala si restul  pozitiilor sunt libere
                    cuv.d = 0;
                    get_max_word(board, cuvMax, cuv, subsir1, subsir2);
                }
                if ( check_v_position(board, cuvMax->w, i, j) ){        //incape in tabla pe verticala si restul  pozitiilor sunt libere
                    cuv.d = 1;
                    get_max_word(board, cuvMax, cuv, subsir1, subsir2);
                }
            }
}

/*
    Caut primul cuvant optim ce se poate aseza pe tabla de joc.
    Daca se trimit ca parametri cele 2 subsiruri pentru bonus,
    se va cauta un cuvant cu punctajul cel mai mare (prima aparitie a acestuia)
*/
void find_next_optimal_word(game_board board, word *cuvMax, int word_used[NUM_WORDS], char subsir1[5], char subsir2[5])
{
    word cuv;
    int idMax = 0;

    init_word(cuvMax);
    for (int i = 0 ; i < NUM_WORDS; i++){
        if (word_used[i] == 0){                 //cuvantul de pe pozitia i vectorul predefinit words nu a fost folosit inca
            init_word(&cuv);
            strcpy(cuv.w, words[i]);
            check_optimal_word(board, &cuv, subsir1, subsir2);
            if (cuv.val > cuvMax->val || (cuv.val == cuvMax->val && cuv.x[0] < cuvMax->x[0]) ||
                (cuv.val == cuvMax->val && cuv.x[0] == cuvMax->x[0] && cuv.y[0] < cuvMax->y[0]) ||
                (cuv.val == cuvMax->val && cuv.x[0] == cuvMax->x[0] && cuv.y[0] == cuvMax->y[0] && cuv.d == 0 && cuvMax->d == 1)){
                *cuvMax = cuv;
                idMax = i;
            }
        }
    }
    word_used[idMax] = 1;
}

/*
    Functie pentru rezolvarea task-ului 5
*/
void task5(game_board board, word word_list[NUM_WORDS], int word_no, int word_used[NUM_WORDS], char subsir1[5], char subsir2[5])
{
    word cuv;

    int player1 = get_player_score(word_list, word_no, 1);
    int player2 = get_player_score(word_list, word_no, 2);

    find_next_optimal_word(board, &cuv, word_used, subsir1, subsir2);
    player2 += cuv.val;
    if (player1 <= player2){
        place_one_word(board, &cuv);
        print_board(board);
    } else
        printf("%s", "Fail!");
}

/*
    Functie pentru rezolvarea task-ului 6
    Returneaza vectorul word_list de dimensiune m, ce contine lista cuvintelor de pe tabla de joc
    (pozitii pare player 1, pozitii impare player2)
*/
void task6(game_board board, word word_list[NUM_WORDS], int *m, int word_used[NUM_WORDS], char subsir1[5], char subsir2[5])
{
    int i, n, player1 = 0, player2 = 0;
    word cuv;

    *m = 0;
    read_number(&n);
    for (i = 0; i < n; i++){
        init_word(&cuv);

        read_one_word(&cuv, word_used, subsir1, subsir2);
        place_one_word(board, &cuv);
        word_value(&cuv);
        player1 += cuv.val;
        word_list[++(*m)] = cuv;

        init_word(&cuv);
        find_next_optimal_word(board, &cuv, word_used, subsir1, subsir2);
        place_one_word(board, &cuv);
        word_value(&cuv);
        player2 += cuv.val;
        word_list[++(*m)] = cuv;
    }
    print_board(board);
    printf("Player %d Won!", player1 > player2 ? 1 : 2);
}

int main()
{
    game_board board;
    word word_list[NUM_WORDS], cuv;
    int task, word_no;
    int word_used[NUM_WORDS] = {0}; //word_used[i] este 1 daca s-a utilizat cuvantul de pe poz i din vectorul predefinit words
    char subsir1[5] = "", subsir2[5] = "";

    init_board(board);
    read_number(&task);
    if(task >= 3 && task <= 6){
        read_string(subsir1); read_string(subsir2);
    }
    if (task >= 1 && task <= 5){
        read_words(word_list, &word_no, word_used, subsir1, subsir2);
        place_words(board, word_list, word_no);
    }

    switch (task){
        case 0:     
            print_board(board);
            break;          //pentru taskurile 0 si 1 apelez print_board()
        case 1:
            print_board(board);
            break;
        case 2:   
            printf("Player 1: %d Points\nPlayer 2: %d Points", get_player_score(word_list, word_no, 1), get_player_score(word_list, word_no, 2));
            break;              
        case 3:
            printf("Player 1: %d Points\nPlayer 2: %d Points", get_player_score(word_list, word_no, 1), get_player_score(word_list, word_no, 2));
            break;
        case 4:
            find_next_word(board, &cuv, word_used);
            place_one_word(board, &cuv);
            print_board(board);
            break;
        case 5:
            task5(board, word_list, word_no, word_used, subsir1, subsir2);
            break;
        case 6:
            task6(board, word_list, &word_no, word_used, subsir1, subsir2);
            break;
    }

    return 0;
}