#include <stdio.h>
#include <stdlib.h>
#include "iolibrary.h"
#include "CSVLib.h"
#include <math.h>

#define M_PI 3.14159265358979323846
#define LIGHT 299.792458

struct dipole
{
    double percent[100];
    double distance[100];
    double t_amplitude[100];
    double b_amplitude[100];
    double impedance[100];
};

double deg_to_rad(double deg)
{
    return M_PI / 180.0 * deg;
}


void array_sort_double(double *array, int n)
{
    int i, j, temp;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

struct dipole calculate(double b_freq, double t_freq)
{
    struct dipole dp;

    double wire_len_base = 234.0 / b_freq;
    double wavelength = LIGHT / b_freq;
    double wire_len_base_meters = wire_len_base / 3.2808399;
    double deg_ratio = t_freq / b_freq;

    for (int a = 0; a <= 90; a++)
    {
        dp.percent[a] = (double)a / 90.0 * 50.0;
        dp.distance[a] = (double)a / 90 * wire_len_base;
        dp.t_amplitude[a] = sin(deg_to_rad((double)a * deg_ratio));
        dp.b_amplitude[a] = sin(deg_to_rad((double)a));
        dp.impedance[a] = 73.1 / pow(sin(2.0 * (sin(deg_to_rad((double)a)) * wire_len_base_meters) * (M_PI / wavelength)), 2.0);
    }

    return dp;
}

int main(void)
{
    double freq_base; 
    char **parsed = NULL;
    double *buffer = NULL;
    char *line;
    int numberOfFields;
    double threshold = 0.0;

    struct dipole dp;

    line = getstring("\n<F>ull or <S>ummary? ");

    if (line[0] == 'S' || line[0] == 's')
    {
        while (1)
        {
            threshold = getdouble("\nEnter the match threshold (0.0-1.0): ");
            if (threshold > 0.0 && threshold <= 1.0) break;
        }
    }

    line = getstring("\nEnter frequencies separated by commas: ");

    if (!(parsed = CSVParse(line, &numberOfFields)))
    {
        printf("String parsing failed!\n");
        return 1;
    }

    free(line);

    buffer = realloc(buffer, sizeof(double) * numberOfFields);

    if (buffer == NULL)
        return 1;

    for (int i = 0; i < numberOfFields; i++)
    {
        buffer[i] = atof(parsed[i]);
    }

    cleanupStrings(parsed, numberOfFields);

    array_sort_double(buffer, numberOfFields);

    freq_base = buffer[0];

    for (int i = 1; i < numberOfFields; i++)
    {
        dp = calculate(freq_base, buffer[i]);

        printf("\n\nFrequency: %5.1f\n\tAngle\tPercent\tDistance\tB_Amplitude\tT_Amplitude\tImpedance", buffer[i]);
        for (int x = 0; x <= 90; x++)
        {
                if ((fabs(dp.b_amplitude[x]) >= threshold && fabs(dp.t_amplitude[x]) >= threshold)||threshold == 0.0)
                    printf("\n\t%d\t%5.1f\t%5.1f\t\t%5.3f\t\t%5.3f\t\t%5.1f", x, dp.percent[x], dp.distance[x], dp.b_amplitude[x], dp.t_amplitude[x], dp.impedance[x]);    
        }
    }

    free(buffer);

    return 0;
}