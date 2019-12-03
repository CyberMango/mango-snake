/*
Author: Guy Eldar
Created: 1.9.19
Purpose: A board in a snake game.
*/

#ifndef __SNAKE_BOARD_H__
#define __SNAKE_BOARD_H__

/*** Headers ***/
#include <stdint.h>
#include <iostream>

#include "../snake_status.h"

/*** Constants ***/
// TODO consider turning these into private constants. figure how to define snake::locations in that manner.
#define SNAKE_BOARD_ROWS (20)
#define SNAKE_BOARD_COLUMNS (30)
#define SNAKE_BOARD_SIZE (SNAKE_BOARD_ROWS * SNAKE_BOARD_COLUMNS)

#define SNAKE_BOARD_NO_LOCATION_ROW (UINT8_MAX)
#define SNAKE_BOARD_NO_LOCATION_COLUMN (UINT8_MAX)
#define SNAKE_BOARD_NO_LOCATION (std::pair<uint8_t, uint8_t>(SNAKE_BOARD_NO_LOCATION_ROW,     \
                                                             SNAKE_BOARD_NO_LOCATION_COLUMN))

/*** Enums ***/
enum snake_board_direction
{
    SNAKE_BOARD_DIRECTION_LEFT = 0,
    SNAKE_BOARD_DIRECTION_RIGHT,
    SNAKE_BOARD_DIRECTION_UP,
    SNAKE_BOARD_DIRECTION_DOWN,
};

/*** Classes ***/
/* A game board in a game of snake. */
class snake_board
{
private:
    /* Represents the snake in the snake game */
    class snake
    {
    private:
        /* The direction the snake is currently headed in. */
        enum snake_board_direction direction;
        /* Ordered locations of the body of the snake. */
        std::pair<uint8_t, uint8_t> locations[SNAKE_BOARD_SIZE];
        /* The index of the tail in the locations array. */
        uint16_t tail_index;
        /* The index of the head in the locations array. */
        uint16_t head_index;

        /*
        Purpose    : Move the snake a single step in its direction.
        Parameters : TODO add an output parameter of the updates class.
        Returns    : enum snake_status.
        */
        enum snake_status move_a_step();

        /*
        Purpose:   : Detect whether or not the snake hit a wall.
        Parameters : TODO output of updates.
        Returns    : true if the snake hit a wall, false otherwise.
        */
       bool did_hit_wall();

    public:
        snake();

        /*
        Purpose    : Make a single turn of the snake game.
        Parameters : TODO add an output parameter of the updates class.
        Returns    : enum snake_status.
        */
        enum snake_status make_a_turn();

        /* 
        Purpose    : Change the direction the snake is headed in.
        Parameters : new_direction[IN] - the new desired direction of the snake.
        Returns    : enum snake_status:
                     SNAKE_STATUS_SNAKE_BOARD_CHANGE_DIRECTION_OPPOSITE_DIRECTION - When trying to
                     turn to the opposite of the current direction.
        */
        snake_status change_direction(enum snake_board_direction new_direction)
        {
            this->direction = new_direction;
            // TODO confirm this is not the opposite direction.
            return SNAKE_STATUS_SUCCESS;
        }

        inline uint16_t next_location_index(uint16_t location_index)
        {
            return (location_index + 1) % SNAKE_BOARD_SIZE;
        }
    };

public:

};

class board_updates
{
private:
    /* The location to add to the snake's locations. */
    std::pair<uint8_t, uint8_t> new_location;
    /* The location to remove from the snake's locations. */
    std::pair<uint8_t, uint8_t> erased_location;
    /* Wether or not the snake hit the wall. */
    bool did_hit_wall;

    /* TODO find how to declare this class here but define it elsewhere. */
    /* An abstract iterator that goes through the updated locations container. */
    class abstract_locations_iterator: public std::iterator<
        std::forward_iterator_tag,
        std::pair<uint8_t, uint8_t>,
        std::pair<uint8_t, uint8_t>,
        const std::pair<uint8_t, uint8_t>*
    >
    {
    protected:
        const std::pair<uint8_t, uint8_t>* location;

    public:
        abstract_locations_iterator& operator++()
        {
            ++location;
            return *this;
        }

        abstract_locations_iterator operator++(int)
        {
            abstract_locations_iterator temp_iterator = *this;
            ++location;
            return temp_iterator;
        }

        bool operator<(const abstract_locations_iterator& other_iterator)
        {
            return (location < other_iterator.location);
        }

        bool operator>(const abstract_locations_iterator& other_iterator)
        {
            return (!(*this < other_iterator));
        }

        std::pair<uint8_t, uint8_t> operator*()
        {
            return *location;
        }
    };

public:
    board_updates(bool did_hit_wall, 
                  std::pair<uint8_t, uint8_t> new_location=SNAKE_BOARD_NO_LOCATION,
                  std::pair<uint8_t, uint8_t> erased_location=SNAKE_BOARD_NO_LOCATION)
    {
        this->did_hit_wall = did_hit_wall;
        this->new_location = new_location;
        this->erased_location = erased_location;
    }
    /* Iterates through the new locations of the update. */
    class new_locations_iterator: public abstract_locations_iterator
    {
    public:
        new_locations_iterator(const board_updates &updates)
        {
            location = &(updates.new_location);
        }
    };

    /* Iterates through the erased locations of the update. */
    class erased_locations_iterator: public abstract_locations_iterator
    {
    public:
        erased_locations_iterator(const board_updates &updates)
        {
            location = &(updates.erased_location);
        }
    };

    friend board_updates::new_locations_iterator;
    friend board_updates::new_locations_iterator;

    new_locations_iterator new_locations_begin()
    {
        return new_locations_iterator(*this);
    }

    new_locations_iterator new_locations_end()
    {
        new_locations_iterator end_iterator(*this);

        if (SNAKE_BOARD_NO_LOCATION != new_location)
        {
            ++end_iterator;
        }
        return end_iterator;
    }

    erased_locations_iterator erased_locations_begin()
    {
        return erased_locations_iterator(*this);
    }

    erased_locations_iterator erased_locations_end()
    {
        erased_locations_iterator end_iterator(*this);

        if (SNAKE_BOARD_NO_LOCATION != erased_location)
        {
            ++end_iterator;
        }
        return end_iterator;
    }

    inline bool did_snake_hit_wall()
    {
        return this->did_hit_wall;
    }
};

#endif /* __SNAKE_BOARD_H__ */
