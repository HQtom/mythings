#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char player1name[20];
char player2name[20];
int currentPlayer = 0;
char **gameborad;
char state[20]="Game is in palying";
int terminate = 0;
int i,j;


//The function that close the game and release the memory
void TearDown()
{
	printf("\nThe game has finished\n");
	printf("\nDestroy the game\n");

	for (i = 0; i < 6; i++) { // free the borad
		free(gameborad[i]);
	}
	free(gameborad);
	terminate = 1;
	
}




// The function that create the game and allocate the memory to the game borad
void Initialization()
{
	printf("\n------Setting up the game, welcome to the Connect 4 :).------\n");
	printf("\n------ Author : Haowei Qing      ICSI333    -----------------\n");
	printf("\n------ Player 1  ==> hold disc ①, Player 2 ==>hold disc ②--\n");
	printf("\n------player1 enter your name:------\n");
	scanf("%s",player1name);
	printf("\n------player2 enter your name:------\n");
	scanf("%s",player2name);

	gameborad = (char **)malloc(sizeof(char *) * 6);  // mem allocation for the game panel, 6 rows
	for (i = 0; i < 6; ++i) {
		gameborad[i] = (char *)malloc(sizeof(char) * 7); // allocate mem for 7 columes
	}

	for (i = 0; i < 6; i++) {             //initialize the pannel
		for (j = 0; j < 7; j++) {
			gameborad[i][j] = 'O';
		}

	}
}	


// The function acce[t input from the key borad, identifying the wrong input and ask user to input again
char AcceptInput()
{
	fflush(stdin);                 // clean the input buffer
	char buff[10];
	char disc;
	printf("-----choose a columns(A-G) you want to insert or type Q to quit the game");
	printf("\n----- plese just enter one character only for each steps\n");
	

	// input filter
	while (1) {
		scanf("%s", buff);
		if (strlen(buff) > 1) {
			printf("I said character only please follow the rule, redo the type\n");
			fflush(stdin);
			continue;
		}
		disc = buff[0];
		if (disc == 'A' || disc == 'B' || disc == 'C' || disc == 'D' || disc == 'E'|| disc == 'F'|| disc == 'G') {
			return disc;
			break;
		}
		else if (disc == 'Q') {
			terminate = 1;
			printf("Game closed\n");
			break;
		}
		else{
			printf("-----Can't find the column, please input a valid character!!\n");
			fflush(stdin);
			continue;   // reset disc
		}

	}
	
}



// updatye the game by input data
void Update(char input) {
	int count = 0;
	int cood = 0;  //colume id
	char d; // disc over borad
	if (input == 'A') {
		cood = 0;
	}
	else if (input == 'B'){
		cood = 1;
	}
	else if (input == 'C') {
		cood = 2;
	}
	else if (input == 'D') {
		cood = 3;
	}
	else if (input == 'E') {
		cood = 4;
	}
	else if (input == 'F') {
		cood = 5;
	}
	else if (input == 'G') {
		cood = 6;
	}
	// insert the disc into the borad, report error if the colume if full
	if (currentPlayer == 1) {
		d = 'A';
	}
	if (currentPlayer == 2) {
		d = 'B';
	}


	// drop the disc
	while (1) {
		count = 0;
		for (i = 0; i < 6; i++) {
			if (gameborad[i][cood] != 'O') {
				count++;
			}
		}
		if (count == 6) {
			printf("\n\nThis colume is full, change to another\n\n");
			break;
		}
		else {
			count = 5 - count;
			gameborad[count][cood] = d;
			break;
		}
	}
	

	





	count = 0;			//countrt back to 0
									// start to check if the game borad is full
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			if (gameborad[i][j] != 'O') {
				count++;
			}
		}
	}
	if (count == 42) {
		printf("\nGame borad is full, game is over.\n");
		terminate = 1;
	}
}


