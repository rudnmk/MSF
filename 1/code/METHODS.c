#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define SIGMA 0.001

float* IterationMethod(float, int);
float* HalfDivMethod(float, float, float, int);
float* GoldenRatioMethod(float, float, float, int);
float* NewtonMethod(float, int);
float* LaguerreMethod(float, int);


int main() {
    float Ra = 1017.0;
    float Rp = 350.0;
    float a = (Ra + Rp) / 2;
    float e = (Ra - Rp) / (Ra + Rp);
    float p = (Ra - Rp) / 2 * a;
    float T = 10695.0;
}


float* IterationMethod(float e, int T) {
    float* E_ARR = (float*)malloc(T * sizeof(float));
    E_ARR[0] = 0;
    for(int i = 1; i < T; i++) {
        float M = (2 * 3.14 / T) * i;
        E_ARR[i] = e * sin(E_ARR[i - 1]) + M;
    }

    return E_ARR;
}

float* HalfDivMethod(float e, float A, float B, int T) {
    int i = 0;
    float* E_ARR = (float*)malloc(T * sizeof(float));
    int flag = 0;
    float C;

    while ((i < T) && ((B - A) > SIGMA)) {
        float M = (2 * 3.14 / T) * i;
        if (flag == 0) {
            C = (B + A) / 2;
        }

        else {
            C = (B - A) / 2;
        }
        
        float Ea = A - e * sin(A) - M;
        float Eb = B - e * sin(B) - M;
        float Ec = C - e * sin(C) - M;

        if (Ea * Ec <= 0.0) {
            B = C;
        }
        else {
            A = C;
        }

        if ((A < 0.0 && B < 0.0) || (A > 0.0 && B > 0.0)) {
            flag = 1;
        }

        E_ARR[i] = Ec;
        i++;
        
        if (E_ARR[i] == E_ARR[i - 1]) {
            break;
        }
    }

    return E_ARR;
}

float* GoldenRatioMethod(float e, float A, float B, int T) {
    int i = 0;
    float* E_ARR = (float*)malloc(T * sizeof(float));
    int flag = 0;
    float C;

    while ((i < T) && ((B - A) > SIGMA)) {
        int M = (2 * 3.14 / T) * i;
        if (flag == 0) {
            C = (B + A) / 1.618;
        }

        else {
            C = (B - A) / 1.618;
        }
        
        float Ea = A - e * sin(A) - M;
        float Eb = B - e * sin(B) - M;
        float Ec = C - e * sin(C) - M;

        if (Ea * Ec <= 0) {
            B = C;
        }
        else {
            A = C;
        }

        if ((A < 0 && B < 0) || (A > 0 && B > 0)) {
            flag = 1;
        }

        E_ARR[i] = Ec;
        i++;
    }

    return E_ARR;
}

float* NewtonMethod(float e, int T) {
    float* E_ARR = (float*)malloc(T * sizeof(float));
    E_ARR[0] = 0;
    for (int i = 1; i < T; i++) {
        float M = (2 * 3.14 / T) * i;
        float fE = E_ARR[i - 1] - e * sin(E_ARR[i - 1]) - M;
        float DfE = 1 - e * cos(E_ARR[i - 1]); 
        E_ARR[i] = E_ARR[i - 1] - (fE / DfE);
    }
    return E_ARR;
}

float* LaguerreMethod(float e, int T) {
    float* E_ARR = (float*)malloc(T * sizeof(float));
    int n = 3;
    E_ARR[0] = 0;
    for (int i = 1; i < T; i++) {
        float M = (2 * 3.14 / T) * i;
        float fE = E_ARR[i - 1] - e * sin(E_ARR[i - 1]) - M;
        float DfE = 1 - e * cos(E_ARR[i - 1]);
        float DDfE = e * sin(E_ARR[i - 1]);
        float hE = fabs((n - 1) * ((n - 1) * pow(DfE, 2) - n * fE * DDfE));
        E_ARR[i] = E_ARR[i - 1] - ((fE * n) / (DfE + sqrt(hE)));
    }
    return E_ARR;
}