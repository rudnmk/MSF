#include "Calculation.h"


void calculation(double time) {
	double E_anomaly;
	double THETA_anomaly;
	double radial_velocity;
	double transversal_velocity;
	double velocity;
	std::vector<double> AGECS_coords = { 0.0, 0.0, 0.0 };
	std::vector<double> GCS_coords = { 0.0, 0.0, 0.0 };
	std::vector<double> geodetic_coords = { 0.0, 0.0, 0.0 };

	std::ofstream data_input;
	data_input.open("C:/Users/mk170/MSF/2/code/lab 2/lab 2 graph/DATA.txt", std::ofstream::app);

	//anomaly calculations
	std::pair <double, double> anomaly_calculation_result = calculate_anomalies();
	E_anomaly = anomaly_calculation_result.first;
	THETA_anomaly = anomaly_calculation_result.second;

	//velocities calculation
	std::pair <double, double> velocity_calculation_result = calculate_velocities(THETA_anomaly);
	radial_velocity = velocity_calculation_result.first;
	transversal_velocity = velocity_calculation_result.second;
	velocity = sqrt(pow(radial_velocity, 2.0) + pow(transversal_velocity, 2.0));

	//calculating spacecraft's position in different coordinate systems
	AGECS_coords = calculate_AGECS(E_anomaly, THETA_anomaly);
	//--------------from here on the calculation is time-based-------------------//
	GCS_coords = calculate_GCS(AGECS_coords, time);
	geodetic_coords = calculate_geodetic_coords(GCS_coords);

	//calculating density and acceleration
	calculate_density_and_acceleration(geodetic_coords[2], radial_velocity, transversal_velocity, velocity);

	//OUTPUT
	data_input << "---------------------TIME: " << time << " ---------------------" << std::endl;
	data_input << "Semi - major axis(SMA): " << SMA << "; Eccentricity(ECC) : " << ECC << "; Focal param: " << P << "; E: " << E_anomaly << "; THETA: " << THETA_anomaly << std::endl;
	data_input << "Radial velocity: " << radial_velocity << "; Transversal velocity: " << transversal_velocity << "; Velocity: " << velocity << std::endl;
	data_input << "AGECS coordinates: [" << AGECS_coords[0] << ", " << AGECS_coords[1] << ", " << AGECS_coords[2] << "]" << std::endl;
	data_input << "GCS coordinates: [" << GCS_coords[0] << ", " << GCS_coords[1] << ", " << GCS_coords[2] << "]" << std::endl;
	data_input << std::setprecision(20) <<"Geodetic coordinates: [" << geodetic_coords[0] << ", " << AGECS_coords[1] << ", " << geodetic_coords[2] << "]" << std::endl;

	data_input.close();
 }




std::pair<double, double> calculate_anomalies() {
	double E_anomaly_past = M;
	double E_anomaly = M + ECC * sin(E_anomaly_past);


	while (fabs(E_anomaly - E_anomaly_past) > accuracy) {
		E_anomaly_past = E_anomaly;
		E_anomaly = M + ECC * sin(E_anomaly_past);
	}

	double THETA_anomaly = 2 * atan(sqrt((1.0 + ECC) / (1.0 - ECC)) * tan(E_anomaly / 2.0));

	return std::make_pair(E_anomaly, THETA_anomaly);
}

std::pair <double, double> calculate_velocities(double THETA) {
	double radial_velocity = sqrt(grav_param / P) * ECC * sin(THETA);
	double transversal_velocity = sqrt(grav_param / P) * (1 + ECC * cos(THETA));
	return std::make_pair(radial_velocity, transversal_velocity);
}

std::vector<double> calculate_AGECS(double E, double THETA) {
	std::vector<double> AGECS_coords = { 0.0, 0.0, 0.0 };
	double AGECS_radius_vector = SMA * (1 - ECC * cos(E));
	double u = THETA + omega;

	AGECS_coords[0] = AGECS_radius_vector * (cos(u) * cos(Omega) - sin(u) * sin(Omega) * cos(i));
	AGECS_coords[1] = AGECS_radius_vector * (cos(u) * sin(Omega) + sin(u) * cos(Omega) * cos(i));
	AGECS_coords[2] = AGECS_radius_vector * sin(u) * sin(i);

	return AGECS_coords;
}

