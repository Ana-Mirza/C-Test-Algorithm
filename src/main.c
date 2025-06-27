/* 
The MIT License (MIT)

Copyright (c) 2020 Anna Brondin and Marcus Nordström

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitatioceiln the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#define DEBUG
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "StepCountingAlgo.h"
#include "scoringStage.h"
#include "detectionStage.h"
#include "postProcessingStage.h"
#include "motionDetectStage.h"

#define nitems(_a)    (sizeof(_a) / sizeof(0[(_a)]))

/* Helth data */
#define GENDER "F"
#define AGE 22
#define WEIGHT 50
#define HEIGHT 166

struct datapoint
{
	char* path;    /* the data of register */
	int steps;
    int dist;       /* meters */
    float calories; /* kcal */
    char* style;     /* the style of walking */
};

static const struct datapoint distance_dataset[] = {
    {"../Dataset/validate/distance/csv_data/15_metri_incet.csv", 30, 15, 1.0746, "incet"},
    {"../Dataset/validate/distance/csv_data/30_metri_incet.csv", 65, 30, 2.5106, "incet"},
    {"../Dataset/validate/distance/csv_data/45_metri_incet.csv", 97, 45, 3.8806, "incet"},
    {"../Dataset/validate/distance/csv_data/60_metri_incet.csv", 128, 60, 4.892, "incet"},
    {"../Dataset/validate/distance/csv_data/75_metri_incet.csv", 158, 75, 6.038, "incet"},
    {"../Dataset/validate/distance/csv_data/90_metri_incet.csv", 195, 90, 7.033, "incet"},
    {"../Dataset/validate/distance/csv_data/100_metri_incet.csv", 215, 100, 8.215, "incet"},
    {"../Dataset/validate/distance/csv_data/15_metri.csv", 26, 15, 0.728, "normal"},
    {"../Dataset/validate/distance/csv_data/30_metri.csv", 52, 30, 1.4353, "normal"},
    {"../Dataset/validate/distance/csv_data/45_metri.csv", 78, 45, 2.1666, "normal"},
    {"../Dataset/validate/distance/csv_data/60_metri.csv", 106, 60, 2.8726, "normal"},
    {"../Dataset/validate/distance/csv_data/75_metri.csv", 133, 75, 3.611, "normal"},
    {"../Dataset/validate/distance/csv_data/90_metri.csv", 160, 90, 4.288, "normal"},
    {"../Dataset/validate/distance/csv_data/100_metri.csv", 187, 100, 5.156, "normal"},
    {"../Dataset/validate/distance/csv_data/15_metri_rapid.csv", 22, 15, 0.6793, "rapid"},
    {"../Dataset/validate/distance/csv_data/30_metri_rapid.csv", 45, 30, 1.2694, "rapid"},
    {"../Dataset/validate/distance/csv_data/45_metri_rapid.csv", 72, 45, 1.8997, "rapid"},
    {"../Dataset/validate/distance/csv_data/60_metri_rapid.csv", 90, 60, 2.475, "rapid"},
    {"../Dataset/validate/distance/csv_data/75_metri_rapid.csv", 117, 75, 3.113, "rapid"},
    {"../Dataset/validate/distance/csv_data/90_metri_rapid.csv", 144, 90, 3.73, "rapid"},
    {"../Dataset/validate/distance/csv_data/100_metri_rapid.csv", 152, 100, 4.115, "rapid"},
    {"../Dataset/validate/distance/csv_data/15_metri_mixt.csv", 25, 15, 2.0867, "mixt"},
    {"../Dataset/validate/distance/csv_data/30_metri_mixt.csv", 52, 30, 1.5272, "mixt"},
    {"../Dataset/validate/distance/csv_data/45_metri_mixt.csv", 76, 45, 3.187, "mixt"},
    {"../Dataset/validate/distance/csv_data/60_metri_mixt.csv", 10, 60, 2.922, "mixt"},
    {"../Dataset/validate/distance/csv_data/75_metri_mixt.csv", 126, 75, 3.721, "mixt"},
    {"../Dataset/validate/distance/csv_data/90_metri_mixt.csv", 152, 90, 4.402, "mixt"},
    {"../Dataset/validate/distance/csv_data/100_metri_mixt.csv", 168, 100, 4.915, "mixt"},
    // {"../Dataset/validate/distance/csv_data/stationar.csv", 0, 0, "-"},
    // {"../Dataset/validate/distance/csv_data/pasi_mici.csv", 40, 15, "pasi mici"},
    // {"../Dataset/validate/distance/csv_data/pasi_mari.csv", 22, 15, "pasi mari"},
    // {"../Dataset/validate/distance/csv_data/opriri.csv", 57, 30, "cu opriri"}
    // {"../Dataset/validate/person1/csv_data/leganat_alergat.csv", 100, 0, "alergat"},
    // {"../Dataset/optimize/csv_data/leganat_alergat.csv", 150, 0, "alergat"},
};

