/* 
 * File:   navigation.h
 * Author: dap
 *
 * Created on 11 November 2011, 10:21
 */

#ifndef NAVIGATION_H
#define	NAVIGATION_H

/*
 * This structure is used to represent a location.
 * The two members are doubles representing the latitude and longitude
 * of the location in degrees.
 */
typedef struct
{
    double lat;
    double lng;
} location;

double great_circle(location location_1, location location_2);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* NAVIGATION_H */



