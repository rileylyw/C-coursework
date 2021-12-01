#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define FILENAME "maze.txt"
#define GRID 100

void test(void);
void readMazeSize(int *xSize, int *ySize, char maze[GRID][GRID]);
bool explore(char maze[GRID][GRID], int xSize, int ySize, int x, int y);

int main(int argc, char* argv[]){
    int xSize, ySize;
    char maze[GRID][GRID];
    readMazeSize(&xSize, &ySize, maze);

    // for(int i=0; i<xSize; i++){
    //     for(int j=0; j<ySize; j++){
    //         printf("%c", maze[i][j]);
    //     }
    //     printf("\n");
    // }

    if(explore(maze, xSize, ySize, 1, 0)){
        printf("%s\n", "ok");
    }
    else{
        printf("%s\n", "not ok");
    }
    // int count=0;
    // for(int i=1; i<=xSize*2; i++){
    //     printf("%s", maze[i]);
    // }

    return 0;
}

void readMazeSize(int *xSize, int *ySize, char maze[GRID][GRID]){
    FILE* fp=fopen(FILENAME, "r");
    if(fp==NULL){
        fprintf(stderr, "Cannot open file %s\n", FILENAME);
        exit(EXIT_FAILURE);
    }
    else{
        fscanf(fp, "%d, ", xSize);
        fscanf(fp, "%d\n", ySize); //fp pointer at the end of 1st line
    }

    int count=0;
    // char *line = maze[count];
    while(fgets(maze[count], (*ySize)+3, fp)){ //get whole line
        maze[count][*ySize]='\0';
        // printf("%s", maze[count]);
        count++;
    }
        // for(int i=0; i<*ySize; ++i){
        //     printf("c: %c", maze[1][0]);
        // }
        // count++;
    // printf('%c\n', maze[0][0]);
    fclose(fp);
}

bool explore(char maze[GRID][GRID], int xSize, int ySize, int x, int y){
    if(x<0 || y<0 || x>xSize-1 || y>ySize-1){ //check inbound
        return false;
    }
    if(maze[x][y] == '#'){ //if already has #
        // printf("%c", maze[x][y]);
        return false;
    }
    if(maze[x][y]=='x'){
        return false;
    }
    if (x==xSize-1 || y==ySize-1){ //arrive exit
        // if(maze[x][y]==' '){
            // printf("c: %c\n", maze[x][y]);
            // printf("x y: %d %d", x, y);

            return true;
        // }
    }

    // if(maze[x][y]==maze[xSize-2][ySize-1]){ //arrive exit
    //     if(maze[x][y]==' '){
    //         printf("c: %c", maze[x][y]);
    //         return true;
    //     }
    // }
    maze[x][y]='x';

    if(explore(maze, xSize, ySize, x-1, y)){ //up
        return true;
    }
    if(explore(maze, xSize, ySize, x, y+1)){ //right
        return true;
    }
    if(explore(maze, xSize, ySize, x, y-1)){ //left
        return true;
    }
    if(explore(maze, xSize, ySize, x+1, y)){ //down
        return true;
    }
    return false;
}

void test(void){
    // assert()
}
