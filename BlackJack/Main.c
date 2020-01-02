#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

void shuffle(game);
void deal(game, player);
int getFace(int card);
int getSuit(int card);
int createCard(int face, int suit);
void printCard(int card);
void delay(int numSeconds);
void hitMe(game, player);
void playGame(game, player);
int calculatePoints(player);
void hitMe(game, player);
int getPoint(int card);
void checkForBlackJack(game, player);
void winners(game, player);

#define DECKSIZE 52
#define NCARDS 13
#define PASSES 8192
#define PLAYERS 4

typedef struct
{
	int t;
	int np;
	int nd;
	int d[];
}game;

typedef struct
{
	int h[5];
	int points;
	int cardNo;
	int b;
}player;


void main()
{
	int option;
	int shuf;
	game *g = malloc(sizeof(g));
	player players[3];

	srand(time(NULL));
	printf("============================================\n");
	printf("=           WELCOME TO BLACKJACK           =\n");
	printf("============================================\n");

	do {
		printf("\n1. New Game\n");
		printf("2. Load Game\n");
		printf("Option: ");
		scanf("%d", &option);
	} while (option != 1 && option != 2);

	if (option == 1) // New Game
	{
		do {
			printf("\nEnter number of players (2-4)? ");
			scanf("%d", &g->np);
		} while (g->np < 2 || g->np > 4);

		do {
			printf("\nPlayer 1, enter number of decks: ");
			scanf("%d", &g->nd);
			printf("\n");
		} while (g->nd < 1 || g->nd > 4);

		printf("Starting new game.");
		for (int i = 0; i < 3; ++i)
		{
			//delay(1);
			printf(".");
		}

		for (int i = 0; i <= DECKSIZE * g->nd; ++i)
		{
			g->d[i] = i;
		}

		printf("\n\nDealer shuffles the deck");
		for (int i = 0; i < 3; ++i)
		{
			//delay(1);
			printf(".");
		}
		shuffle(g);
		//delay(1);
		printf("\n\nDealer deals the cards to the players");
		for (int i = 0; i < 3; ++i)
		{
			//delay(1);
			printf(".");
		}
		deal(g, players);
		//delay(1);
		printf("\n\nThe Dealer gets the hole card and places it face down\n");
		//delay(1);
		printf("\nThe Dealers face up card is the");
		printCard(players[0].h[1]);
		printf("\n");

		if (getPoint(players[0].h[1]) == 10 || getPoint(players[0].h[1]) == 11)
		{
			printf("\nDealer peeks at his facedown card");
			if (calculatePoints(players, 0) == 21)
			{
				printf("\n\nDealer has a BlackJack!");
				printf("\nDealer wins with a");
				printCard(players[0].h[0]);
				printf(" and a");
				printCard(players[0].h[1]);
			}
			else {
				printf("\nDealer doesn't have a BlackJack!\n");
			}
		}

		playGame(g, players);
	}
	else // Load Game
	{
		
	}
	getch();
}

void shuffle(game *g)
{
	int i;
	int j;
	int tmp;
	int n = PASSES;

	while (n--)
	{
		i = rand() % (DECKSIZE * g->nd);
		j = rand() % (DECKSIZE * g->nd);

		tmp = g->d[i];
		g->d[i] = g->d[j];
		g->d[j] = tmp;
	}

	j = 1;
	for (i = 0; i < DECKSIZE * g->nd; ++i)
	{
		++j;
	}
}

void deal(game *g, player players[])
{
	int card = 0;
    players[3];
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < g->np; i++)
		{
			players[i].h[j] = g->d[card];
			g->d[card] = 0;
			card++;
		}
	}
}

int getFace(int card)
{
	return card % 13;
}

int getSuit(int card)
{
	return card / 13;
}

int createCard(int face, int suit)
{
	return face * suit;
}

void printCard(int card)
{
	static char *suits[4] = { "Spades", "Hearts", "Diamonds", "Clubs" };
	static char *faces[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10",
							"Jack", "Queen", "King", "Ace" };

	printf(" %s of %s", faces[getFace(card)], suits[getSuit(card)]);
}

void delay(int numSeconds)
{
	// Converting time into milli_seconds 
	int milliSeconds = 1000 * numSeconds;

	// Storing start time 
	clock_t startTime = clock();

	// looping till required time is not achieved 
	while (clock() < startTime + milliSeconds);
}