std::vector<double> calculate_GCS(std::vector<double> AGECS_coords, double time) {
	double ang_rotation = angular_vel * time;
	std::vector<double> GCS_coords = { 0.0, 0.0, 0.0 };

	GCS_coords[0] = cos(ang_rotation) * AGECS_coords[0] + sin(ang_rotation) * AGECS_coords[1];
	GCS_coords[1] = cos(ang_rotation) * AGECS_coords[1] - sin(ang_rotation) * AGECS_coords[0];
	GCS_coords[2] = AGECS_coords[2];

	return GCS_coords;
}

std::vector<double> calculate_geodetic_coords(std::vector<double> GCS_coords) {
	std::vector<double> geodetic_coords = { 0.0, 0.0, 0.0 };

	double a = 6378.136;
	double e = 0.0067385254;
	double x = GCS_coords[0];
	double y = GCS_coords[1];
	double z = GCS_coords[2];
	double D = sqrt(pow(x, 2.0) + pow(y, 2.0));
	if (D == 0) {
		geodetic_coords[0] = 0;
		geodetic_coords[1] = (PI / 2.0) * (z / fabs(z));
		geodetic_coords[2] = z * sin(geodetic_coords[1]) - a * sqrt(1 - e * pow(sin(geodetic_coords[1]), 2.0));
		return geodetic_coords;
	}

	else {
		double L = asin(y / D);
		if (y < 0 && x > 0) {
			geodetic_coords[0] = 2 * PI - L;
		}
		else if (y < 0 && x < 0) {
			geodetic_coords[0] = PI + L;
		}
		else if (y > 0 && x < 0) {
			geodetic_coords[0] = PI - L;
		}
		else {
			geodetic_coords[0] = L;
		}

		if (z == 0) {
			geodetic_coords[1] = 0;
			geodetic_coords[2] = D - a;
			return geodetic_coords;
		}
		else {
			double r = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
			double c = asin(z / r);
			double p = (e * a) / (2 * r);
			double s1 = 0;
			double b = c + s1;
			double s2 = asin((p * sin(2 * b)) / (sqrt(1 - e * pow(sin(b), 2.0))));
			while (fabs(s2 - s1) >= ((0.0001 / 3600.0) * PI / 180.0)){
				s1 = s2;
				b = c + s1;
				s2 = asin((p * sin(2 * b)) / (sqrt(1 - e * pow(sin(b), 2.0))));
			}
			geodetic_coords[1] = b;
			geodetic_coords[2] = D * cos(b) + z * sin(b) - a * sqrt(1 - e * pow(sin(b), 2.0));
			return geodetic_coords;
		}
	}

}


