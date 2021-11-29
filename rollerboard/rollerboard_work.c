#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define CRN_RBD_MATRIX 3

typedef struct board{
   char rollerBoard[6][6];
   struct board *next;
} board;

typedef struct{
   board *f, *r;
} listOfBoards;
/**
 * @brief [QUEUE OPERATION] Enqueue the Queue of Boards
 * 
 * @param currentList The current state of queue to enqueue the next Board
 * @param rollerBoard The Rollerboard to enqueue
 * @param RBD_ROWS The number of Rows of RollerBoard read from the given command-line argument
 */
void enqueue(listOfBoards *currentList, char rollerBoard[][6], int RBD_ROWS){
   int i;
   board *p = (board *) malloc(sizeof(board));
   for (i = 0; i < RBD_ROWS; i++) {
      strcpy(p -> rollerBoard[i], rollerBoard[i]);
   }

   p -> next = NULL;
   if (currentList -> r == NULL) {
      currentList -> f = currentList -> r = p;
   } else {
      currentList -> r -> next = p;
      currentList -> r = p;
   }
}
/**
 * @brief [QUEUE OPERATION] Append the row to the front of queue
 * 
 * @param currentList The current state of queue
 * @param rollerBoard The Rollerboard to enqueue
 * @param RBD_ROWS The number of Rows of RollerBoard read from the given command-line argument
 */
void front(listOfBoards * currentList, char rollerBoard[][6], int RBD_ROWS) {
    int i;
    board * p = currentList -> f;
    for (i = 0; i < RBD_ROWS; i++) {
        strcpy(rollerBoard[i], p -> rollerBoard[i]);
    }
}
/**
 * @brief [QUEUE OPERATION] Dequeue the Queue of Boards
 * 
 * @param currentList The current state of queue to dequeue the Board
 * @return int 1 if successfully dequeued, 0 if the queue has been emptied out
 */
int dequeue(listOfBoards * currentList) {
    if (currentList -> f == NULL)
        return 0;
    board * newboard = currentList -> f;
    if (currentList -> f == currentList -> r) {
        currentList -> f = currentList -> r = NULL;
    } else {
        currentList -> f = currentList -> f -> next;
    }
    free(newboard);
    return 1;
}

int empty(listOfBoards * currentList) {
    if (currentList -> f == NULL)
        return 1;
    return 0;
}
void visit(listOfBoards * currentList, int k, int RBD_ROWS, int verbose) {
    int i, j = 0;
    board * p = currentList -> f;
    if (p == NULL) {
        return;
    }
    while (p != NULL) {
        j++;
        if (j == k && !verbose) {
            for (i = 0; i < RBD_ROWS; i++) {
                printf("%s\n", p -> rollerBoard[i]);
            }
            break;
        }

        p = p -> next;
    }
	if (!verbose)
	    printf("\n");
}
int count(listOfBoards * currentList, char RBD_CONTENT[][6], int RBD_ROWS) {
    int i;
    board * p = currentList -> f;
    if (p == NULL) {
        return 0;
    }
    int id = 1;
    while (p != NULL) {
        int flag = 0;

        for (i = 0; i < RBD_ROWS; i++) {
            if (strcmp(RBD_CONTENT[i], p -> rollerBoard[i]) == 0) {
                flag++;
            }
        }
        if (flag == RBD_ROWS) {
            return id;
        }
        id++;
        p = p -> next;
    }
    return 0;
}
void terminateQueue(listOfBoards * currentList) {
    while (dequeue(currentList));
    free(currentList);
}
/**
 * @brief Initialize the Boards Queue
 * 
 * @return listOfBoards*
 */
listOfBoards * init() {
    listOfBoards * currentList = (listOfBoards * ) malloc(sizeof(listOfBoards));
    currentList -> f = currentList -> r = NULL;
    return currentList;
}

/**
 * @brief 
 * 
 * @param currentList Queue of Boards being used throughout the program 
 * @param which element of parentBoard currently in use
 * @param parentBoard The parent board array to track the parent element
 * @param movesTracked (int) Track each move made
 * @param RBD_ROWS (int) The number of rows of Rollerboard read from the Rollerboard file
 * @param verbose (int) The flag to specify wether or not to print the moves performed
 */
