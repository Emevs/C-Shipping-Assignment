#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function to read in the chosen file. Reads in information one field at a time
 * and assigns it to a relevant variable.  
 * Returns 0 if a success, 1 if it fails to read the file. 
 * 
 * @param file_name. Name of the file the user wants read in.
 * @param head_ptr. Pointer to the head pointer of the linked list.
 * @param logfile. Pointer to the logfile.
 * @return int, returns 1 if file was read, 0 if not.
 */
int read_file(char *file_name, ship_node **head_ptr, FILE *logfile)
{

    FILE *ship_file;
    //char mode[3] = "r";
    date_of_file date;
    time_of_day time;
    int success;
    // open file
    ship_file = fopen(file_name, "r");
    if(ship_file != NULL)
    {
        /*
         * Reads in first line of file, which is the date and time.
         */
        fscanf(ship_file, "%d", &date.day);
        fscanf(ship_file, "%d", &date.month);
        fscanf(ship_file, "%d", &date.year);
        fscanf(ship_file, "%d", &time.hours);
        fscanf(ship_file, "%d", &time.minutes);
        fscanf(ship_file, "%d", &time.seconds);

        date.file_time = time;
        char AIS_ID[61];

        /*
         * Loop through rest of file until it reaches the end and allocate 
         * ship name.
         */
        while(fscanf(ship_file, "%s", AIS_ID) != EOF)
        {

            location ship_location;
            double course;
            double speed;
            /*
             * Read in locations, course and speed, to create a new ship.
             */
            fscanf(ship_file, "%lf", &ship_location.lat);
            fscanf(ship_file, "%lf", &ship_location.lng);
            fscanf(ship_file, "%lf", &course);
            fscanf(ship_file, "%lf", &speed);

            /*
             * Create a new ship by calling create_ship function in Ships.c and 
             * then insert_ship, to add it the the linked list.
             */
            ship_node *new_ship;
            new_ship = create_ship_node(AIS_ID, ship_location, course, speed, date);
            insert_ship(head_ptr, new_ship, logfile);
        }
        success = 1;
    }
    else
    {
        printf("Sorry, there was an error opening the file.");
        success = 0;
    }

    fclose(ship_file);
    return success;
}

/**
 * Function to create a logfile of all major events (risk of collisions, 
 * collisions, when a ship leaves the area and when a ship enters the area.)
 * 
 * @param file_name. Name of the file the user wants read in, to give logfile
 * relevent name.
 * @return pointer to a file.
 */
FILE *create_logfile(char *file_name)
{
    FILE *logfile;
    char logfile_name[100];
    
    strcat(logfile_name, file_name);
    strcat(logfile_name, "_logfile.txt");
    
    logfile = fopen(logfile_name, "a");

    return logfile;
}

