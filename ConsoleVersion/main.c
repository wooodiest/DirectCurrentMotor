// Compiled with MSVC
// To build with VisualStudio add _CRT_SECURE_NO_WARNINGS in preprocessor definitions

// Created by
//     - Michal Kuchnicki
//     - Mikolaj Nowacki
//     - Konrad Matuszewski

// Useful:
//     - CSV Plots - https://chart-studio.plotly.com/create/
//     - CSV Plots - https://www.csvplot.com/

// Includes
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Constances for user input                    /* if not specified, SI units */
const float min_current          =  0.000001f;
const float min_magnetic_field   =  0.000001f;
const float min_frame_side       =  0.000001f;
const float min_alpha            = -180.0f;     /* degrees */
const int   min_number_of_wires  =  1;
const float min_angular_velocity =  0.000001f;
const float min_delta_time       =  0.000001f; 
const float min_time_period      =  1.000f;
const float min_frame_mass       =  0.000001f;

const float max_current          =  1000000.0f;
const float max_magnetic_field   =  1000000.0f;
const float max_frame_side       =  1000000.0f;
const float max_alpha            =  180.0f;     /* degrees */
const int   max_number_of_wires  =  1000;
const float max_angular_velocity =  1000000.0f;
const float max_delta_time       =  0.016f;
const float max_time_period      =  60.000f;
const float max_frame_mass       =  1000000.0f;

const char* division_line        = "----------------------------------------------------------------------------------------";

// Definitions
int  DisplayMenu();
void Solution_1();
void Solution_2();
void Solution_3();

// Utils
float GetFloat(const char* message, float min, float max);
int   GetInt  (const char* message, int   min,   int max);
float DegToRad(float degrees);
float RadToDeg(float radians);
int   CheckQuadrant(float radians);

// Entry point
int main(int argc, char** argv)
{
    printf("Symulacja pracy silnika elektrycznego na prad staly\n");
    printf(" - Michal Kuchnicki\n");
    printf(" - Mikolaj Nowacki\n");
    printf(" - Konrad Matuszewski\n");

    int running = 1;
    while (running)
        running = DisplayMenu(); /* DisplayMenu() returns 0 if user wanted to quit */ 
 
    return 0;
}

// Declarations
void Solution_1()
{   
    // Get data
    float current, magneticField, frameSideA, frameSideB, alpha; /* alpha in radians */
    int   numberOfWires;

    printf("%s\n", division_line);
    printf("Wyznaczenie momentu sily dzialajacego na ramke z pradem w polu magnetycznym dla dowolnego kata alfa pomiedzy ramka a liniami pola magnetycznego.\n\n");

    current       = GetFloat("Podaj wartosc natezenia pradu: ",      min_current,         max_current);
    magneticField = GetFloat("Podaj wartosc pola magnetycznego: ",   min_magnetic_field,  max_magnetic_field);   
    frameSideA    = GetFloat("Podaj wartosc boku A ramki: ",         min_frame_side,      max_frame_side);
    frameSideB    = GetFloat("Podaj wartosc boku B ramki: ",         min_frame_side,      max_frame_side);    
    numberOfWires = GetInt("Podaj ilosc zwoi: ",                     min_number_of_wires, max_number_of_wires);
    alpha = DegToRad(GetFloat("Podaj wartosc kata alpha(stopnie): ", min_alpha,           max_alpha));

    printf("\nDane:\n");
    printf("Natezenie pradu       : %f\n", current);
    printf("Pole magnetyczne      : %f\n", magneticField);
    printf("Bok A ramki           : %f\n", frameSideA);
    printf("Bok B ramki           : %f\n", frameSideB);
    printf("Ilosc zwoi            : %d\n", numberOfWires);
    printf("Kat alfa              : %f\n", RadToDeg(alpha));

    // Output data
    float torque = current * magneticField * frameSideA * frameSideB * numberOfWires * sin(alpha);
    printf("\nMoment sily: %f\n", torque);
}