void playGame(game *g, player players[])
{
	int option;

	for (int j = 0; j < g->np; j++)
	{
		players[j].cardNo = 2;
		players[j].points = calculatePoints(players, j);
	}

	for (int i = 1; i < g->np; i++)
	{
		printf("\nPlayer %d got the", i);
		for (int j = 0; j < 2; j++)
		{
			printCard(players[i].h[j]);
			if (j == 0) {
				printf(" and the");
			}
		}
	}

	printf("\n\nThe Dealers points are %d", players[0].points);
	printf("\nPlayer 1's points are %d", players[1].points);
	printf("\nPlayer 2's points are %d", players[2].points);
	printf("\nPlayer 3's points are %d", players[3].points);


	for (int i = 1; i < g->np; i++)
	{
		do
		{
			g->t = i;
			delay(1);
			printf("\n\nPlayer %d's go", i);
			printf("\n1. Stand");
			printf("\n2. Hit");
			printf("\n3. Split\n");
			scanf("%d", &option);

			if (option == 1) // stand
			{
				printf("You choose to Stand.");
				break;
			}
			else if(option == 2) // hit
			{
				hitMe(g, players);
				if (players[i].points > 21)
				{
					break;
				}
			}
		} while (option == 2);
	}
	g->t = 0;
	printf("\n\nThe Dealers hole card was the");
	printCard(players[0].h[0]);
	printf("\n");
	while (players[0].points <= 21)
	{
		hitMe(g, players);
	}
}

void hitMe(game *g, player players[])
{
	int nextCard, j = 4;
	int cardNo = players[g->t].cardNo;
	do
	{
		nextCard = g->d[j];
		g->d[j] = 0;
		j++;
	} while (nextCard == 0);

	players[g->t].h[cardNo] = nextCard;
	players[g->t].cardNo++;

	players[g->t].points += getPoint(nextCard);

	printf("\nYou got the");
	printCard(nextCard);
	printf("\nYou are now on %d points", players[g->t].points);
	checkForBlackJack(g, players);
}

int calculatePoints(player players[], int j)
{
	int points = 0;
	int pointTotal = 0;
	int face;
	
	
	for (int i = 0; i < 2; i++)
	{
		face = getFace(players[j].h[i]);
		switch (face)
		{
			case 0:
				points = 2;
				break;
			case 1:
				points = 3;
				break;
			case 2:
				points = 4;
				break;
			case 3:
				points = 5;
				break;
			case 4:
				points = 6;
				break;
			case 5:
				points = 7;
				break;
			case 6:
				points = 8;
				break;
			case 7:
				points = 9;
				break;
			case 8:
				points = 10;
				break;
			case 9:
				points = 10;
				break;
			case 10:
				points = 10;
				break;
			case 11:
				points = 10;
				break;
			case 12:
				points = 11;
				break;
		}
		pointTotal += points;
	}

	return pointTotal;
}

int getPoint(int card)
{
	int points = 0;
	int face;


	for (int i = 0; i < 2; i++)
	{
		face = getFace(card);
		switch (face)
		{
		case 0:
			points = 2;
			break;
		case 1:
			points = 3;
			break;
		case 2:
			points = 4;
			break;
		case 3:
			points = 5;
			break;
		case 4:
			points = 6;
			break;
		case 5:
			points = 7;
			break;
		case 6:
			points = 8;
			break;
		case 7:
			points = 9;
			break;
		case 8:
			points = 10;
			break;
		case 9:
			points = 10;
			break;
		case 10:
			points = 10;
			break;
		case 11:
			points = 10;
			break;
		case 12:
			points = 11;
			break;
		}
	}

	return points;
}

void checkForBlackJack(game *g, player players[])
{
	if (players[g->t].points == 21)
	{
		printf("\n\nPlayer %d got BlackJack!", g->t);
	}
	else if (players[g->t].points > 21)
	{
		if (g->t == 0)
		{
			printf("\n\nDealer goes Bust!\n");
			players[g->t].b = 1;
		}
		else {
			printf("\n\nPlayer %d goes Bust!\n", g->t);
			players[g->t].b = 1;
		}
	}
	winners(g, players);
}

void winners(game *g, player players[])
{
	if (players[0].b == 1)
	{
		for (int i = 1; i < g->np; i++)
		{
			if (players[i].b != 1)
			{
				printf("\nPlayer %d wins with %d points!", i, players[i].points);
			}
		}
	}
}