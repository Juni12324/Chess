#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define EMPTY 0
#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6

#define WHITE 0
#define BLACK 1

struct Piece{
    int type;
    int colour;
    char symbol[2];
    bool firstTime;
};

struct History{
    int moveNumber;
    char piece[2];
    int oldx;
    int oldy;
    int newx;
    int newy;
    struct History* next;
    struct History* prev;
};

struct Position{
    int x;
    int y;
};

void resetBoard();
void printBoard();
bool checkMove(int oldx, int oldy, int newx, int newy);
struct Position reverseTranslate(char* pos);
void movePiece(int oldx, int oldy, int newx, int newy);
void createHistory(int oldx, int oldy, int newx, int newy);

struct Piece Board[8][8];//[ROW][COLUMN]
bool whiteWin = false;
bool blackWin = false;
struct History* head = NULL;
int moveNum = 1;

int main(){
    resetBoard();
    bool moved = false;
    bool whiteMove, blackMove;
    while (!whiteWin && !blackWin){
        printBoard();
        if (moveNum%2 == 1){
            printf("\n%d: White's turn to move: \n", moveNum);
            whiteMove = true;
            blackMove = false;
        } else{
            printf("\n%d: Black's turn to move: \n", moveNum);
            blackMove = true;
            whiteMove = false;
        }
        //move <currentpos> <newpos> e.g., move C4 D4
        char buffer[20];
        scanf(" %[^\n]", buffer);
        char* command = strtok(buffer, " ");
        if (!strcmp(command, "move")){
            char* oldpos = strtok(NULL, " ");
            char* newpos = strtok(NULL, " ");
            struct Position oldPosition = reverseTranslate(oldpos);
            struct Position newPosition = reverseTranslate(newpos);
            if (whiteMove && Board[oldPosition.y][oldPosition.x].colour == BLACK){
                printf("invalid move\n");
            } else if(blackMove && Board[oldPosition.y][oldPosition.x].colour == WHITE){
                printf("invalid move\n");
            } else if(Board[oldPosition.y][oldPosition.x].type == EMPTY){
                printf("invalid move\n");
            } else if (checkMove(oldPosition.x, oldPosition.y, newPosition.x, newPosition.y)){
                createHistory(oldPosition.x, oldPosition.y, newPosition.x, newPosition.y);
                movePiece(oldPosition.x, oldPosition.y, newPosition.x, newPosition.y);
                moved = true;
            } else{
                printf("invalid move\n");
            }
        }
        if (moved){
            moveNum++;
            moved = false;
        }
    }
    return 0;
}

void createHistory(int oldx, int oldy, int newx, int newy){
    struct History* newHistory = malloc(sizeof(struct History));
    newHistory->moveNumber = moveNum;
    newHistory->oldx = oldx;
    newHistory->oldy = oldy;
    newHistory->newx = newx;
    newHistory->newy = newy;
    strcpy(newHistory->piece, Board[oldy][oldx].symbol);
    if (head == NULL){
        newHistory->next = head;
        newHistory = head;
    } else{
        head->prev = newHistory;
        newHistory->next = head;
        newHistory = head;
    }
}

void deleteHistory(){
    struct History* temp = head;
    struct History* prev = head;
    while (temp != NULL){
        prev = temp;
        temp = temp->next;
        free(prev);
    }
}


void movePiece(int oldx, int oldy, int newx, int newy){
    //Copy piece into the new position
    if (Board[newy][newx].type == KING){//If the new position ahs the king it is dead and game is over
        if (Board[newy][newx].colour == BLACK){
            blackWin = true;
        } else{
            whiteWin = true;
        }
    }
    Board[newy][newx] = Board[oldy][oldx];
    Board[oldy][oldx].type = EMPTY; //Empty the old position
}

struct Position reverseTranslate(char* pos){
    struct Position temp;
    if (pos[0] == 'A'){
        temp.x = 0;
    } else if (pos[0] == 'B'){
        temp.x = 1;
    } else if (pos[0] == 'C'){
        temp.x = 2;
    } else if (pos[0] == 'D'){
        temp.x = 3;
    } else if (pos[0] == 'E'){
        temp.x = 4;
    } else if (pos[0] == 'F'){
        temp.x = 5;
    } else if (pos[0] == 'G'){
        temp.x = 6;
    } else if (pos[0] == 'H'){
        temp.x = 7;
    }
    temp.y = pos[1] - '0';
    temp.y = 8 - temp.y;
    return temp;
}

