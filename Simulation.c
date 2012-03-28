#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function to run simulation. Asks user for time duration and time step and 
 * iterate through the program.
 * 
 * @param head_ptr. Pointer to the head pointer of the linked list.
 * @param logfile. Pointer to the logfile.
 */
void simulation(ship_node **head_ptr, FILE *logfile)
{
    /*
     * Asks user for time duration and a time step to run the program.
     */
    double time_duration;
    printf("Please enter how long you would like the simulation to run in hours in decimal time: ");
    scanf("%lf", &time_duration);
    double time_duration_mins = time_duration *60;
    double time_step;
    printf("Please enter the time step in minutes in decimal time: ");
    scanf("%lf", &time_step);
    
    /*
     * calculate current time for use in logfiles. 
     */
    double time_elapsed = 0;
    time_of_day time;
    
    time = current_time((*head_ptr)->run_date.file_time, time_elapsed);
    
    time_elapsed = time_elapsed + time_step;
    time = current_time(time, time_elapsed);
    /*
     * Loop through, updating ship locations and detecting collisions and risks,
     * incrementing time_elapsed by the time step until the time elapsed is 
     * equal to the time duration in minutes.
     */
    while(time_elapsed != time_duration_mins)
    {
        /*
         * Update time and calculate current time, for use in the logfile,
         * update the ship locations for that time and detect collisions.
         */
        update_all_locations(head_ptr, time_step, logfile, time);
        detect_collision(head_ptr, logfile, time);
        time_elapsed = time_elapsed + time_step;
        time = current_time(time, time_elapsed);
    }
    
}
