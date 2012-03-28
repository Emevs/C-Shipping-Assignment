/* 
 * File:   main.c
 * Author: Emily
 *
 * Created on 06 December 2011, 20:55
 */

#include <stdio.h>
#include <stdlib.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Main function to ask user for the file name to read data from, create a 
 * logfile and call the simulate and print all functions.
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv)
{
    ship_node *head;
    head = NULL;
    char file_name[61];
    printf("Please input the file name that contains the ship information: ");
    scanf("%60s", file_name);
    /*
     * Create logfile by calling create_logfile function in Files.c and read 
     * data in from input file. 
     */
    FILE *logfile;
    logfile = create_logfile(file_name);
    int file;
    file = read_file(file_name, &head, logfile);

    if(file)
    {
        /*
         * If file was read in correctly run simulation to update ship locations 
         * then print all remaining ships to the screen and the logfile.
         */
        simulation(&head, logfile);
        print_all(&head);
        all_to_logfile(&head, logfile);
    }
    fclose(logfile);
    return(EXIT_SUCCESS);
}