void Solution_2()
{
    // Get data
    float current, magneticField, frameSideA, frameSideB, mass, alpha, angularVelocity, deltaTime, timePeriod; /* alpha in radians */
    int   numberOfWires;

    printf("%s\n", division_line);
    printf("Symulacja pracy silnika - numeryczne rozwiazanie rownania momentow metoda Eulera.\n\n");

    current         = GetFloat("Podaj wartosc natezenia pradu: ",        min_current,         max_current);
    magneticField   = GetFloat("Podaj wartosc pola magnetycznego: ",     min_magnetic_field,  max_magnetic_field);
    frameSideA      = GetFloat("Podaj wartosc boku A ramki: ",           min_frame_side,      max_frame_side);
    frameSideB      = GetFloat("Podaj wartosc boku B ramki: ",           min_frame_side,      max_frame_side);
    mass            = GetFloat("Podaj wartosc masy ramki: ",             min_frame_mass,      max_frame_mass);
    numberOfWires   = GetInt("Podaj ilosc zwoi: ",                       min_number_of_wires, max_number_of_wires);
    alpha           = DegToRad(GetFloat("Podaj wartosc poczatkowa kata alpha(stopnie): ", min_alpha,            max_alpha));
    angularVelocity = GetFloat("Podaj wartosc poczatkowa predkosci katowej: ",            min_angular_velocity, max_angular_velocity);
    deltaTime       = GetFloat("Podaj wartosc deltaTime: ",     min_delta_time,  max_delta_time);
    timePeriod      = GetFloat("Podaj wartosc czasu pomiaru: ", min_time_period, max_time_period);

    printf("\nDane:\n");
    printf("Natezenie pradu       : %f\n", current);
    printf("Pole magnetyczne      : %f\n", magneticField);
    printf("Bok A ramki           : %f\n", frameSideA);
    printf("Bok B ramki           : %f\n", frameSideB);
    printf("Ilosc zwoi            : %d\n", numberOfWires);
    printf("Kat alfa              : %f\n", RadToDeg(alpha));
    printf("Predkosc katowa       : %f\n", angularVelocity);
    printf("DeltaTime             : %f\n", deltaTime);
    printf("Czas                  : %f\n", timePeriod);

    // Alocate memory
    int n = (int)(timePeriod / deltaTime) + 1; // number of elements (+1 because first element will be count as 0.0s)
    float* out_Alpha               = malloc(n * sizeof(float));
    float* out_AngularVelocity     = malloc(n * sizeof(float));
    float* out_AngularAcceleration = malloc(n * sizeof(float));
    float* out_Torque              = malloc(n * sizeof(float));
    float* out_Time                = malloc(n * sizeof(float));

    // Calculations
    const float inertia = (frameSideA * frameSideA * mass) / ( 4 * (frameSideA + frameSideB)) * (frameSideB + frameSideA / 3.0f);
    float K  = current * magneticField * frameSideA * frameSideB * numberOfWires;
    float KI = K / inertia;

    out_AngularVelocity[0]     = angularVelocity;
    out_Alpha[0]               = alpha;          
    out_Torque[0]              = K * sin(out_Alpha[0]);
    out_AngularAcceleration[0] = out_Torque[0] / inertia;
    out_Time[0]                = 0.0f;

    float prevQuadrant = CheckQuadrant(M_PI_2 - out_Alpha[0]); // we want the angle between the positive x axis
    int i;
    for (i = 1; i < n; i++)
    {
        out_AngularVelocity[i]     = out_AngularVelocity[i - 1] + deltaTime * KI * sin(out_Alpha[i - 1]);
        out_Alpha[i]               = out_Alpha[i - 1]           + deltaTime * out_AngularVelocity[i];
        out_Torque[i]              = K * sin(out_Alpha[i]);
        out_AngularAcceleration[i] = out_Torque[i] / inertia;
        out_Time[i]                = i * deltaTime;
       
        // To simulate a change in the direction of current flow so that the motor rotates all the time,
        // we check what quadrant of the coordinate system we are in and if appropriate transitions occur,
        // we change the direction of current flow and recalculate what is needed
        int quadrant = CheckQuadrant(M_PI_2 - out_Alpha[i]); // we want the angle between the positive x axis
        if ((prevQuadrant == 1 && quadrant == 2) ||
            (prevQuadrant == 2 && quadrant == 1) ||
            (prevQuadrant == 3 && quadrant == 4) ||
            (prevQuadrant == 4 && quadrant == 3))
        {
            current *= -1.0f;
            K = current * magneticField * frameSideA * frameSideB * numberOfWires;
            KI = K / inertia;
        }
        prevQuadrant = quadrant;
    }

    // Export data
    const char* fileName = "symulacja.csv";
    FILE* file = fopen(fileName, "w");
    if (file == NULL)
    { 
        printf("Time,Alpha,Angular Velocity,Angular Acceleration,Torque\n");    /* Output Alpha in radians */
        for (i = 0; i < n; i++) 
            printf("%f,%f,%f,%f,%f\n", out_Time[i], RadToDeg(out_Alpha[i]), out_AngularVelocity[i], out_AngularAcceleration[i], out_Torque[i]);
                  
        printf("\nNie mozna zapisac danych do pliku: %s\n", fileName);
    }
    else
    {
        fprintf(file, "Time,Alpha,Angular Velocity,Angular Acceleration,Torque\n"); /* Output Alpha in radians */
        for (i = 0; i < n; i++)
            fprintf(file, "%f,%f,%f,%f,%f\n", out_Time[i], RadToDeg(out_Alpha[i]), out_AngularVelocity[i], out_AngularAcceleration[i], out_Torque[i]);

        fclose(file);
        printf("\nDane symulacji zostaly zapisane do pliku: %s\n", fileName);
    }

    /// Free memory
    free(out_Alpha);
    free(out_AngularVelocity);
    free(out_AngularAcceleration);
    free(out_Torque);
    free(out_Time);
}

