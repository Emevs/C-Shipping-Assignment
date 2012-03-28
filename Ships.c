#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function takes a character array (a line read in from the file) and splits it,
 * creating a ship out of the extracted information. 
 * 
 * @param AIS_ID. Ships ID/name
 * @param ship_location. Ships current location. 
 * @param course. Ships current course. 
 * @param speed. Ships current speed. 
 * @param date. Date at top of file
 * @return pointer to ship node.
 */
ship_node *create_ship_node(char *AIS_ID, location ship_location, double course, double speed, date_of_file date)//pass in details for ship_node
{
    /*
     * Uses malloc to allocate memory for the new ship.
     */
    ship_node *ship;
    ship = malloc(sizeof (ship_node));

    /*
     * Allocates the parameters to the ship structs variables to create a ship.
     */
    strcpy(ship->AIS_ID, AIS_ID);
    ship->current_location = ship_location;
    ship->course = course;
    ship->speed = speed;
    ship->run_date = date;
    ship->keep_track = 0;
    ship->next = NULL;

    return ship;
}

/**
 * Function to add a new ship to the head of a linked list.
 * 
 * @param head_ptr. Pointer to head pointer in linked list.
 * @param new_ship. Pointer to new ship.
 * @param logfile. Pointer to logfile
 */
void insert_ship(ship_node **head_ptr, ship_node *new_ship, FILE *logfile)
{    
    new_ship->next = *head_ptr;
    *head_ptr = new_ship;
    start_location(new_ship, head_ptr, logfile, new_ship->run_date.file_time);
}

/**
 * Function to remove a ship from the linked list. USed when ships have 
 * sunk after a collision and when they are outside of the area and not 
 * going to sail in.
 * 
 * @param head_ptr. Pointer to head pointer in linked list.
 * @param ship. Pointer to ship.
 */
void remove_ship(ship_node **head_ptr, ship_node *ship)
{
    ship_node *current_ship = *head_ptr;
    ship_node *previous_ship = NULL;

    /*
     * Loops through linked list. 
     */
    while (current_ship != NULL)
    {
        /*
         * Compares the AIS_ID's of the current ship and the passed in 
         * ship to ensure the correct ship is removed. 
         */
        if ((current_ship->AIS_ID) == (ship->AIS_ID))
        {
            if (previous_ship == NULL)
            {
                *head_ptr = current_ship->next;
            } else
            {
                previous_ship->next = current_ship->next;
            }
            free(current_ship);
        }
        previous_ship = current_ship;
        current_ship = current_ship->next;
    }
}

