#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.1415926
#define μ 4902.8000  //Гравитационный параметр Луны

int DataCollection(float, float, float);


int main() {
    float Ra = 2754.1;
    float Rp = 2087.1;
    float a = (Ra + Rp) / 2.0;
    float e = (Ra - Rp) / (2.0 * a);
    float p = a * (1 - pow(e, 2.0));
    DataCollection(a, e, p);
    return 0;
}

int DataCollection(float a, float e, float p) {
    FILE* T_ = fopen("T.txt", "w");
    FILE* M_dat = fopen("M_Data.txt", "w");
    FILE* E_dat = fopen("E_Data.txt", "w");
    FILE* THETA_dat = fopen("THETA_Data.txt", "w");
    FILE* R_dat = fopen("Radius_Data.txt", "w");
    FILE* Vn_dat = fopen("VelN_Data.txt", "w");
    FILE* Vr_dat = fopen("VelR_Data.txt", "w");
    FILE* V_dat = fopen("Velocity_Data.txt", "w");

    float past_E = 0.0;
    float T = 10695.0;
    fprintf(T_, "%f", T);
    fclose(T_);
    for (float i = 0.0; i < T; i++) {
        float M = (2.0 * PI / T) * i;
        float E = e * sin(past_E) + M;
        float THETA = atan(tan(past_E / 2.0) * sqrt((1 + e) / (1 - e))) * 2.0;
        float radius = p / (1 + e * cos(THETA));
        float Vn = sqrt(μ / p) * (1 + e * cos(THETA));
        float Vr = sqrt(μ / p) * e * sin(THETA);
        float V = sqrt(pow(Vn, 2.0) + pow(Vr, 2.0));

        fprintf(M_dat, "%f \n", (M * 180.0 / PI));
        fprintf(E_dat, "%f \n", (E * 180.0 / PI));
        if (THETA < 0) {
            fprintf(THETA_dat, "%f \n", (THETA * 180.0 / PI + 360.0));
        }
        else {
            fprintf(THETA_dat, "%f \n", (THETA * 180.0 / PI));
        }
        fprintf(R_dat, "%f \n", radius);
        fprintf(Vn_dat, "%f \n", Vn);
        fprintf(Vr_dat, "%f \n", Vr);
        fprintf(V_dat, "%f \n", V);

        past_E = E;
    }


    fclose(M_dat);
    fclose(E_dat);
    fclose(THETA_dat);
    fclose(R_dat);
    fclose(Vn_dat);
    fclose(Vr_dat);
    fclose(V_dat);
}