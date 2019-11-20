#include "computer_agent.h"
#include "player_agent.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

/*
 * In this file, implement your AI agent.
 *
 * Use the minimax algorithm to determine the next move.
 *
 * Look at player_agent.h and player_agent.c for an example of how to
 * implement each function.
 */

typedef struct
{
   char * name;
} computer_agent_data_t;

static
const char * computer_agent_describe (agent_t * gthis)
{
   computer_agent_data_t * data = (computer_agent_data_t *) gthis->data;
   return data->name;
}

void play_all_moves(board_t * b, player_t p, int start, int max)
{
        unsigned int w=board_get_width(b);
        //unsigned int h=board_get_height(b);

        player_t op;

        for (unsigned int i=start; i<max; ++i)
        {
                //printf("%d\n", i);
                board_get(b, i/w, i%w, &op);
                //printf("%d\n", op);

                if (op!=PLAYER_EMPTY)
                        continue;

                board_play(b, i, p);
                board_print(b, stdout);
                play_all_moves(b, (p == PLAYER_YELLOW ? PLAYER_BLUE : PLAYER_YELLOW), i+1, max);
                printf("%d\n", i);
                board_play(b, i, PLAYER_EMPTY);
        }
}

static
int computer_agent_play (agent_t * gthis, const board_t * b,
        player_t color, unsigned int * move)
{
	srand(time(NULL));

        const unsigned int width = board_get_width(b);

        while(true)
        {
                if (board_can_play(b, color))
                {
                        *move = rand() % width;

                        if (!board_can_play_move(b, color, *move))
                        {
                                continue;
                        }
                        return 0;
                }
                return 0;
        }
}

static
bool computer_agent_destroy (agent_t * this)
{
   free (this->data);
   return true;
}

agent_t * computer_agent_create (const char * name)
{
    // Allocate vtable structure
    agent_t * n = malloc (sizeof(agent_t));

    n->destroy = computer_agent_destroy;
    n->play = computer_agent_play;
    n->describe = computer_agent_describe;

    computer_agent_data_t * data = malloc (sizeof(computer_agent_data_t));
    n->data = data;

    data->name = "Computer";
    return n;
}