void calculate_density_and_acceleration(double H, double radial_vel, double transversal_vel, double vel) {
	std::ofstream data_input;
	double density;
	double S_acc;
	double T_acc;
	double W_acc = 0;
	double acc;
	double g = (6.67430 * 5.9726 * pow(10.0, 7.0)) / pow(6378.1 + H, 2.0);
	bool lower_than_500 = 0;

	double a_param_0_120[7] = { 26.8629, 27.4598, 28.6395, 29.6418, 30.1671, 29.7578, 30.7854 };
	double a_param_1_120[7] = { -0.451674, -0.463668, -0.490987, -0.514957, -0.527837, -0.517915, -0.545695 };
	double a_param_2_120[7] = { 0.00290397, 0.002974, 0.00320649, 0.00341926, 0.00353211, 0.00342699, 0.00370328 };
	double a_param_3_120[7] = { -1.06953 * pow(10.0, -5.0), -1.0753 * pow(10.0, -5.0), -1.1681 * pow(10.0, -5.0), -1.25785 * pow(10.0, -5.0), -1.30227 * pow(10.0, -5.0), -1.24137 * pow(10.0, -5.0), -1.37072 * pow(10.0, -5.0) };
	double a_param_4_120[7] = { 2.21598 * pow(10.0, -8.0), 2.17059 * pow(10.0, -8.0), 2.36847 * pow(10.0, -8.0), 2.5727 * pow(10.0, -8.0), 2.66455 * pow(10.0, -8.0), 2.48209 * pow(10.0, -8.0), 2.80614 * pow(10.0, -8.0) };
	double a_param_5_120[7] = { -2.42941 * pow(10.0, -11.0), -2.30249 * pow(10.0, -11.0), -2.51809 * pow(10.0, -11.0), -2.75874 * pow(10.0, -11.0), -2.85432 * pow(10.0, -11.0), -2.58413 * pow(10.0, -11.0), -3.00184 * pow(10.0, -11.0) };
	double a_param_6_120[7] = { 1.09926 * pow(10.0, -14.0), 1.00123 * pow(10.0, -14.0), 1.09536 * pow(10.0, -14.0), 1.21091 * pow(10.0, -14.0), 1.25009 * pow(10.0, -14.0), 1.09383 * pow(10.0, -14.0), 1.31142 * pow(10.0, -14.0) };

	double a_param_0_500[7] = { 17.8481, -2.54909, -13.9599, -23.3079, -14.7264, -4.912, -5.40952 };
	double a_param_1_500[7] = { -0.132025, 0.0140064, 0.0844951, 0.135141, 0.0713256, 0.0108326, 0.00550749 };
	double a_param_2_500[7] = { 0.000227717, -0.00016946, -0.000328875, -0.000420802, -0.000228015, -8.10546 * pow(10.0, -5.0), -3.78851 * pow(10.0, -5.0) };
	double a_param_3_500[7] = { -2.2543 * pow(10.0, -7.0), 3.27196 * pow(10.0, -7.0), 5.05918 * pow(10.0, -7.0), 5.73717 * pow(10.0, -7.0), 2.8487 * pow(10.0, -7.0), 1.15712 * pow(10.0, -7.0), 2.4808 * pow(10.0, -8.0) };
	double a_param_4_500[7] = { 1.33574 * pow(10.0, -10.0), -2.8763 * pow(10.0, -10.0), -3.92299 * pow(10.0, -10.0), -4.03238 * pow(10.0, -10.0), -1.74383 * pow(10.0, -10.0), -8.13296 * pow(10.0, -11.0), 4.92183 * pow(10.0, -12.0) };
	double a_param_5_500[7] = { -4.50458 * pow(10.0, -14.0), 1.22625 * pow(10.0, -13.0), 1.52279 * pow(10.0, -13.0), 1.42846 * pow(10.0, -13.0), 5.08071 * pow(10.0, -14.0), 3.04913 * pow(10.0, -14.0), -8.65011 * pow(10.0, -15.0) };
	double a_param_6_500[7] = { 6.72086 * pow(10.0, -18.0), -2.05736 * pow(10.0, -17.0), -2.35576 * pow(10.0, -17.0), -2.01726 * pow(10.0, -17.0), -5.34955 * pow(10.0, -18.0), -4.94989 * pow(10.0, -18.0), 1.9849 * pow(10.0, -18.0) };


	if (H < 500) {
		lower_than_500 = 1;
	}

	data_input.open("C:/Users/mk170/MSF/2/code/lab 2/lab 2 graph/acceleration_data.txt");
	for (int step = 0; step < 7; step++) {
		if (lower_than_500) {
			density = night_density * exp(a_param_0_120[step] + a_param_1_120[step] * H + a_param_2_120[step] * pow(H, 2.0) + a_param_3_120[step] * pow(H, 3.0) + a_param_4_120[step] * pow(H, 4.0) + a_param_5_120[step] * pow(H, 5.0) + a_param_6_120[step] * pow(H, 6.0));
		}
		else {
			density = night_density * exp(a_param_0_500[step] + a_param_1_500[step] * H + a_param_2_500[step] * pow(H, 2.0) + a_param_3_500[step] * pow(H, 3.0) + a_param_4_500[step] * pow(H, 4.0) + a_param_5_500[step] * pow(H, 5.0) + a_param_6_500[step] * pow(H, 6.0));
		}
		S_acc = (-1.0) * SIGMA * density * vel * radial_vel * 1000.0;
		T_acc = (-1.0) * SIGMA * density * vel * transversal_vel * 1000.0;
		acc = sqrt(pow(S_acc, 2.0) + pow(T_acc, 2.0));
		data_input << S_acc << " " << T_acc << " " << W_acc << " " << acc << std::endl;
		std::cout << "  g = " << g << "; overall perturbing acceleration = " << acc << std::endl;
	}
	data_input << lower_than_500 << std::endl;
	data_input.close();
}