void resetBoard(){
    moveNum = 1;
    blackWin = false;
    whiteWin = false;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            Board[i][j].type = EMPTY;
        }
    }
    //Set up pawns
    for (int i = 0; i < 8; i++){
        struct Piece newPiece;
        newPiece.type = PAWN;
        newPiece.colour = WHITE;
        strcpy(newPiece.symbol, "P");
        newPiece.firstTime = true;
        Board[6][i] = newPiece;
    }
    for (int i = 0; i < 8; i++){
        struct Piece newPiece;
        newPiece.type = PAWN;
        newPiece.colour = BLACK;
        strcpy(newPiece.symbol, "P");
        newPiece.firstTime = true;
        Board[1][i] = newPiece;
    }
    //Set up the back ranks for Black
    for (int i = 0 ; i < 8 ; i++){
        if (i == 0 || i == 7){
            struct Piece newPiece;
            newPiece.type = ROOK;
            newPiece.colour = BLACK;
            strcpy(newPiece.symbol, "R");
            newPiece.firstTime = true;
            Board[0][i] = newPiece;
        }
        if (i == 1 || i == 6){
            struct Piece newPiece;
            newPiece.type = KNIGHT;
            newPiece.colour = BLACK;
            strcpy(newPiece.symbol, "K");
            newPiece.firstTime = true;
            Board[0][i] = newPiece;
        }
        if (i == 2 || i == 5){
            struct Piece newPiece;
            newPiece.type = BISHOP;
            newPiece.colour = BLACK;
            strcpy(newPiece.symbol, "B");
            newPiece.firstTime = true;
            Board[0][i] = newPiece;
        }
        if (i == 3){
            struct Piece newPiece;
            newPiece.type = QUEEN;
            newPiece.colour = BLACK;
            strcpy(newPiece.symbol, "Q");
            newPiece.firstTime = true;
            Board[0][i] = newPiece;
        }
        if (i == 4){
            struct Piece newPiece;
            newPiece.type = KING;
            newPiece.colour = BLACK;
            strcpy(newPiece.symbol, "X");
            newPiece.firstTime = true;
            Board[0][i] = newPiece;
        }
    }
    //Set up the back ranks for White
    for (int i = 0 ; i < 8 ; i++){
        if (i == 0 || i == 7){
            struct Piece newPiece;
            newPiece.type = ROOK;
            newPiece.colour = WHITE;
            strcpy(newPiece.symbol, "R");
            newPiece.firstTime = true;
            Board[7][i] = newPiece;
        }
        if (i == 1 || i == 6){
            struct Piece newPiece;
            newPiece.type = KNIGHT;
            newPiece.colour = WHITE;
            strcpy(newPiece.symbol, "K");
            newPiece.firstTime = true;
            Board[7][i] = newPiece;
        }
        if (i == 2 || i == 5){
            struct Piece newPiece;
            newPiece.type = BISHOP;
            newPiece.colour = WHITE;
            strcpy(newPiece.symbol, "B");
            newPiece.firstTime = true;
            Board[7][i] = newPiece;
        }
        if (i == 3){
            struct Piece newPiece;
            newPiece.type = QUEEN;
            newPiece.colour = WHITE;
            strcpy(newPiece.symbol, "Q");
            newPiece.firstTime = true;
            Board[7][i] = newPiece;
        }
        if (i == 4){
            struct Piece newPiece;
            newPiece.type = KING;
            newPiece.colour = WHITE;
            strcpy(newPiece.symbol, "X");
            newPiece.firstTime = true;
            Board[7][i] = newPiece;
        }
    }

}

void printBoard(){
    for (int i = 0; i < 8; i++){
        printf("  -------------------------\n");
        printf("%d |", 8 - i);
        for (int j = 0; j < 8; j++){
            if (Board[i][j].type == EMPTY){
                printf("  ");
            } else{
                if (Board[i][j].colour == WHITE){
                    printf("W");
                } else {
                    printf("B");
                }
                printf("%s", Board[i][j].symbol);
            }
            printf("|");
        }
        printf("\n");
    }
    printf("  -------------------------\n");
    printf("   A  B  C  D  E  F  G  H  \n");
}