static const struct datapoint steps_dataset[] = {
    {"../Dataset/validate/person1/csv_data/leganat_normal.csv", 150, 0, 0, "normal"},
    {"../Dataset/validate/person1/csv_data/leganat_incet.csv", 150, 0, 0, "incet"},
    {"../Dataset/validate/person1/csv_data/leganat_rapid.csv", 150, 0, 0, "rapid"},
    {"../Dataset/validate/person1/csv_data/leganat_alergat.csv", 150, 0, 0, "alergat"},
    {"../Dataset/validate/person1/csv_data/buzunar.csv", 170, 0, 0, "normal"},
    {"../Dataset/validate/person1/csv_data/stationar.csv", 150, 0, 0, "normal"},
    {"../Dataset/validate/person1/csv_data/scari.csv", 150, 0, 0, "normal"},
    {"../Dataset/optimize/csv_data/50_pasi.csv", 50, 0, 0, "normal"},
    {"../Dataset/optimize/csv_data/10pasi.csv", 10, 0, 0, "normal"},
};

static float abs_val(float a) {
    return a > 0.0 ? a : a * (-1.0);
}

static void runAlgo(char* path)
{
    char *line = NULL;
    size_t len = 0;
    size_t read;
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        long time = strtol(strtok(line, ","), NULL, 10);
        long x = (strtol(strtok(NULL, ","), NULL, 10));
        long y = (strtol(strtok(NULL, ","), NULL, 10));
        long z = (strtol(strtok(NULL, ","), NULL, 10));
        processSample(time, x, y, z);
    }
    fclose(fp);
    if (line)
        free(line);
}

int main(int argc, char *argv[])
{
    initAlgo(GENDER, AGE, HEIGHT, WEIGHT);

    int error = 0;
    int totalSteps = 0;
    int totalCorectSteps = 0;
    float mean = 0;
    float meanDistAcc = 0;
    float meanCalAcc = 0;

    for (int idx = 0; idx < nitems(distance_dataset); idx++)
    {
        int correct_steps = distance_dataset[idx].steps;
        runAlgo(distance_dataset[idx].path);

        int steps = getSteps();
        float dist = getDistance();
        float kcal = getCalories();
        float meanAvg = getMeanAvg();

        mean += meanAvg;

        totalSteps += steps;
        totalCorectSteps += correct_steps;

        /* accuracies for distance and calories */
        float distError = abs_val(dist - distance_dataset[idx].dist) / (float) distance_dataset[idx].dist;
        float calError = abs_val((float)kcal - (float)distance_dataset[idx].calories) / distance_dataset[idx].calories;
        float distAcc = (1.0 - distError) * 100.0;
        float calAcc = (1.0 - calError) * 100.0;

        meanDistAcc += distAcc;
        meanCalAcc += calAcc;

        /* Print data data */
        printf("DistAcc=%f calAcc=%f Distance=%f kcal=%f Steps=%d CorrectSteps=%d CorrectDistance=%d CorrectCalories=%f mers-%s\n",
            distAcc, calAcc, dist, kcal, steps, correct_steps, distance_dataset[idx].dist, distance_dataset[idx].calories, distance_dataset[idx].style);

        resetSteps();
        resetAlgo();
    }

    /* compute mean accuracy over the dataset */
    float meanAvg = mean / nitems(distance_dataset);
    meanDistAcc = meanDistAcc / (float) nitems(distance_dataset);
    meanCalAcc = meanCalAcc / (float) nitems(distance_dataset);

    printf("Total steps counted=%d total correct number=%d\n", totalSteps, totalCorectSteps);
    printf("meanAvg=%f\n", meanAvg);
    printf("Mean distance accuracy=%f;   Mean calories accuracy=%f\n", meanDistAcc, meanCalAcc);
}