void path(listOfBoards * currentList, int which, int * parentBoard, int * movesTracked, int RBD_ROWS, int verbose) {
    if (which == 0) {
        return;
    }
    path(currentList, parentBoard[which], parentBoard, movesTracked, RBD_ROWS, verbose);
    if (!verbose)
		printf("%d:\n", ( * movesTracked) ++);
	else
		( * movesTracked) ++;
    visit(currentList, which, RBD_ROWS, verbose);
}
/**
 * @brief Roll the coloumn of the roller board
 * 
 * @param currentList (listOfBoards *) Queue of Boards being used throughout the program 
 * @param visitedQueue (listOfBoards *) Queue of Boards that's been visited
 * @param temporaryRollerBoard (char [][]) A Roller Board to temporarily assign values
 * @param RBD_ROWS (int) The number of rows of Rollerboard read from the Rollerboard file
 * @param parentBoard (int *) The parent board array to track the parent element
 * @param size (int *) The element of Parent Board
 */
void rollColoumn (listOfBoards * currentList, listOfBoards * visitedQueue, char temporaryRollerBoard[][6], int RBD_ROWS, int* parentBoard, int* size, int ba) {
	int j;
	if (count(visitedQueue, temporaryRollerBoard, RBD_ROWS) == 0) {
		enqueue(visitedQueue, temporaryRollerBoard, RBD_ROWS);
		parentBoard[*size] = ba;
		enqueue(currentList, temporaryRollerBoard, RBD_ROWS);
		(*size)++;
	}
}
/**
 * @brief Roll the row of the roller board
 * 
 * @param currentList (listOfBoards *) Queue of Boards being used throughout the program 
 * @param visitedQueue (listOfBoards *) Queue of Boards that's been visited
 * @param temporaryRollerBoard (char [][]) A Roller Board to temporarily assign values
 * @param RBD_ROWS (int) The number of rows of Rollerboard read from the Rollerboard file
 * @param parentBoard (int *) The parent board array to track the parent element
 * @param size (int *) The element of Parent Board
 */
void rollRow (listOfBoards * currentList, listOfBoards * visitedQueue, char temporaryRollerBoard[][6], int RBD_ROWS, int* parentBoard, int* size, int ba) {
	int j;
	if (count(visitedQueue, temporaryRollerBoard, RBD_ROWS) == 0) {
		enqueue(visitedQueue, temporaryRollerBoard, RBD_ROWS);
		parentBoard[*size] = ba;
		// for(j=0;j<RBD_ROWS;j++){
		// 	printf("%s\n",temporaryRollerBoard[j]);
		// }
		// printf("------------------\n");
		enqueue(currentList, temporaryRollerBoard, RBD_ROWS);
		(*size)++;
	}
}
/**
 * @brief Breadth-First using brute force to find number of moves performed on a given Rollerboard
 * 
 * @param RBD_ROWS (int) The number of rows of Rollerboard read from the Rollerboard file
 * @param RBD_COLS (int) The number of coloumns of Rollerboard read from the Rollerboard file
 * @param BOARD (char**) The ROLLERBOARD itself, to perform the moves on
 * @param verbose (int) The flag to specify wether or not to print the moves performed
 */
