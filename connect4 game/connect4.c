#include <stdio.h>

#define PLAYER1 1
#define PLAYER2 2
#define ROWS 9
#define COLS 9
#define EMPTY 0
#define BOARD_FILE "tahta.txt"
#define MOVE_FILE "hamle.txt"
#define MAX_MOVE 80

#define MODE_NEW_GAME 1
#define MODE_CONTINUE 2

int get_game_mode()
{
    printf("Welcome to Connect Four Game!\n");
    printf("Please select game mode:\n");
    printf("1. New game\n");
    printf("2. Continue\n");
    int game_mode;
    scanf("%d", &game_mode);
    return game_mode;
}

void clear_state()
{
    FILE *fp1 = fopen(BOARD_FILE, "w+");
    FILE *fp2 = fopen(MOVE_FILE, "w+");
    fclose(fp1);
    fclose(fp2);
}

void save_board_state(int board[ROWS][COLS])
{
    FILE *fp = fopen(BOARD_FILE, "w+");
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS - 1; j++)
        {
            fprintf(fp, "%d,", board[i][j]);
        }
        fprintf(fp, "%d\n", board[i][j]);
    }
    fclose(fp);
}

void load_state(int board[ROWS][COLS], int *playerturn)
{
    FILE *fp = fopen(BOARD_FILE, "r");
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS - 1; j++)
        {
            fscanf(fp, "%d,", &board[i][j]);
        }
        fscanf(fp, "%d\n", &board[i][j]);
    }
    fclose(fp);

    FILE *fp2 = fopen(MOVE_FILE, "r");
    int player, row, col;
    while (fscanf(fp2, "player(%d), %d, %d\n", &player, &row, &col) != EOF)
    {
        if (player == PLAYER1)
            *playerturn = PLAYER2; // change the player
        else
            *playerturn = PLAYER1; // change the player
    }

    fclose(fp2);
}

void displaygame(int board[ROWS][COLS])
{ // displaying the board

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("|");
            if (board[i][j] == EMPTY)
            {
                printf(" ");
            }
            else if (board[i][j] == PLAYER1)
            {
                printf("\033[0;31m"); // for red color
                printf("X");
                printf("\033[0m");
            }
            else
            {
                printf("\033[0;34m"); // for blue color
                printf("O");
                printf("\033[0m");
            }
        }
        printf("|\n");
    }
}

int isvalidmove(int board[ROWS][COLS], int selectedcol,int *gamecount)
{
    if(*gamecount > MAX_MOVE){
        printf("Board is full! Game is tied.");
        return 0;
    }

    if (selectedcol >= COLS || selectedcol < 0)
    { // if valid selection.
        return 0;
    }
    if (board[0][selectedcol] != EMPTY)
    { // top of the column.
        return 0;
    }
    return 1;
}

void makemove(int board[ROWS][COLS], int selectedcol, int playerturn)
{
    FILE *fp = fopen(MOVE_FILE, "a");


    for (int i = ROWS - 1; i >= 0; i--)
    { // check the board from bottom to the top for empty places
        if (board[i][selectedcol] == EMPTY)
        {
            board[i][selectedcol] = playerturn;
            fprintf(fp, "player(%d), %d, %d\n", playerturn, i, selectedcol);
            break;
        }
    }

    fclose(fp);
}

int checkwinner(int board[ROWS][COLS], int whichplayer)
{

    int i, j, k;
    // Horizontal
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS - 3; j++)
        {
            if (board[i][j] == whichplayer && board[i][j + 1] == whichplayer && board[i][j + 2] == whichplayer && board[i][j + 3] == whichplayer)
            {
                return 1;
            }
        }
    }
    // Vertical
    for (i = 0; i < ROWS - 3; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (board[i][j] == whichplayer && board[i + 1][j] == whichplayer && board[i + 2][j] == whichplayer && board[i + 3][j] == whichplayer)
            {
                return 1;
            }
        }
    }
    // Diagonal, from left top to right bottom
    for (i = 0; i < ROWS - 3; i++)
    {
        for (j = 0; j < COLS - 3; j++)
        {
            if (board[i][j] == whichplayer && board[i + 1][j + 1] == whichplayer && board[i + 2][j + 2] == whichplayer && board[i + 3][j + 3] == whichplayer)
            {
                return 1;
            }
        }
    }
    // Diagonal, from left bottom to right top
    for (i = 3; i < ROWS; i++)
    {
        for (j = 0; j < COLS - 3; j++)
        {
            if (board[i][j] == whichplayer && board[i - 1][j + 1] == whichplayer && board[i - 2][j + 2] == whichplayer && board[i - 3][j + 3] == whichplayer)
            {
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    int board[ROWS][COLS] = {{0}}; // fill the board 0s for initial.
    int gameover = 0;
    int playerturn = PLAYER1;
    int selectedcol; // player selection
    int gamecount = 0;

    int game_mode = get_game_mode();
    if (game_mode == MODE_NEW_GAME)
        clear_state();
    else if (game_mode == MODE_CONTINUE)
        load_state(board, &playerturn);

    do
    {
        save_board_state(board);
        displaygame(board);
        printf("\n%d. Player turn. Please make your move. Select column from 0 to 8: ", playerturn);
        scanf("%d", &selectedcol);
        if (isvalidmove(board, selectedcol,&gamecount))
        { // if its valid move than make move

            makemove(board, selectedcol, playerturn);

            if (checkwinner(board, playerturn))
            {                       // after every move whether check game is finish or not. if there is a winner enter if.
                displaygame(board); // display board
                printf("\n***Congrulations! Player %d wins!***", playerturn);
                gameover = 1;
            }
            else if (playerturn == PLAYER1)
            {                         // if game doesnt finish and current player is player1
                playerturn = PLAYER2; // change the player
            }
            else                      // if game doesnt finish and current player is player2
                playerturn = PLAYER1; // change the player
        }

        else
        { // invalid move
            printf("Invalid move! Player %d's turn again.\n", playerturn);
        }

    } while (!gameover);

    return 0;
}