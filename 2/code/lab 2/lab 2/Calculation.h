#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <iomanip>


#define PI 3.1415926

//Earth parameters
#define apogee_radius 7228.1 //apogee height(850 km) + Earth radius(6378.1 km)
#define perigee_radius 6728.1 //perigee height(350 km) + Earth radius
#define grav_param 398600.4415 //Gravitational parameter of the Earth
#define angular_vel 7.2921158553 * pow(10.0, -5.0) //Angular velocity of the Earth

#define i 45 * PI / 180.0 //inclanation
#define Omega 20 * PI / 180.0 // ascending node longitude
#define omega 0 // pericenter argument
#define M 15 * PI / 180.0 // average anomaly
#define accuracy 0.001 * PI / 180.0

#define SMA ((apogee_radius + perigee_radius) / 2) // semi-major axis
#define ECC ((apogee_radius - perigee_radius) / (apogee_radius + perigee_radius)) //eccentricity
#define P (SMA * (1 - pow(ECC, 2.0))) // focal parameter


#define night_density 1.58868 * pow(10.0, -8.0) //night density of the atmosphere on the height of 120 km
#define T 2 * PI * sqrt(pow(SMA, 3.0) / grav_param)

#define SIGMA (12.0 * 2.0) / (2.0 * 1500.0)



std::pair<double, double> calculate_anomalies();
std::pair<double, double> calculate_velocities(double THETA);

std::vector<double> calculate_AGECS(double E, double THETA);
std::vector<double> calculate_GCS(std::vector<double> AGECS_coords, double time);
std::vector<double> calculate_geodetic_coords(std::vector<double> GCS_coords);

void calculate_density_and_acceleration(double H, double radial_vel, double transversal_vel, double vel);

void calculation(double time);