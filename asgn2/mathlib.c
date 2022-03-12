#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define EPSILON 1e-14

double Exp(double x) {
    double term = 1.0;
    double sum = term;
    if (x == 0.0) {
        return 1.0;
    } else if (x < 0.0) {
        return 1.0 / Exp(Abs(x));
    } else {
        for (double i = 1.0; term > EPSILON; i++) {
            term *= x / i;
            sum += term;
        }
        return sum;
    }
}
//following assignment doc pseudocode

double Sin(double x) {
    int sign = 1;
    double sum = x;
    double term = x;
    double k = 3.0;
    while (x > 2 * M_PI) {
        x /= 2 * M_PI;
    }
    while (Abs(sum) > EPSILON) {
        term = term * (x * x) / ((k - 1.0) * k);
        sign = -sign;
        sum += sign * term;
        k += 2.0;
    }
    return sum;
    //following the pseudocode on assignment doc
}

double Cos(double x) {
    int sign = 1;
    double term = 1.0;
    double sum = 1.0;
    double k = 2.0;
    while (x > 2 * M_PI) {
        x /= 2 * M_PI;
    }
    while (Abs(term) > EPSILON) {
        term = term * (x * x) / ((k - 1.0) * k);
        sign = -sign;
        sum += sign * term;
        k += 2.0;
    }
    return sum;
}
//following python pseudocode on assignment doc
double Sqrt(double x) {

    if (x < 0.0) {
        return 0.0;
    }
    double prev_term = 0.0;
    double curr_term = 1.0;
    double scale = 1.0;
    while (x > 1) {
        x /= 4.0;
        scale *= 2;
    }
    while (Abs(curr_term - prev_term) > EPSILON) {
        prev_term = curr_term;
        curr_term = 0.5 * (prev_term + (x / prev_term));
    }
    return curr_term * scale;
}
//following python pseudocode on assignment doc and scaling
double Log(double x) {
    if (x <= 0.0) {
        return 0.0;
    }
    double y = 1.0;
    double e = Exp(y);
    double scale = Exp(1.0);
    double counter = 0.0;
    while (x > Exp(1.0)) {
        x /= scale;
        counter += 1.0;
    }
    while (Abs(e - y) > EPSILON) {
        y = y + x / e - 1;
        e = Exp(y);
    }
    return y + counter;
}
//Following python pseudocode on assignment doc and scaling

double integrate(double (*f)(double), double a, double b, uint32_t n) {
    double sum = f(a) + f(b);
    double h = (b - a) / n;
    for (uint32_t i = 1; i < n / 2 - 1; i++) {
        sum += 2 * f(a + (2 * i) * ((b - a) / n));
    }
    for (uint32_t j = 1; j < n / 2; j++) {
        sum += 4 * f(a + (2 * j - 1) * ((b - a) / n));
    }
    sum *= (h / 3.0);
    return sum;
}
