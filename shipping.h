/* 
 * File:   shipping.h
 * Author: eme3
 *
 * Created on 14 December 2011, 14:25
 */

#ifndef SHIPPING_H
#define	SHIPPING_H
/*
 * Structure to store time in 'real' time.
 */
typedef struct
{
    int hours;
    int minutes;
    int seconds;
} time_of_day;

/*
 * Structure to store the file date and contains a time_of_day struct.
 */
typedef struct
{
    int day;
    int month;
    int year;
    time_of_day file_time;
} date_of_file;

/*
 * Structure is used to store the ship information.
 * Information such as AIS ID, current location, course, speed, date read in
 * from the file, a flag as to whether the ship is in the area and a pointer to 
 * the next ship node.
 */
typedef struct ship_info
{
    char AIS_ID[61];
    location current_location;
    double course;
    double speed;
    date_of_file run_date;
    int keep_track; //0 if out 1 if in
    struct ship_info *next;
} ship_node;

/*
 * Function declarations, see functions for information.
 */

/*
 * Functions from Files.c
 */
int read_file(char *file_name, ship_node **head_ptr, FILE *logfile);
FILE *create_logfile(char *file_name);

/*
 * Functions from Locations.c
 */
location update_location(location current_location, double CG, double K, double time_step);
void update_all_locations(ship_node **head, double time_step, FILE *logfile, time_of_day time);
void detect_collision(ship_node **head, FILE *logfile, time_of_day time); 
void inside_shipping_area(ship_node *ship, ship_node **head_ptr, FILE *logfile, time_of_day time);
void start_location(ship_node *ship, ship_node **head_ptr, FILE *logfile, time_of_day time);

/*
 * Functions from Ships.c
 */
ship_node *create_ship_node(char AIS_ID[61], 
                            location ship_location, 
                            double course, 
                            double speed, 
                            date_of_file date);
void insert_ship(ship_node **head_ptr, ship_node *new_ship, FILE *logfile);
void remove_ship(ship_node **head_ptr, ship_node *ship);

/*
 * Functions from Print.c
 */
void print_all(ship_node **head_ptr);
void print_ship(ship_node *current_ship);
void all_to_logfile(ship_node **head_ptr, FILE *logfile);
void log_ship(ship_node *current_ship, FILE *logfile);

/*
 * Functions from Simulation.c
 */
void simulation(ship_node **head_ptr, FILE *logfile);

/*
 * Functions from Time_converter.c
 */
int to_real_seconds(double time_elapsed);
time_of_day from_seconds(int seconds);
double elapsed_to_mins(double time_elapsed);
time_of_day current_time(time_of_day file_time, double time_elapsed);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SHIPPING_H */