// display the borad
void display() {
	printf("\n");
	
	printf("    A      B      C      D      E      F      G  ");
	
	for ( i = 0; i < 6; i++) {
		printf("\n--------------------------------------------------\n");
			for (j = 0; j < 7; j++) {
				if (gameborad[i][j] == 'A') {
					printf(" | ① |" );
				}
				if (gameborad[i][j] == 'B') {
					printf(" | ② |" );
				}
				else if((gameborad[i][j] == 'O')){
					printf(" | ○ |");
				}
			
			//printf(" | %c | ", gameborad[i][j]);
		}
		printf("\n--------------------------------------------------\n");
	}
	if (terminate == 1){ 
		printf("\n%s", state); 
	}
	printf("\n");
}

void winChecker() {
	char recent;
	int counter;   // if counter is 4 then win
	int rowTemp;  //temp number of row index
	int colTemp;   // same for colume
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			//Check every possible disc
			if ('O' != gameborad[i][j])
			{
				recent = gameborad[i][j];
				counter = 1;
				rowTemp = i;
				colTemp = j;

				//horizontal checkr
				while ((++colTemp < 7) && (4 != counter))
				{
					if (recent == gameborad[i][colTemp])
					{
						counter++;
						if ( counter == 4)
						{
							if (recent == 'A')
							{
								printf("Player 1 %s win!!!!\n",player1name);
							}
							else if (recent == 'B')
							{
								printf("Player 2 %s win !!!!\n",player2name);
							}
							terminate = 1;
							break;
						}
					}
					else
					{
						counter = 0;
						break;
					}
				}

				//vertical checker
				rowTemp = i;
				colTemp = j;
				counter = 1;
				while ((++ rowTemp < 6) && (4 != counter))
				{
					if (recent == gameborad[rowTemp][j])
					{
						counter++;
						if (counter == 4)
						{
							if (recent == 'A')
							{
								printf("Player 1 %s win!!!!\n", player1name);
							}
							else if (recent == 'B')
							{
								printf("Player 2 %s win !!!!\n", player2name);
							}
							terminate = 1;
							break;
						}
					}
					else
					{
						counter = 0;
						break;
					}

				}

				//diagnoal checker 1
				rowTemp = i;
				colTemp = j;
				counter = 1;
				while ((++rowTemp < 6) && (++colTemp < 7) && (4 != counter))
				{
					if (recent== gameborad[rowTemp][colTemp])
					{
						counter++;
						if (counter == 4)
						{
							if (recent == 'A')
							{
								printf("Player 1 %s win!!!!\n", player1name);
							}
							else if (recent == 'B')
							{
								printf("Player 2 %s win !!!!\n", player2name);
							}
							terminate = 1;
							break;
						}
					}
					else
					{
						counter = 0;
						break;
					}
				}

				//diagnoal checker 2
				rowTemp = i;
				colTemp = j;
				counter = 1;
				while ((++rowTemp < 6) && (--colTemp >= 0) && (4 != counter))
				{
					if (recent == gameborad[rowTemp][colTemp])
					{
						counter++;
						if (counter == 4)
						{
							if (recent == 'A')
							{
								printf("Player 1 %s win!!!!\n", player1name);
							}
							else if (recent == 'B')
							{
								printf("Player 2 %s win !!!!\n", player2name);
							}
							terminate = 1;
							break;
						}
					}
					else
					{
						counter = 0;
						break;
					}
				}

			}

		}

	}
}







int main() {
	char c;
	Initialization();
	display(); //display the initail plane
	currentPlayer = 1;
	while (1) {
		if (currentPlayer == 1) {
			printf("Player: %s turn\n", player1name);
		}
		else {
			printf("Player: %s turn\n", player2name);
		}
		c = AcceptInput();
		if (terminate == 1) {
			TearDown();
			break;
		}
		
		Update(c);
		display();
		winChecker();
		if (terminate == 1) {
			TearDown();
			break;
		}
		
		
		if (currentPlayer == 1) {  //player switch
			currentPlayer++;
		}
		else {
			currentPlayer--;
		}
	}
}


