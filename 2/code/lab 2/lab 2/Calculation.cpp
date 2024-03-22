#include "Calculation.h"


void calculation(float time) {
	float E_anomaly;
	float THETA_anomaly;
	float radial_velocity;
	float transversal_velocity;
	float velocity;
	std::vector<float> AGECS_coords = { 0.0, 0.0, 0.0 };
	std::vector<float> GCS_coords = { 0.0, 0.0, 0.0 };
	std::vector<float> geodetic_coords = { 0.0, 0.0, 0.0 };
	int solar_activity[7] = { 75, 100, 125, 150, 175, 200, 250 };
	float* density_120 = new float[7];
	float* density_500 = new float[7];

	float* S_acc_120 = new float[7];
	float* T_acc_120 = new float[7];
	float* acc_120 = new float[7];

	float* S_acc_500 = new float[7];
	float* T_acc_500 = new float[7];
	float* acc_500 = new float[7];

	//anomaly calculations
	std::pair <float, float> anomaly_calculation_result = calculate_anomalies();
	E_anomaly = anomaly_calculation_result.first;
	THETA_anomaly = anomaly_calculation_result.second;

	//velocities calculation
	std::pair <float, float> velocity_calculation_result = calculate_velocities(THETA_anomaly);
	radial_velocity = velocity_calculation_result.first;
	transversal_velocity = velocity_calculation_result.second;
	velocity = sqrt(pow(radial_velocity, 2.0) + pow(transversal_velocity, 2.0));

	//calculating spacecraft's position in different coordinate systems
	AGECS_coords = calculate_AGECS(E_anomaly, THETA_anomaly);
	//--------------from here on the calculation is time-based-------------------
	GCS_coords = calculate_GCS(AGECS_coords, time);
	geodetic_coords = calculate_geodetic_coords(GCS_coords);

	//calculating density and acceleration
	float* dens_ptr = calculate_density(geodetic_coords[2]);
	std::array <std::vector <float>, 14> acc_array = calculate_acceleration(dens_ptr, radial_velocity, transversal_velocity, velocity);

	for (int l = 0; l < 14; l++) {
		if (l < 7) {
			density_120[l] = *(dens_ptr + l);
			S_acc_120[l] = acc_array[l][0];
			T_acc_120[l] = acc_array[l][1];
			acc_120[l] = sqrt(pow(S_acc_120[l], 2.0) + pow(T_acc_120[l], 2.0));
		}
		else {
			density_500[l - 7] = *(dens_ptr + l);
			S_acc_500[l - 7] = acc_array[l][0];
			T_acc_500[l - 7] = acc_array[l][1];
			acc_500[l - 7] = sqrt(pow(S_acc_500[l - 7], 2.0) + pow(T_acc_500[l - 7], 2.0));
		}
	}


	//OUTPUT
	std::cout << "E: " << E_anomaly << "; THETA: " << THETA_anomaly << std::endl;
	std::cout << "Radial velocity: " << radial_velocity << "; Transversal velocity: " << transversal_velocity << "; Velocity: " << velocity << std::endl;
	std::cout << "AGECS coordinates: [" << AGECS_coords[0] << ", " << AGECS_coords[1] << ", " << AGECS_coords[2] << "]" << std::endl;
	std::cout << "TIME: " << time << std::endl;
	std::cout << "GCS coordinates: [" << GCS_coords[0] << ", " << GCS_coords[1] << ", " << GCS_coords[2] << "]" << std::endl;
	std::cout << "Geodetic coordinates: [" << geodetic_coords[0] << ", " << AGECS_coords[1] << ", " << geodetic_coords[2] << "]" << std::endl;
	std::cout << "------Density with fixated height = 120 km------" << std::endl;
	for (int l = 0; l < 7; l++) {
		std::cout << "Solar activity: " << solar_activity[l] << "; Density: " << density_120[l] << "; S-related acceleration: " << S_acc_120[l] << "; T_related acceleration: " << T_acc_120[l] << "; Acceleration: " << acc_120[l] << std::endl;
	}
	std::cout << "------Density with fixated height = 500 km------" << std::endl;;
	for (int k = 0; k < 7; k++) {
		std::cout << "Solar activity: " << solar_activity[k] << "; Density: " << density_500[k] << "; S-related acceleration: " << S_acc_500[k] << "; T_related acceleration: " << T_acc_500[k] << "; Acceleration: " << acc_500[k] << std::endl;
	}
	std::cout << "\n";
	std::cout << "----------------------------------------------------------------------------------" << std::endl;



	delete[] density_120;
	delete[] density_500;
	delete[] S_acc_120;
	delete[] T_acc_120;
	delete[] acc_120;
	delete[] S_acc_500;
	delete[] T_acc_500;
	delete[] acc_500;
 }




