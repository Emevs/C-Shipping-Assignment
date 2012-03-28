#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function to update the location of a single ship. Maths provided in 
 * assignment specification.
 * 
 * @param current_location. Current ship location
 * @param CG. Current ship course 
 * @param K. Current ship speed
 * @param time_elapsed. Time that has passed since start of simulation in minutes.
 * @return updated location
 */
location update_location(location current_location, double CG, double K, double time_step)
{
    location new_location;
    double CGR;
    double LTSR;
    /*
     * convert course and latitude to radians, to update the location.
     */
    CGR = (CG * M_PI) / 180;
    LTSR = (current_location.lat * M_PI) / 180;
    new_location.lat = current_location.lat + (((K * cos(CGR)) * time_step) / 3600.0);
    new_location.lng = current_location.lng + ((((K * sin(CGR)) * time_step) / (cos(LTSR)) / 3600.0));

    return new_location;
}

/**
 * Function to iterate through the whole linked list and update the location of
 * each ship, by calling the update_location function above.
 * 
 * @param head_ptr. pointer to head pointer of linked list
 * @param time_elapsed. time passes since the start of he simulation
 * @param logfile. Pointer to the logfile
 * @param time. Current time in real format
 */
void update_all_locations(ship_node **head_ptr, double time_step, FILE *logfile, time_of_day time)
{
    ship_node *current_ship = *head_ptr;
    /*
     * Loops through whole linked list and updates location for a single ship.
     */
    while(current_ship != NULL)
    {
        location new_location = update_location(current_ship->current_location,
                current_ship->course,
                current_ship->speed, time_step);
        current_ship->current_location = new_location;
        /*
         * Calls function inside_shipping_area to make sure the ship is still 
         * inside the area and needs to be tracked.
         */
        inside_shipping_area(current_ship, head_ptr, logfile, time);
        current_ship = current_ship->next;
    }
}

/**
 * Function to iterate through all the ships in the linked list, comparing them
 * to all the other ships to detect if there is a collision or a risk of 
 * collision.
 * 
 * @param head_ptr. Pointer to head pointer of linked list.
 * @param logfile. Pointer to logfile
 * @param time. Real time
 * @return 
 */
void detect_collision(ship_node **head_ptr, FILE *logfile, time_of_day time)
{
    ship_node *current_ship = *head_ptr;
    /*
     * Loop through whole linked list to compare the ships with each other. 
     */
    while(current_ship != NULL)
    {
        ship_node *next_ship = current_ship->next;
        /*
         * Loop through linked list again, but starting from the ship after 
         * the current ship.
         */
        while(next_ship != NULL)
        {
            /*
             * Called the supplied function, great_circle_distance.
             */
            double distance = great_circle(current_ship->current_location,
                    next_ship->current_location);
            /*
             * If the distance between teh ships is less than 0.25 of a mile
             * then the ships have collided. Print information to the screen
             * and update the logfile.
             */
            if(distance < 0.25)
            {
                printf("At time %d:%d:%d, ship %s and ship %s collided.\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        current_ship->AIS_ID,
                        next_ship->AIS_ID);
                fprintf(logfile, "At time %d:%d:%d, ship %s and ship %s collided.\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        current_ship->AIS_ID,
                        next_ship->AIS_ID);
                /*
                 * Call the remove_ship function in Ships.c as ships that have collided no 
                 * longer need to be tracked.
                 */
                remove_ship(head_ptr, current_ship);
                remove_ship(head_ptr, next_ship);
                /*
                 * Break out of inner loop to continue looking through the linked list.
                 */
                break;
            }
                /*
                 * If the distance between the ships is less than two miles, there 
                 * is a risk of collision so print information to the screen
                 * and update the logfile. 
                 */
            else if(distance < 2)
            {
                printf("At time %d:%d:%d ship %s and ship %s are at risk of colliding.\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        current_ship->AIS_ID,
                        next_ship->AIS_ID);
                fprintf(logfile, "At time %d:%d:%d ship %s and ship %s are at risk of colliding.\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        current_ship->AIS_ID,
                        next_ship->AIS_ID);
            }
            next_ship = next_ship->next;
        }
        current_ship = current_ship->next;
    }
}

