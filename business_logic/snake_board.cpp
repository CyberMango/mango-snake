/*
Author: Guy Eldar
Created: 1.9.19
Purpose: Implementations of functions from snake_board.h .
*/

/*** Headers ***/
#include "snake_board_internal.h"

/*** Functions ***/
snake_board::snake::snake()
{
    this->direction = STARTING_DIRECTION;
    this->tail_index = 0;
    this->head_index = STARTING_SNAKE_LENGTH - 1;
    this->locations[0].first = STARTING_X;
    this->locations[0].second = STARTING_Y;
    
    for (size_t i = 1; i < STARTING_SNAKE_LENGTH; ++i)
    {
        this->locations[i].second = this->locations[i - 1].second;
        this->locations[i].first = this->locations[i - 1].first + 1;
    }
}

enum snake_status snake_board::snake::move_a_step()
{
    enum snake_status status = SNAKE_STATUS_UNINITIALIZED;
    pair<uint8_t, uint8_t> new_head = locations[head_index];

    switch(this->direction)
    {
    case SNAKE_BOARD_DIRECTION_RIGHT:
        ++(new_head.second);
        break;

    case SNAKE_BOARD_DIRECTION_LEFT:
        --(new_head.second);
        break;

    case SNAKE_BOARD_DIRECTION_DOWN:
        ++(new_head.first);
        break;

    case SNAKE_BOARD_DIRECTION_UP:
        --(new_head.first);
        break;

    default:
        status = SNAKE_STATUS_SNAKE_BOARD_MOVE_A_STEP_INVALID_DIRECTION;
        //TODO debug print
        goto l_cleanup;
    }

    head_index = next_location_index(head_index);
    tail_index = next_location_index(tail_index);
    locations[head_index] = new_head;

    status = SNAKE_STATUS_SUCCESS;

l_cleanup:
    return status;
}

inline bool snake_board::snake::did_hit_wall()
{
    if ((locations[head_index].first >= SNAKE_BOARD_ROWS) ||
        (locations[head_index].second >= SNAKE_BOARD_COLUMNS))
    {
        return true;
    }

    return false;
}