void search(int RBD_ROWS, int RBD_COLS, char ** BOARD, int verbose, int assertion) {
   int size = 1;
   int top = 0;
	int sorted = 0;
   int parentBoard[102400];
   int movesTracked = 0;
    listOfBoards * currentList = init();
    listOfBoards * visitedQueue = init();
    int i, j;
    char RBD_CONTENT[6][6];
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            RBD_CONTENT[i][j] = BOARD[i][j]; //add initial board
        }
    }

    enqueue(visitedQueue, RBD_CONTENT, RBD_ROWS);
    enqueue(currentList, RBD_CONTENT, RBD_ROWS);

   //  parentBoard[1] = 0;
    size++;
    while (!empty(currentList)) {
        int i, j;
        front(currentList, RBD_CONTENT, RBD_ROWS);
        int onesInFirstRow = 0;
        for (i = 0; i < RBD_COLS; i++) {
            if (RBD_CONTENT[0][i] == '1') {
                onesInFirstRow++;
            }
        }
        if (top < onesInFirstRow) {
            top = onesInFirstRow;
        }
        int ba = count(visitedQueue, RBD_CONTENT, RBD_ROWS); //element of parentBoard in use

		// If all elements in the first row are ones, the board is sorted.
        if (onesInFirstRow == RBD_COLS) {
            path(visitedQueue, ba, parentBoard, & movesTracked, RBD_ROWS, verbose);
			sorted = 1;
            break;
        }
        char temporaryRollerBoard[6][6];

        dequeue(currentList);

        for (i = 0; i < RBD_COLS; i++) {
			/**
			 * @brief (BOUNDARY) [START] COLOUMN ROLL DIRECTION DETERMINATION BLOCK
			 *             Determine the direction of RollerBoard to roll
			 *             Possible Directions : {UP, DOWN}
			 */
            int t = 0; //number of 1 in a row
            for (j = 0; j < RBD_ROWS; j++)
                strcpy(temporaryRollerBoard[j], RBD_CONTENT[j]);

            char p = temporaryRollerBoard[0][i];
            for (j = 1; j < RBD_ROWS; j++) {
                temporaryRollerBoard[j - 1][i] = temporaryRollerBoard[j][i]; //up
            }
            temporaryRollerBoard[RBD_ROWS - 1][i] = p; //roll over
            for (j = 0; j < RBD_COLS; j++) {
                if (temporaryRollerBoard[0][j] == '1') {
                    t++;
                }
            }
			/**
			 * @brief (BOUNDARY) [END] DIRECTION DETERMINATION BLOCK
			 * 
			 */
            if (t >= top) {
				if (assertion) {
					assert(t >= top);
					printf("[BOARD COL ROLLED]   Times Rolled  : %d\n", t);
				}
				rollColoumn (currentList, visitedQueue, temporaryRollerBoard, RBD_ROWS, parentBoard, &size, ba);
            }
        }
        for (i = 0; i < RBD_ROWS; i++) {
			/**
			 * @brief (BOUNDARY) [START] ROW ROLL DIRECTION DETERMINATION BLOCK
			 *             Determine the direction of RollerBoard to roll
			 *             Possible Directions : {LEFT, RIGHT}
			 */
            for (j = 0; j < RBD_ROWS; j++)
                strcpy(temporaryRollerBoard[j], RBD_CONTENT[j]);

            char p = temporaryRollerBoard[i][0];
            for (j = 1; j < RBD_COLS; j++) {
                temporaryRollerBoard[i][j - 1] = temporaryRollerBoard[i][j];
            }
            temporaryRollerBoard[i][RBD_COLS - 1] = p;

            int t = 0;
            for (j = 0; j < RBD_COLS; j++) {
                if (temporaryRollerBoard[0][j] == '1') {
                    t++;
                }
            }
			/**
			 * @brief (BOUNDARY) [END] DIRECTION DETERMINATION BLOCK
			 * 
			 */
            if (t >= top) {
				if (assertion) {
					assert(t >= top);
					printf("[BOARD ROW ROLLED]   Times Rolled  : %d\n", t);
				}
				rollRow (currentList, visitedQueue, temporaryRollerBoard, RBD_ROWS, parentBoard, &size, ba);
            }
        }
    }
	if (!sorted) {
		printf("No Solution.");
		exit(EXIT_FAILURE);
	}
	if (verbose) {
		printf("%d moves", movesTracked-1);
	}
	if (assertion) {
		printf("[Post-Sorted State]     Board Successfully Sorted  \t: ");
		assert(sorted == 1);
		printf("TRUE\n[Searched Queue State]  Queue exists\t\t\t\t: ");
		assert(currentList -> f != NULL);
		printf("TRUE\n");
	}
    terminateQueue(currentList);
    terminateQueue(visitedQueue);
	if (assertion) {
		printf("[Searched Queue Terminated]  Queue exists\t\t\t: ");
		assert(currentList);
		printf("FALSE\n");
	}
}
char ** read(char file[], int * rows, int * cols) {
    FILE * specifiedRBD;
    int i, j;
    char ** content = (char ** ) malloc(sizeof(char * ) * 6);

    for (i = 0; i < 6; i++) {
        content[i] = (char * ) malloc(sizeof(char) * 6);
        for (j = 0; j < 6; j++) {
            content[i][j] = ' ';
        }
    }

    specifiedRBD = fopen(file, "r");

    if (specifiedRBD == NULL) {
        printf("[FAILURE] Command-line argument not specified or the file was not found.");
        exit(EXIT_FAILURE);
    }

    if (fscanf(specifiedRBD, "%d%d", rows, cols) < 0) {
        printf("[%s] [READ FAILURE] ROW and COLOUMN definitions incorrectly specified", file);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < * rows; i++) {
        if (fscanf(specifiedRBD, "%s", content[i]) < 0) {
            printf("[%s] [READ FAILURE] Rollerboard definition incorrectly specified", file);
            exit(EXIT_FAILURE);
        }
    }
    return content;
}