/**
 * Function to check if the ship is still in the area. 
 * 
 * @param ship. Pointer to the current ship.
 * @param head_ptr. Pointer to the head pointer of the linked list
 * @param logfile. Pointer to the logfile
 * @param time. Time in real format
 */
void inside_shipping_area(ship_node *ship, ship_node **head_ptr, FILE *logfile, time_of_day time)
{
    if(((ship->current_location.lat > 51.667) && (ship->current_location.lat < 52.833)) &&
            ((ship->current_location.lng > -6.667) && (ship->current_location.lng < -3.833)))
    {
        /*
         * if the ship is in the area but the flag keep_track was not 1 change as the 
         * ship has entered the area.
         */
        if(ship->keep_track != 1)
        {
            ship->keep_track = 1;
            printf("At time %d:%d:%d ship %s sailed into the area\n",
                    time.hours,
                    time.minutes,
                    time.seconds,
                    ship->AIS_ID);
            fprintf(logfile, "At time %d:%d:%d ship %s sailed into the area\n",
                    time.hours,
                    time.minutes,
                    time.seconds,
                    ship->AIS_ID);
        }
    }
    else
    {
        /*
         * If the ship is above the latitude coordinate of 52.833 and has a course
         * that is between 90 and 270 then keep_track 
         * becomes 2 as the ship may come into the area at some point.
         */
        if(((ship->course > 90) && (ship->course < 270)) && (ship->current_location.lat > 52.833))
        {
            ship->keep_track = 2;
        }
            /*
             * If the ship is lower than 51.667 but is on a course that is greater
             * than 270 or less than 90 keep_track becomes 2 as it might come into 
             * the area.
             */
        else if(((ship->course > 270) || (ship->course < 90)) && (ship->current_location.lat < 51.667))
        {
            ship->keep_track = 2;
        }
        else
        {
            /*
             * If the ship is not inside the area, but was previously,
             * the ship has sailed out of the area and is removed, as it no
             * longer needs to be tracked. 
             */
            if(ship->keep_track == 1)
            {
                printf("At time %d:%d:%d ship %s sailed out of the area\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        ship->AIS_ID);
                fprintf(logfile, "At time %d:%d:%d ship %s sailed out of the area\n",
                        time.hours,
                        time.minutes,
                        time.seconds,
                        ship->AIS_ID);
                ship->keep_track = 0;
                remove_ship(head_ptr, ship);
            }
        }
    }
}

/**
 * Function to check the location of the ship when they are first added to the 
 * linked list.
 * 
 * @param ship. Pointer to current ship
 * @param head_ptr. Pointer to head pointer of linked list
 * @param logfile. Pointer to logfile
 * @param time. Time in real format
 */
void start_location(ship_node *ship, ship_node **head_ptr, FILE *logfile, time_of_day time)
{
    /*
     * If the ship is in the area the keep_track flag is set to 1.
     */
    if(((ship->current_location.lat > 51.667) && (ship->current_location.lat < 52.833)) &&
            ((ship->current_location.lng > -6.667) && (ship->current_location.lng < -3.833)))
    {
        ship->keep_track = 1;
    }
        /*
         *If the ship is outside the area but above the latitude coordinate of 52.833 and has a course
         * that is between 90 and 270 then keep_track 
         * becomes 2 as the ship may come into the area at some point.
         */
    else if(((ship->course > 90) && (ship->course < 270)) && (ship->current_location.lat > 52.833))
    {
        ship->keep_track = 2;
    }
        /*
         *If the ship is lower than 51.667 but is on a course that is greater
         * than 270 or less than 90 keep_track becomes 2 as it might come into 
         * the area.
         */
    else if(((ship->course > 270) || (ship->course < 90)) && (ship->current_location.lat < 51.667))
    {
        ship->keep_track = 2;
    }
    else
    {
        ship->keep_track = 0;
    }
}
