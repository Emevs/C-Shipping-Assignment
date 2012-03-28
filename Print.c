#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function to iterate through all ships in the linked list, and calls the 
 * print_ship function to print the information about each ship. 
 * Takes a pointer to the head of the linked list.
 * 
 * @param head_ptr. Pointer to head pointer of linked list
 */
void print_all(ship_node **head_ptr)
{
    ship_node *current_ship = *head_ptr;
    /*
     * Loops through linked list and calls the print_ship function below to 
     * print the individual ship information/
     */
    while(current_ship != NULL)
    {
        print_ship(current_ship);
        current_ship = current_ship->next;
    }
}

/**
 * Function to print out information about the ship it has been passed.
 * 
 * @param current_ship, pointer to current ship
 */
void print_ship(ship_node *current_ship)
{
    printf("\nAIS ID: %s \n"
            "Latitude: %lf \n"
            "Longitude: %lf \n"
            "Course over ground: %lf \n"
            "Speed: %lf \n",
            current_ship->AIS_ID,
            current_ship->current_location.lat,
            current_ship->current_location.lng,
            current_ship->course,
            current_ship->speed);

}

/**
 * Function to add all remaining ships to the logfile, by going though the 
 * whole linked list.
 * 
 * @param head_ptr. Pointer to head pointer of linked list
 * @param logfile. Pointer to logfile
 */
void all_to_logfile(ship_node **head_ptr, FILE *logfile)
{
    ship_node *current_ship = *head_ptr;
    while(current_ship != NULL)
    {
        log_ship(current_ship, logfile);
        current_ship = current_ship->next;
    }
}

/**
 * Function to print individual ship information to the logfile.
 * 
 * @param current_ship. Poiner to the current ship.
 * @param logfile. Pointer to the logfile.
 */
void log_ship(ship_node *current_ship, FILE *logfile)
{
    fprintf(logfile, "\nAt the end of the simulation, the remaining ships and their locations are:\n"
            "AIS ID: %s \n"
            "Latitude: %lf \n"
            "Longitude: %lf \n"
            "Course over ground: %lf \n"
            "Speed: %lf \n",
            current_ship->AIS_ID,
            current_ship->current_location.lat,
            current_ship->current_location.lng,
            current_ship->course,
            current_ship->speed);
}
