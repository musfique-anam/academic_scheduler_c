#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conflict.h"
#include "globals.h"
#include "structures.h"

// Detect class conflicts
void detectClassConflicts()
{
    FILE *fp = fopen("schedule.dat", "rb");
    if (!fp)
    {
        printf("No class schedule file found.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int count = size / sizeof(Schedule);
    Schedule *schedules = malloc(sizeof(Schedule) * count);
    if (!schedules)
    {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return;
    }

    fread(schedules, sizeof(Schedule), count, fp);
    fclose(fp);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            // Batch conflict
            if (strcmp(schedules[i].batch, schedules[j].batch) == 0 &&
                strcmp(schedules[i].day, schedules[j].day) == 0 &&
                strcmp(schedules[i].startTime, schedules[j].startTime) == 0)
            {
                printf("⚠ Class conflict for batch %s on %s at %s\n",
                       schedules[i].batch, schedules[i].day, schedules[i].startTime);
                found = 1;
            }
            // Teacher conflict
            if (strcmp(schedules[i].teacher, schedules[j].teacher) == 0 &&
                strcmp(schedules[i].day, schedules[j].day) == 0 &&
                strcmp(schedules[i].startTime, schedules[j].startTime) == 0)
            {
                printf("⚠ Teacher conflict: %s has overlapping classes on %s at %s\n",
                       schedules[i].teacher, schedules[i].day, schedules[i].startTime);
                found = 1;
            }
        }
    }

    if (!found)
        printf("✅ No class conflicts detected.\n");

    free(schedules);
}

// Detect exam conflicts
void detectExamConflicts()
{
    FILE *fp = fopen("exam_schedule.dat", "rb");
    if (!fp)
    {
        printf("No exam schedule file found.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int count = size / sizeof(Schedule);
    Schedule *schedules = malloc(sizeof(Schedule) * count);
    if (!schedules)
    {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return;
    }

    fread(schedules, sizeof(Schedule), count, fp);
    fclose(fp);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            // Batch conflict
            if (strcmp(schedules[i].batch, schedules[j].batch) == 0 &&
                strcmp(schedules[i].day, schedules[j].day) == 0 &&
                strcmp(schedules[i].startTime, schedules[j].startTime) == 0)
            {
                printf("⚠ Exam conflict for batch %s on %s at %s\n",
                       schedules[i].batch, schedules[i].day, schedules[i].startTime);
                found = 1;
            }
            // Teacher conflict
            if (strcmp(schedules[i].teacher, schedules[j].teacher) == 0 &&
                strcmp(schedules[i].day, schedules[j].day) == 0 &&
                strcmp(schedules[i].startTime, schedules[j].startTime) == 0)
            {
                printf("⚠ Teacher conflict: %s has overlapping exams on %s at %s\n",
                       schedules[i].teacher, schedules[i].day, schedules[i].startTime);
                found = 1;
            }
        }
    }

    if (!found)
        printf("✅ No exam conflicts detected.\n");

    free(schedules);
}

// Wrapper function to match main menu call
void detectConflicts()
{
    detectClassConflicts();
    detectExamConflicts();
}
