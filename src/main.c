/**
 * Rain - a random shell command thingy I made
 * Version: 1.0
**/

#include <stdio.h>
#include <stdlib.h>
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
	printf("\33[?25l");
	printf("\33[34m");
	system("clear");
	
	if(argc == 1) return;
	else if(argc == 2) rainSymbol = argv[1][0];
	else exit(-1);
}

int main(int argc, char *argv[])
{
	init(argc, argv);
	rain();

	return 0;
}
