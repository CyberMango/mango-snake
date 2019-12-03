/*
Author: Guy Eldar
Created: 1.9.19
Purpose: Internal header for the snake_board module.
*/

#ifndef __SNAKE_BOARD_INTERNAL_H__
#define __SNAKE_BOARD_INTERNAL_H__

/*** Headers ***/
#include <iostream>
#include <stdint.h>
using namespace std;

#include "snake_board.h"

/*** Constants ***/
/* Starting values for the snake board. */
#define STARTING_X (4)
#define STARTING_Y (2)
#define STARTING_DIRECTION (SNAKE_BOARD_DIRECTION_RIGHT)
#define STARTING_SNAKE_LENGTH (5)

#endif /* __SNAKE_BOARD_INTERNAL_H__ */