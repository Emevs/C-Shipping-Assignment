#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "navigation.h"
#include "shipping.h"

/**
 * Function to convert time into 'real' time (seconds).
 * 
 * @param time_elapsed. time passed simce start of simulation
 * @return int. Time elapsed in seconds
 */
int to_real_seconds(double time_elapsed)
{
    int time_elapsed_secs;
    time_elapsed_secs = time_elapsed*60;
    return time_elapsed_secs;
}

/**
 * Function to convert seconds into hours, minutes and seconds, for use in 
 * the function below.
 * 
 * @param seconds. Time passed in seconds
 * @return converted time
 */
time_of_day from_seconds(int seconds)
{
    time_of_day time;
    time.minutes = seconds/60;
    time.seconds = seconds%60;
    time.hours = time.minutes/60;
    time.minutes = time.minutes%60;
    
    return time;
}

/**
 * Function to add the elapsed time to the file time for use in the logfile.
 * 
 * @param file_time, time at start of file
 * @param time_elapsed, time passed since start of simulation
 * @return current time
 */
time_of_day current_time(time_of_day file_time, double time_elapsed)
{
    int seconds_elapsed = to_real_seconds(time_elapsed);
    
    time_of_day new_time;
    int seconds;
    seconds = file_time.hours*3600;
    seconds += file_time.minutes*60;
    seconds += file_time.seconds;
    
    seconds += seconds_elapsed;
    
    new_time = from_seconds(seconds);
    return new_time;
}
