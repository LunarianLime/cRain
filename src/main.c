/**
 * Rain - a random shell command thingy I made
 * Version: 1.0
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define rainPeriod 1
#define maxRaindrops 200
char rainSymbol = '|';

struct Size
{
	int height;
	int width;
};

struct Size wsize;

struct RainDrop
{
	int x;
	int y;
};

struct RainDrop raindrops[maxRaindrops];

void printAt(int x, int y, char c)
{
	printf("\033[%d;%dH%c", y, x, c);
	printf("\n");
}

void rain()
{
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 50000000;

	int stepsElapsed = 0;
	int rainCount = 0;

	while(1)
	{
		nanosleep(&tim, &tim2);

		if(stepsElapsed % rainPeriod == 0 && rainCount + 1 != maxRaindrops)
		{
			raindrops[rainCount].x = (rand() % wsize.width);
			raindrops[rainCount].y = 0;
			rainCount++;
		}

		for(int i = 0; i < maxRaindrops; i++)
		{
			int pos_x = raindrops[i].x;
			int pos_y = raindrops[i].y;

			if(pos_x == 0) continue;

			if(pos_y != 0) printAt(pos_x, pos_y - 1, ' ');
			printAt(pos_x, pos_y, rainSymbol);	

			if(pos_y == wsize.height - 1)
			{
				printAt(pos_x, pos_y, ' ');
				raindrops[i].x = rand() % wsize.width;
				raindrops[i].y = 0;
			}
			raindrops[i].y++;
		}

		stepsElapsed++;
	}
}

int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		return -1;
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
	return -1;
}

void init(int argc, char *argv[])
{
	getWindowSize(&wsize.height, &wsize.width);	
	system("clear");
	printf("\33[?25l");

	printf("\33[34m");

	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0 && i+1 < argc)
		{
			rainSymbol = argv[i+1][0];
			i++;
		}
		else if(strcmp(argv[i], "-c") == 0 && i+1 < argc)
		{
			int colour = atoi(argv[i+1]);
			i++;

			switch(colour)
			{
				case 0:
					printf("\33[30m");
					break;
				case 1:
					printf("\33[31m");
					break;
				case 2:
					printf("\33[32m");
					break;
				case 3:
					printf("\33[33m");
					break;
				case 4:
					printf("\33[34m");
					break;
				case 5:
					printf("\33[35m");
					break;
				case 6:
					printf("\33[36m");
					break;
				case 7:
					printf("\33[37m");
					break;
				default:
					break;
			}
		}
		else if(strcmp(argv[i], "-h") == 0)
		{
			printf("\33[0m");
			printf("rain <arg>\n\t");
				       	printf("Argument list:\n");
					printf("-h: display this message\n");
					printf("-s <char>: char to use as a symbol for the rain particles\n\t");
						printf("('|' by default)\n");
					printf("-c <int>: changes the colour of rain\n\t");
						printf("Blue by default\n\t");
						printf("(1 - Red)\n\t");
						printf("(2 - Green)\n\t");
						printf("(3 - Yellow)\n\t");
						printf("(4 - Blue)\n\t");
						printf("(5 - Purple)\n\t");
						printf("(6 - Cyan)\n\t");
						printf("(7 - White)\n\t");
			
			exit(0);			
		}
		else 
		{
			printf("Unrecognized argument; use -h for a list of arguments\n");
			exit(0);
		}
	}	
}

int main(int argc, char *argv[])
{
	init(argc, argv);
	rain();

	return 0;
}