std::pair<float, float> calculate_anomalies() {
	float E_anomaly_past = M;
	float E_anomaly = M + ECC * sin(E_anomaly_past);


	while (fabs(E_anomaly - E_anomaly_past) > accuracy) {
		E_anomaly_past = E_anomaly;
		E_anomaly = M + ECC * sin(E_anomaly_past);
	}

	float THETA_anomaly = 2 * atan(sqrt((1.0 + ECC) / (1.0 - ECC) * tan(E_anomaly / 2.0)));

	return std::make_pair(E_anomaly, THETA_anomaly);
}

std::pair <float, float> calculate_velocities(float THETA) {
	float radial_velocity = sqrt(grav_param / P) * ECC * sin(THETA);
	float transversal_velocity = sqrt(grav_param / P) * (1 + ECC * cos(THETA));
	return std::make_pair(radial_velocity, transversal_velocity);
}

std::vector<float> calculate_AGECS(float E, float THETA) {
	std::vector<float> AGECS_coords = { 0.0, 0.0, 0.0 };
	float AGECS_radius_vector = SMA * (1 - ECC * cos(E));
	float u = THETA + omega;

	AGECS_coords[0] = AGECS_radius_vector * (cos(u) * cos(Omega) - sin(u) * sin(Omega) * cos(i));
	AGECS_coords[1] = AGECS_radius_vector * (cos(u) * sin(Omega) + sin(u) * cos(Omega) * cos(i));
	AGECS_coords[2] = AGECS_radius_vector * sin(u) * sin(i);

	return AGECS_coords;
}

std::vector<float> calculate_GCS(std::vector<float> AGECS_coords, float time) {
	float ang_rotation = angular_vel * time;
	std::vector<float> GCS_coords = { 0.0, 0.0, 0.0 };

	GCS_coords[0] = cos(ang_rotation) * AGECS_coords[0] + sin(ang_rotation) * AGECS_coords[1];
	GCS_coords[1] = cos(ang_rotation) * AGECS_coords[1] - sin(ang_rotation) * AGECS_coords[0];
	GCS_coords[2] = AGECS_coords[2];

	return GCS_coords;
}

