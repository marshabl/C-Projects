#include "board.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/**
 * In this file, implement all of the functions for board.h
 *
 * Once done, make sure you pass all the tests in board_test.c
 * (make test_board)
 *
 */

struct board_t
{
	unsigned int height;
	unsigned int width;
	unsigned int run;
	player_t * position;	
};

bool board_create (board_t ** b, unsigned int height, unsigned int width, unsigned int run, const player_t * initial)
{	
	*b = malloc(sizeof(**b));

	(*b)->position = malloc(height*width*sizeof(*initial));
	(*b)->height = height;
        (*b)->width = width;
        (*b)->run = run;

	if (initial != NULL)
	{
		for (unsigned int i=0; i<(height*width); ++i)
		{
			(*b)->position[i] = initial[i];
		}
		//memcpy((*b)->position, initial, height*width*sizeof(*initial));
	}
	
	else
	{
		//memset((*b)->position, PLAYER_EMPTY, height*width*sizeof(*initial));
		for (unsigned int i=0; i<(height*width); ++i)
                {
                        (*b)->position[i] = PLAYER_EMPTY;
                }
	}
	
	return 1;
}

bool board_destroy (board_t * b)
{
	free(b->position);
	free(b);
	return 1;
}

unsigned int board_get_height (const board_t * b)
{
	return b->height;
}

unsigned int board_get_width (const board_t * b)
{
	return b->width;
}


unsigned int board_get_run (const board_t * b)
{
	return b->run;
}

bool board_play (board_t * b, unsigned int column, player_t player)
{
	unsigned int height = b->height;
	unsigned int width = b->width;
	
       	for (unsigned int i=0; i<height; ++i)
       	{       
               	if (b->position[i*width + column] == PLAYER_EMPTY)
               	{
			b->position[i*width + column] = player;
                        return 1;
               	}
       	}
	
	return 0;
}

bool board_can_play (const board_t * b, player_t p)
{
	unsigned int width = b->width;
	unsigned int height = b->height;
	
	for (unsigned int i=0; i<(width*height); ++i)	
	{
		if (b->position[i] == PLAYER_EMPTY)
		{
			return 1;
		}
	}
	return 0;
}

bool board_can_play_move (const board_t * b, player_t p, unsigned int column)
{
	unsigned int width = b->width;
	unsigned int height = b->height;

        for (unsigned int i=0; i<height; ++i)
        {
        	if (b->position[i*width + column] == PLAYER_EMPTY)
                {
                                return 1;
                }
        }
	
	return 0;
}

bool board_get (const board_t * b, unsigned int row, unsigned int column, player_t * piece)
{
	if (row+1>b->height || column+1>b->width)
	{
		return 0;
	}
	else
	{
		*piece = b->position[row*(b->width) + column];
		return 1;		
	}
}

bool board_unplay (board_t * b, unsigned int column)
{
	unsigned int height = b->height;
	unsigned int width = b->width;
	
	unsigned int count = 0;
	
	for (unsigned int i=0; i<height; ++i)
        {
                if (b->position[i*width + column] == PLAYER_EMPTY)
                {
				++count;
                }
        }

	if (count == height)
	{
		return 0;
	}
	else
	{
		board_play(b, column, PLAYER_EMPTY);
		return 1;
	}
}

bool board_duplicate (board_t ** newboard, const board_t * old)
{
	unsigned int height = old->height;
	unsigned int width = old->width;
	unsigned int run = old->run;
	player_t * position = old->position;
	board_create(newboard, height, width, run, position);
	return 1;
}

bool checkHor (const board_t * b, player_t * player)
{
	unsigned int row = board_get_height(b);
	unsigned int column = board_get_width(b);
	unsigned int run = board_get_run(b);

	int bcount = 0;
	int ycount = 0;

	for (int i=0; i<row; ++i)
	{
		for (int j=0; j<column; ++j)
		{
			if (b->position[i*column + j] == PLAYER_BLUE)
			{
				ycount = 0;
				++bcount;
				if (bcount == run)
				{
					*player = PLAYER_BLUE;
					return 1;
				}
			}
			else if (b->position[i*column + j] == PLAYER_YELLOW)
                        {       
				bcount = 0;
                                ++ycount;
                                if (ycount == run)
                                {
					*player = PLAYER_YELLOW;       
                                        return 1;
                                }
                        }
			else
			{
				bcount = 0;
                		ycount = 0;
			}
		}
		bcount = 0;
		ycount = 0;
	}
	return 0;
}

