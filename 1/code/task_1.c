#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.1415926

int DataCollection(int, float, float);


int main() {
    float Ra = 1017.0;
    float Rp = 350.0;
    float a = (Ra + Rp) / 2;
    float e = (Ra - Rp) / (Ra + Rp);
    float p = (Ra - Rp) / (2 * a);
    int T = 10695;
    printf("%f", p);
    DataCollection(T, e, p);
    return 0;
}

int DataCollection(int T, float e, float p) {
    FILE* M_dat = fopen("M_Data.txt", "w");
    FILE* E_dat = fopen("E_Data.txt", "w");
    FILE* THETA_dat = fopen("THETA_Data.txt", "w");
    FILE* R_dat = fopen("Radius_Data.txt", "w");
    FILE* Vn_dat = fopen("VelN_Data.txt", "w");
    FILE* Vr_dat = fopen("VelR_Data.txt", "w");
    FILE* V_dat = fopen("Velocity_Data.txt", "w");

    float past_E = 0;
    for (int i = 0; i < T; i++) {
        float M = (2 * PI / T) * i;
        float E = M + e * sin(past_E);
        float THETA = atan(tan((E + PI) / 2) * sqrt((1 + e) / (1 - e))) / 2;
        float radius = p / (1 + e * cos(THETA));
        float Vn = sqrt(M / p) * (1 + e * cos(THETA));
        float Vr = sqrt(M / p) * e * sin(THETA);
        float V = sqrt(pow(Vn, 2) + pow(Vr, 2));

        fprintf(M_dat, "%f \n", M);
        fprintf(E_dat, "%f \n", E);
        fprintf(THETA_dat, "%f \n", THETA);
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