std::vector<float> calculate_geodetic_coords(std::vector<float> GCS_coords) {
	std::vector<float> geodetic_coords = { 0.0, 0.0, 0.0 };

	float a = 6378.136;
	float e = 0.0067385254;
	float x = GCS_coords[0];
	float y = GCS_coords[1];
	float z = GCS_coords[2];
	float D = sqrt(pow(x, 2.0) + pow(y, 2.0));
	if (D == 0) {
		geodetic_coords[0] = 0;
		geodetic_coords[1] = (PI / 2.0) * (z / fabs(z));
		geodetic_coords[2] = z * sin(geodetic_coords[1]) - a * sqrt(1 - e * pow(sin(geodetic_coords[1]), 2.0));
		return geodetic_coords;
	}

	else {
		float L = asin(y / D);
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
			float r = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
			float c = asin(z / r);
			float p = (e * a) / (2 * r);
			float s1 = 0;
			float b = c + s1;
			float s2 = asin((p * sin(2 * b)) / (sqrt(1 - e * pow(sin(b), 2.0))));
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

float *calculate_density(float H) {
	float night_density_1[7];
	float night_density_2[7];
	float density[14];

	float a_param_120_0[7] = { 26.8629, 27.4598, 28.6395, 29.6418, 30.1671, 29.7578, 30.7854 };
	float a_param_120_1[7] = { -0.451674, -0.463668, -0.490987, -0.514957, -0.527837, -0.517915, -0.545695 };
	float a_param_120_2[7] = { 0.00290397, 0.002974, 0.00320649, 0.00341926, 0.00353211, 0.00342699, 0.00370328 };
	float a_param_120_3[7] = { -1.06953 * pow(10.0, -5.0), -1.0753 * pow(10.0, -5.0), -1.1681 * pow(10.0, -5.0), -1.25785 * pow(10.0, -5.0), -1.30227 * pow(10.0, -5.0), -1.24137 * pow(10.0, -5.0), -1.37072 * pow(10.0, -5.0) };
	float a_param_120_4[7] = { 2.21598 * pow(10.0, -8.0), 2.17059 * pow(10.0, -8.0), 2.36847 * pow(10.0, -8.0), 2.5727 * pow(10.0, -8.0), 2.66455 * pow(10.0, -8.0), 2.48209 * pow(10.0, -8.0), 2.80614 * pow(10.0, -8.0) };
	float a_param_120_5[7] = { -2.42941 * pow(10.0, -11.0), -2.30249 * pow(10.0, -11.0), -2.51809 * pow(10.0, -11.0), -2.75874 * pow(10.0, -11.0), -2.85432 * pow(10.0, -11.0), -2.58413 * pow(10.0, -11.0), -3.00184 * pow(10.0, -11.0) };
	float a_param_120_6[7] = { 1.09926 * pow(10.0, -14.0), 1.00123 * pow(10.0, -14.0), 1.09536 * pow(10.0, -14.0), 1.21091 * pow(10.0, -14.0), 1.25009 * pow(10.0, -14.0), 1.09383 * pow(10.0, -14.0), 1.31142 * pow(10.0, -14.0) };

	float a_param_500_0[7] = { 17.8481, -2.54909, -13.9599, -23.3079, -14.7264, -4.912, -5.40952 };
	float a_param_500_1[7] = { -0.132025, 0.0140064, 0.0844951, 0.135141, 0.0713256, 0.0108326, 0.00550749 };
	float a_param_500_2[7] = { 0.000227717, -0.00016946, -0.000328875, -0.000420802, -0.000228015, -8.10546 * pow(10.0, -5.0), -3.78851 * pow(10.0, -5.0) };
	float a_param_500_3[7] = { -2.2543 * pow(10.0, -7.0), 3.27196 * pow(10.0, -7.0), 5.05918 * pow(10.0, -7.0), 5.73717 * pow(10.0, -7.0), 2.8487 * pow(10.0, -7.0), 1.15712 * pow(10.0, -7.0), 2.4808 * pow(10.0, -8.0) };
	float a_param_500_4[7] = { 1.33574 * pow(10.0, -10.0), -2.8763 * pow(10.0, -10.0), -3.92299 * pow(10.0, -10.0), -4.03238 * pow(10.0, -10.0), -1.74383 * pow(10.0, -10.0), -8.13296 * pow(10.0, -11.0), 4.92183 * pow(10.0, -12.0) };
	float a_param_500_5[7] = { -4.50458 * pow(10.0, -14.0), 1.22625 * pow(10.0, -13), 1.52279 * pow(10.0, -13), 1.42846 * pow(10.0, -13), 5.08071 * pow(10.0, -14.0), 3.04913 * pow(10.0, -14.0), -8.65011 * pow(10.0, -15) };
	float a_param_500_6[7] = { 6.72086 * pow(10.0, -18.0), -2.05736 * pow(10.0, -17.0), -2.35576 * pow(10.0, -17.0), -2.01726 * pow(10.0, -17.0), -5.34955 * pow(10.0, -18.0), -4.94989 * pow(10.0, -18.0), 1.9849 * pow(10.0, -18.0) };

	for (int step = 0; step < 7; step++) {
		night_density_1[step] = night_density * pow(ECC, (a_param_120_0[step] + a_param_120_1[step] * H + a_param_120_2[step] * pow(H, 2.0) + a_param_120_3[step] * pow(H, 3.0) + a_param_120_4[step] * pow(H, 4.0) + a_param_120_5[step] * pow(H, 5.0) + a_param_120_6[step] * pow(H, 6.0)));
		night_density_2[step] = night_density * pow(ECC, (a_param_500_0[step] + a_param_500_1[step] * H + a_param_500_2[step] * pow(H, 2.0) + a_param_500_3[step] * pow(H, 3.0) + a_param_500_4[step] * pow(H, 4.0) + a_param_500_5[step] * pow(H, 5.0) + a_param_500_6[step] * pow(H, 6.0)));
	}

	for (int j = 0; j < 14; j++) {
		if (j < 7) {
			density[j] = night_density_1[j];
		}
		else {
			density[j] = night_density_2[j - 7];
		}
	}

	return density;
}

std::array<std::vector <float>, 14> calculate_acceleration(float* density, float radial_vel, float transversal_vel, float vel) {
	std::array<std::vector <float>, 14> acc;
	std::vector <float> acc_ = { 0.0, 0.0, 0.0 };
	for (int l = 0; l < 14; l++) {
		acc_[0] = SIGMA * *(density + l) * vel * radial_vel;
		acc_[1] = SIGMA * *(density + l) * vel * transversal_vel;
		acc[l] = acc_;
	}

	return acc;
}