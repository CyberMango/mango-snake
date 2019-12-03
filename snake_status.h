/*
Author: Guy Eldar
Created: 1.9.19
Purpose: Return statuses for the snake project.
*/

#ifndef __SNAKE_STATUS_H__
#define __SNAKE_STATUS_H__

/*** Enums ***/
enum snake_status
{
    SNAKE_STATUS_UNINITIALIZED = -1,
    SNAKE_STATUS_SUCCESS = 0,

    SNAKE_STATUS_SNAKE_BOARD_MOVE_A_STEP_INVALID_DIRECTION,
};

#endif /* __SNAKE_STATUS_H__ */