bool checkVer (const board_t * b, player_t * player)
{
        unsigned int row = board_get_height(b);
        unsigned int column = board_get_width(b);
        unsigned int run = board_get_run(b);

        int bcount = 0;
	int ycount = 0;
        for (int i=0; i<column; ++i)
        {
                for (int j=0; j<row; ++j)
                {
                        if (b->position[i + j*column] == PLAYER_BLUE)
                        {
				ycount = 0;
                                ++bcount;
                                if (bcount == run)
                                {
					*player = PLAYER_BLUE;
                                        return *player;
                                }
                        }
			else if (b->position[i + j*column] == PLAYER_YELLOW)
                        {
				bcount = 0;
                                ++ycount;
                                if (ycount == run)
                                {
					*player = PLAYER_YELLOW;
                                        return *player;
                                }
                        }
			else
			{
				ycount = 0;
                		bcount = 0;
			}
                }
		ycount = 0;
		bcount = 0;
        }
        return 0;
}

bool checkDia (const board_t * b, player_t * player)
{
	unsigned int run = board_get_run(b);
	unsigned int height = board_get_height(b);
	unsigned int width = board_get_width(b);
	unsigned char * init = b->position;
	unsigned int bcount = 1;
	unsigned int ycount = 1;

	for (unsigned int i=0; i<height*width; ++i)
	{
		int crow = i/width;
		int ccol = i % width;

		if (height - crow >= run && width - ccol >= run)
		{
			for (int j=0; width-ccol-j > 0; ++j)
			{
				if ((crow+1+j)*width+ccol+1+j < height*width)
				{
					if (init[i]==PLAYER_BLUE && init[(crow+1+j)*width+ccol+1+j] == PLAYER_BLUE)
                                	{
                                        	ycount = 1;
                                        	++bcount;
                                        	if (bcount == run)
                                        	{
                                                	*player = PLAYER_BLUE;
                                                	return *player;
                                        	}
                                	}
                                	else if (init[i]==PLAYER_YELLOW && init[(crow+1+j)*width+ccol+1+j] == PLAYER_YELLOW)
                                	{
                                        	bcount = 1;
                                        	++ycount;
                                        	if (ycount == run)
                                        	{
                                                	*player = PLAYER_YELLOW;
                                                	return *player;
                                        	}
                                	}
                                	else
                                	{
                                        	--ycount;
                                        	--bcount;
                                	}

				}
			}	
			ycount = 1;
			bcount = 1;
		}
		if (height - crow >= run && ccol+1 >= run)
		{
			int k=1;
			for (int j=ccol; j>0; --j)
			{
				if ((crow+k)*width+j-1<height*width)
				{
					if (init[i]==PLAYER_BLUE && init[(crow+k)*width+j-1] == PLAYER_BLUE)
					{
						ycount = 1;
						++bcount;
						if (bcount == run)
                                        	{
							*player = PLAYER_BLUE;
                                                	return *player;
                                        	}
					}
					else if (init[i]==PLAYER_YELLOW && init[(crow+k)*width+j-1] == PLAYER_YELLOW)
                                	{
						bcount = 1;
                                        	++ycount;
                                        	if (ycount == run)
                                        	{
                                                	*player = PLAYER_YELLOW;
                                                	return *player;
                                        	}
                                	}
					else
					{
						--ycount;
                        			--bcount;
					}
					++k;
				}
			}
			ycount = 1;
			bcount = 1;
		}
	}
	return 0;	
}

int board_has_winner (const board_t * b, player_t * player)
{
	if (checkHor(b, player) || checkVer(b, player) || checkDia(b, player))
	{
		return 1;
	}
	else if (!board_can_play(b, *player))
	{
		return -1;
	}
	else 
	{
		return 0;
	}
}

bool board_clear (board_t * b)
{
	for (unsigned int i=0; i<((b->height)*(b->width)); ++i)
        {       
        	b->position[i] = PLAYER_EMPTY;
        }
	return 1;
}

bool board_print (const board_t * b, FILE * f)
{
	
	int w = b->width;
	int h = b->height;
	unsigned char * p = b->position;
	
	if (f == NULL)
	{
		return 0;
	}
	
	for (int i=0; i<w*h; ++i)
	{
		fprintf(f, "%u", p[i]);
	}
	fputs("\n", f);
	return 1;
}