bool checkMove(int oldx, int oldy, int newx, int newy){
    struct Piece* oldPiece = &Board[oldy][oldx];
    struct Piece* newPiece = &Board[newy][newx];
    //New position must be within 0 and 7 inclusive
    if (!(newx >= 0 && newx <= 7 && newy >= 0 && newy <= 7)){
        return false;
    }
    //If it is the same position it is invalid
    if (newx == oldx && oldy == newy){
        return false;
    }
    if (!(newPiece->type == EMPTY) || (newPiece->colour = oldPiece->colour)){//If the new position is not empty or opposite colour
        return false;//But pawns can't capture forward
    }

    switch (oldPiece->type){
        case PAWN:{
            //Pawns can move twice if it is their first time moving in the same column
            if (newx == oldx){
                if (newPiece->colour != oldPiece->colour){//If the new position is not empty or opposite colour
                    return false;//pawns can't capture forward
                }
                if (oldPiece->colour == WHITE){
                    if (newy - oldy == -1){
                    } else if (newy - oldy == -2 && oldPiece->firstTime && Board[oldy - 1][oldx].type == EMPTY){//But there cannot be a piece in between
                        oldPiece->firstTime = false;
                    } else{
                        return false;
                    }
                } else if (oldPiece->colour == BLACK){
                    if (newy - oldy == 1){
                    } else if (newy - oldy == 2 && oldPiece->firstTime && Board[oldy + 1][oldx].type == EMPTY){
                        oldPiece->firstTime = false;
                    } else{
                        return false;
                    }
                }
            } else {
                if (oldPiece->colour == WHITE && (newx == oldx + 1 || newx == oldx - 1) && (newy == oldy - 1)){//White pieces move up the board
                } else if (oldPiece->colour == BLACK && (newx == oldx + 1 || newx == oldx - 1) && (newy == oldy + 1)){
                } else{
                    return false;
                }
            }
        }
        case ROOK:{
            //Make sure that there aren't any pieces in between its new and old position
            //Rooks can move to any places in the same column
            if (newx == oldx && newy != oldy){
                if (newy > oldy){
                    for (int i = oldy + 1; i < newy; i++){
                        if (Board[i][oldx].type != EMPTY){
                            return false;
                        }
                    }
                } else if (newy < oldy){
                    for (int i = oldy - 1; i > newy; i--){
                        if (Board[i][oldx].type != EMPTY){
                            return false;
                        }
                    }
                }
            } else if (newy == oldy && newx != oldx){//Rooks can move to any places in the same row
                if (newx > oldx){
                    for (int i = oldx + 1; i < newx; i++){
                        if (Board[oldy][i].type != EMPTY){
                            return false;
                        }
                    }
                } else if (newx < oldx){
                    for (int i = oldx - 1; i > newx; i--){
                        if (Board[oldy][i].type != EMPTY){
                            return false;
                        }
                    }
                }
            } else{
                return false;
            }
            break;
        }
        case KNIGHT:{
            if (abs(newx - oldx) == 2 && abs(newy - oldy) == 1){
            } else if (abs(newx - oldx) == 1 && abs(newy - oldy) == 2){ 
            } else{
                return false;
            }
            break;
        }
        case BISHOP:{
            //Bishops can move diagonally both ways
            if (abs(newy - oldy) == abs(newx - oldx)){
                //Make sure that there aren't any pieces in between tis new and old position
                if (newx > oldx && newy > oldy){
                    for (int i = oldx + 1, j = oldy + 1; i < newx && j < newy; i++, j++){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx < oldx && newy < oldy){
                    for (int i = oldx - 1, j = oldy - 1; i > newx && j > newy; i--, j--){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx > oldx && newy < oldy){
                    for (int i = oldx + 1, j = oldy - 1; i < newx && j > newy; i++, j--){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx < oldx && newy > oldy){
                    for (int i = oldx - 1, j = oldy + 1; i > newx && j < newy; i--, j++){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else{
                    return false;
                }
            } else{
                return false;
            }
            break;
        }
        case QUEEN:{
            if (newx == oldx && newy != oldy){
                if (newy > oldy){
                    for (int i = oldy + 1; i < newy; i++){
                        if (Board[i][oldx].type != EMPTY){
                            return false;
                        }
                    }
                } else if (newy < oldy){
                    for (int i = oldy - 1; i > newy; i--){
                        if (Board[i][oldx].type != EMPTY){
                            return false;
                        }
                    }
                }
            } else if (newy == oldy && newx != oldx){//Rooks can move to any places in the same row
                if (newx > oldx){
                    for (int i = oldx + 1; i < newx; i++){
                        if (Board[oldy][i].type != EMPTY){
                            return false;
                        }
                    }
                } else if (newx < oldx){
                    for (int i = oldx - 1; i > newx; i--){
                        if (Board[oldy][i].type != EMPTY){
                            return false;
                        }
                    }
                }
            } else if (abs(newy - oldy) == abs(newx - oldx)){
                //Make sure that there aren't any pieces in between tis new and old position
                if (newx > oldx && newy > oldy){
                    for (int i = oldx + 1, j = oldy + 1; i < newx && j < newy; i++, j++){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx < oldx && newy < oldy){
                    for (int i = oldx - 1, j = oldy - 1; i > newx && j > newy; i--, j--){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx > oldx && newy < oldy){
                    for (int i = oldx + 1, j = oldy - 1; i < newx && j > newy; i++, j--){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else if (newx < oldx && newy > oldy){
                    for (int i = oldx - 1, j = oldy + 1; i > newx && j < newy; i--, j++){
                        if (!(Board[j][i].type == EMPTY)){
                            return false;
                        }
                    }
                } else{
                    return false;
                }
            } else{
                return false;
            }
        }
        case KING:{
            //King can only move within one unit range horizontally, vertically and diagonally
            if(!(abs(newx - oldx) <= 1 && abs(newy - oldy) <= 1)){
                return false;
            }
            break;
        }
    }
    return true;
}