void assertCases(void) {
	listOfBoards * testQueue;
	char ** corneredBoard;
	int XO, RO;
	char testBoard[CRN_RBD_MATRIX][6] = {
		{'1','1','1'},
		{'0','0','0'},
		{'0','0','0'}
	};
	printf("\n\n[-ASSERTION-] Queue Operations\n");
	printf("[Pre-Initialized State]  Front is unassigned: ");
	assert(testQueue -> f != NULL);
	printf("TRUE\n[Post-Initialized State] Front is NULL      : ");
	testQueue = init();
	assert(testQueue -> f == NULL);
	printf("TRUE\n[Pre-Enqueued State]     Front is NULL      : TRUE\n[Post-Enqueued State]    Board is assigned  : ");
	enqueue(testQueue, testBoard, CRN_RBD_MATRIX);
	assert(testQueue -> f != NULL);
	printf("TRUE\n[Pre-Dequeued State]     Front is NULL      : FALSE\n[Post-Dequed State]      Front is NULL      : ");
	dequeue(testQueue);
	assert(testQueue -> f == NULL);
	printf("TRUE\n");

	printf("\n[-ASSERTION-] Search Operations\n[-READING 3x3 CORNERED BOARD-]\n");
	corneredBoard = read("3x3crn.rbd", &RO, &XO);
	printf("Cornered Board [1][2]     : ");
	assert(corneredBoard[1][2] == '1');
	printf("%c\nCornered Board [2][1]     : ", corneredBoard[1][2]);
	assert(corneredBoard[2][1] == '1');
	printf("%c\nCornered Board [2][2]     : ", corneredBoard[2][1]);
	assert(corneredBoard[2][2] == '1');
	printf("%c\n[-3x3 CORNERED BOARD CONFIRMED-]", corneredBoard[2][2]);
	search(CRN_RBD_MATRIX, CRN_RBD_MATRIX, corneredBoard, 1, 1);
    
}
/**
 * @brief [MAIN]
 * 
 * @param argc Arguments count
 * @param argv Arguements value
 */
int main(int argc, char * argv[]) {
    int RBD_ROWSX, RBD_COLSX, i, j;
    char file[52];
	int verbose = 1;
    if (argc == 3) {
        strcpy(file, argv[2]);
		verbose = (strcmp(argv[1],"-v") == 0) ? 0 : 1;
    } 
	else if (argc == 2) {
        strcpy(file, argv[1]);
    }
    char ** RBD_CONTENT = read(file, & RBD_ROWSX, & RBD_COLSX);

    search(RBD_ROWSX, RBD_COLSX, RBD_CONTENT, verbose, 0);

	/**
	 * UNCOMMENT THE CALL BELOW TO RUN ASSERTION TEST CASES
	 */
	
	// assertCases();
}