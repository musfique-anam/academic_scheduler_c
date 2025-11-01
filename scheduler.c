// scheduler.c
#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "globals.h"
#include "structures.h"

// Function to generate class routine
void generateClassRoutine()
{
    if (numBatches == 0 || numCourses == 0 || numTeachers == 0 || numRooms == 0)
    {
        printf("Cannot generate class routine. Make sure batches, courses, teachers, and rooms exist.\n");
        return;
    }

    FILE *fp = fopen("routine.txt", "w");
    FILE *fpDat = fopen("schedule.dat", "wb");
    if (!fp || !fpDat)
    {
        printf("Error opening routine files.\n");
        if (fp)
            fclose(fp);
        if (fpDat)
            fclose(fpDat);
        return;
    }

    // Header
    fprintf(fp, "Day       Timeslot   Batch    Course          Teacher         Room\n");
    fprintf(fp, "---------------------------------------------------------------\n");

    const char *days[] = {"Saturday", "Sunday", "Monday"};
    const char *times[] = {"10:00", "12:00", "14:00"};

    int dayIndex = 0;
    int timeIndex = 0;

    for (int b = 0; b < numBatches; b++)
    {
        Batch *batch = &batches[b];
        for (int c = 0; c < batch->courseCount; c++)
        {
            char *courseCode = batch->courseCodes[c];

            // Find course
            Course *course = NULL;
            for (int i = 0; i < numCourses; i++)
            {
                if (strcmp(courses[i].code, courseCode) == 0)
                {
                    course = &courses[i];
                    break;
                }
            }
            if (!course)
                continue;

            // Find teacher
            Teacher *teacher = NULL;
            for (int t = 0; t < numTeachers; t++)
            {
                if (strcmp(teachers[t].name, course->teacherName) == 0)
                {
                    teacher = &teachers[t];
                    break;
                }
            }

            // Find room
            Room *room = NULL;
            for (int r = 0; r < numRooms; r++)
            {
                if (course->isLab && rooms[r].isLabRoom)
                {
                    room = &rooms[r];
                    break;
                }
                else if (!course->isLab && !rooms[r].isLabRoom)
                {
                    room = &rooms[r];
                    break;
                }
            }

            // Assign day and time
            const char *day = days[dayIndex % 3];
            const char *time = times[timeIndex % 3];

            fprintf(fp, "%-10s %-10s %-8s %-15s %-15s %-8s\n",
                    day, time, batch->name, course->name,
                    teacher ? teacher->name : "TBD",
                    room ? room->roomNo : "TBD");

            // Move to next slot
            timeIndex++;
            if (timeIndex % 3 == 0)
                dayIndex++;
        }
    }

    fclose(fp);
    fclose(fpDat);

    printf("✅ Class routine generated successfully in routine.txt\n");
}

// Additional functions can go here if needed