void Solution_3()
{
    // Get data
    float frameSideA, frameSideB, mass;

    printf("%s\n", division_line);
    printf("Wyznaczenie momentu bezwladnosci ramki w oparciu o jej geometrie.\n\n");

    frameSideA = GetFloat("Podaj wartosc boku A ramki: ", min_frame_side, max_frame_side);
    frameSideB = GetFloat("Podaj wartosc boku B ramki: ", min_frame_side, max_frame_side);
    mass       = GetFloat("Podaj wartosc masy ramki: ",   min_frame_mass, max_frame_mass);

    printf("\nDane:\n");
    printf("Bok A ramki           : %f\n", frameSideA);
    printf("Bok B ramki           : %f\n", frameSideB);
    printf("Masa ramki            : %f\n", mass);

    // Output data
    float inertia = (frameSideA * frameSideA * mass) / (4 * (frameSideA + frameSideB)) * (frameSideB + frameSideA / 3.0f);
    printf("\nMoment bezwladnosci ramki: %f\n", inertia);
}

int DisplayMenu()
{
    printf("%s\n", division_line);
    const char* menu = 
        "[1] - Wyznaczanie momentu sily\n"
        "[2] - Symulacja pracy silnika\n"
        "[3] - Wyznaczanie momentu bezwladnosci\n"
        "[0] - Wyjscie\n"
        "Wybierz zagadnienie: ";

    switch (GetInt(menu, 0, 3))
    {
        case 1: Solution_1(); return 1;
        case 2: Solution_2(); return 1;
        case 3: Solution_3(); return 1;
        case 0: return 0;

        default: return 1;
    }
}

float GetFloat(const char* message, float min, float max)
{
    float output = 0.0f;
    int succes = 0;
    int char_buffer;

    while (!succes)
    {
        printf("%s", message);

        if (!scanf("%f", &output))
        {
            while ((char_buffer = getchar()) != '\n') {}
            printf("Nie poprawna wartosc\n");
            continue;
        }
        if (min > output || output > max)
        {
            printf("Nie poprawna wartosc. Wartosc musi byz z zakresu: (%f, %f)\n", min, max);
            continue;
        }
        succes = 1;
    }

    return output;
}

int GetInt(const char* message, int min, int max)
{
    int output = 0;
    int succes = 0;
    int char_buffer;

    while (!succes)
    {
        printf("%s", message);

        if (!scanf("%d", &output))
        {
            while ((char_buffer = getchar()) != '\n') {}
            printf("Nie poprawna wartosc\n");
            continue;
        }
        if (min > output || output > max)
        {
            printf("Nie poprawna wartosc. Wartosc musi byz z zakresu: (%d, %d)\n", min, max);
            continue;
        }
        succes = 1;
    }

    return output;
}

int CheckQuadrant(float radians)
{
    float angle = fmod(radians, 2 * M_PI);

    while (angle < 0.0f)
        angle += 2 * M_PI;        

    if (angle >= 0.0f          && angle < M_PI_2)
        return 1;
    if (angle >= M_PI_2        && angle < M_PI)
        return 2;
    if (angle >= M_PI          && angle < 3.0f * M_PI_2)
        return 3;
    if (angle >= 3.0f * M_PI_2 && angle < 2.0f * M_PI)
        return 4;
}

float DegToRad(float degrees)
{
    return (degrees / 180.0f) * M_PI;
}

float RadToDeg(float radians)
{
    return radians * (180.0f / M_PI);
}
