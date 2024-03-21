#pragma once
#include <iostream>
#include <math.h>
#include <vector>


#define PI 3.1415926
#define apogee_radius 7228.1 //apogee height(850 km) + Earth radius(6378.1 km)
#define perigee_radius 6728.1 //perigee height(350 km) + Earth radius
#define i 45 * PI / 180. //inclanation
#define Omega 20 * PI / 180.0 // ascending node longitude
#define omega 0 // pericenter argument
#define M 15 * PI / 180.0 // average anomaly
#define accuracy 0.001 * PI / 180.0

#define SMA ((apogee_radius + perigee_radius) / 2) // semi-major axis
#define ECC ((apogee_radius - perigee_radius) / (apogee_radius + perigee_radius)) //eccentricity
#define P ((apogee_radius - perigee_radius) / (2 * SMA)) // focal parameter
#define grav_param 398600.4415 //Gravitational parameter of the Earth
#define angular_vel 7.2921158553 * pow(10.0, -5.0) //Angular velocity of the Earth
#define night_density 1.58868 * pow(10.0, -8.0) //night density of the atmosphere on the height of 120 km



std::pair<float, float> calculate_anomalies();
std::pair<float, float> calculate_velocities(float THETA);

std::vector<float> calculate_AGECS(float E, float THETA);
std::vector<float> calculate_GCS(std::vector<float> AGECS_coords, float time);
std::vector<float> calculate_geodetic_coords(std::vector<float> GCS_coords);

float*calculate_density(float H);

void calculation();