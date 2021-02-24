#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include <netdb.h>
/*
ICSI333 project Connect 4 part 3
Haowei Qing & Qian Wang
*/
int currentPlayer = 0;
char player1name[20];
char player2name[20];
char **gameborad;
int terminate = 0;
int i,j;
int c_sock;
int client;
int ssock;
/**
Function: TearDown
-------------------------------
The function that close the game and release the memory
*/
void TearDown(int s)
{
	printf("\nThe game has finished\n");
	printf("\nDestroy the game\n");
	for (i = 0; i < 6; i++) { // free the borad
		free(gameborad[i]);
	}
	free(gameborad);
	close(s);// free network resorce, close the socket
	terminate = 1;
}
/**
Function: Initialization
-------------------------------
Function that initialization the game.setup players name and allocates memory.
set up the server and socket. 
*/
void Initialization(int i, char *argv2, char *argv3)
{
	
	if (i == 0) {//for server
		printf("Initiallizing server\n");
		struct sockaddr_in s;
		struct sockaddr_in c;
		socklen_t addr_size;
		ssock = socket(PF_INET, SOCK_STREAM, 0);
		if (ssock == -1)
		{
			perror("Socket.\n");
			
		}
		s.sin_port = htons(1776);
		s.sin_addr.s_addr = htonl(INADDR_ANY);
		s.sin_family = AF_INET;
		int bid = bind(ssock, (struct sockaddr*)&s, sizeof(s));
		if (bid == -1)
		{
			perror("Bind.\n");
		}	
		printf("Server information\n");
		printf("server IP:%s\n", inet_ntoa(s.sin_addr));
		printf("socket is:%d\n", 1776);
		printf("Connection method: ./(name) client IP Port.\n");
		if ((listen(ssock, 1)) == -1)
		{
			perror("Listen\n");
			
		}
		addr_size = sizeof(c);
		if ((client = accept(ssock, (struct sockaddr*)&c, (socklen_t*)&addr_size)) < 0)//for client
		{
			perror("accept");
		}
	}
	else if (i = 1) {
		struct sockaddr_in serv_add;
		if ((c_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket");
		}
		serv_add.sin_family = AF_INET;
		serv_add.sin_port = htons(atoi(argv3));
		if (inet_pton(AF_INET, argv2, &serv_add.sin_addr) <= 0)
		{
			perror("socket\n");
		}
		if (connect(c_sock, (struct sockaddr*)&serv_add, sizeof(serv_add)) < 0)
		{
			perror("\nConnection");
			
		}
	}
	printf("\n------Setting up the game, welcome to the Connect 4 :).------\n");
	printf("\n------ Author : Haowei Qing      ICSI333    -----------------\n");
	printf("\n------ Player 1  ==> hold disc ①, Player 2 ==>hold disc ②--\n");
	//printf("\n------player1 enter your name:------\n");
	//scanf("%s",player1name);
	//printf("\n------player2 enter your name:------\n");
	//scanf("%s",player2name);
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
/**
Function: AcceptInput
-------------------------------
Function that findout the input, detect the choosen column
*/char AcceptInput()
{
	char ch;
	while((ch = getchar())!='\n'&&ch != EOF)
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
			return disc;
			break;
		}
		else{
			printf("-----Can't find the column, please input a valid character!!\n");
			fflush(stdin);
			continue;   // reset disc
		}
	}
}
/**
Function: Update
-------------------------------
Function that put the disc into the column that player chosen.
*/void Update(char input) {
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
/**
Function: display
-------------------------------
Function that display the game's situation
*/void display() {
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
		printf("Game is running"); 
	}
	printf("\n");
}
/**
Function: winChecker
-------------------------------
Function that check the winner, connect 4 disc from vertical, horizontal or diagonal
*/
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
/**
Function: main
-------------------------------
Function that about game order and player's term
*/
int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("No argument input\n");
		return 0;
	}
	char* argv2 = argv[2];
	char* argv3 = argv[3];
	char** boradbuffer;
	char* buff;
	buff = (char*)malloc(sizeof(char*) * 100);
	char msg[20];
	char cc;
	if (strcmp(argv[1],"server")==0) {//server main function
		int q = 0;
		Initialization(q," "," ");
		printf("\n------server,player1 enter your name:------\n");
		scanf("%s", player1name);
		send(client, player1name, strlen(player1name), 0);
		read(client, player2name, 1000);
		display();
		currentPlayer = 1; // when = 1, player1 turn, when = 2 player 2 turn
		while (1) {
			if (currentPlayer == 1) {//playing stage
				printf("Player: %s turn\n", player1name);
				cc = AcceptInput();
				buff[0] = cc;
				send(client, buff, strlen(buff), 0);
				//send data to client
				if (terminate == 1) {
					TearDown(ssock);
					break;
				}
				Update(cc);
				display();
				winChecker();
				if (terminate == 1) {
					TearDown(ssock);
					break;
				}
				currentPlayer++;
			}
			else {//waiting stage
				printf("Player: %s turn, please wait\n", player2name);
				printf("Warning: your may input right now but the input will be applied on your next turn.\n");
				printf("Away from your keyborad when other player is thinking, be nice.\n");
				read(client, buff,1000);
				cc = buff[0];//get the data from client
				if (cc == 'Q') {
					terminate =1;
				}
				if (terminate == 1) {
					TearDown(ssock);
					break;
				}
				Update(cc);
				display();
				winChecker();
				if (terminate == 1) {
					TearDown(ssock);
					break;
				}
				currentPlayer--;
			}
		}
	}
	else if (strcmp(argv[1],"client")==0) {//client main function
		int w = 1;
		Initialization(w,argv2,argv3);
		printf("\n------client,player2 enter your name:------\n");
		scanf("%s", player2name);
		send(c_sock, player2name, strlen(player2name), 0);
		read(c_sock, player1name, 1000);
		display();
		currentPlayer = 1;
		while (1) {
			if (currentPlayer == 1) {//waiting stage
				printf("Player: %s turn, please wait\n", player1name);
				printf("Warning: your may input right now but the input will be applied on your next turn.");
				printf("Away from your keyborad when other player is thinking, be nice.");
				read(c_sock, buff, 1000);
				cc = buff[0];
				//get data from server
				if (cc == 'Q') {
					terminate = 1;
				}
				if (terminate == 1) {
					TearDown(c_sock);
					break;
				}
				Update(cc);
				display();
				winChecker();
				if (terminate == 1) {
					TearDown(c_sock);
					break;
				}
				currentPlayer++;
			}
			else {//playing stage
				printf("Player: %s turn\n", player2name);
				cc = AcceptInput();
				buff[0] = cc;
				send(c_sock, buff, strlen(buff), 0);
				//send data to server
				if (terminate == 1) {
					TearDown(c_sock);
					break;
				}
				Update(cc);
				display();
				winChecker();
				if (terminate == 1) {
					TearDown(c_sock);
					break;
				}
				currentPlayer--;
			}
		}
	}
	else {
		printf("Can't identify the argument");
	}
}


