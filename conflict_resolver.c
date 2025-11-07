#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "globals.h"

#define MAX_DAYS 7
#define MAX_TIMES 6

const char *days[MAX_DAYS] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
const char *times[MAX_TIMES] = {"08:00", "10:00", "12:00", "14:00", "16:00", "18:00"};

int hasConflict(Schedule a, Schedule b)
{
    return (strcmp(a.day, b.day) == 0 &&
            strcmp(a.startTime, b.startTime) == 0 &&
            (strcmp(a.batch, b.batch) == 0 ||
             strcmp(a.teacher, b.teacher) == 0 ||
             strcmp(a.roomNo, b.roomNo) == 0));
}

void findNextAvailableSlot(Schedule *s, Schedule *schedules, int count)
{
    for (int d = 0; d < MAX_DAYS; d++)
    {
        for (int t = 0; t < MAX_TIMES; t++)
        {
            int conflict = 0;
            for (int i = 0; i < count; i++)
            {
                if ((strcmp(s->batch, schedules[i].batch) == 0 ||
                     strcmp(s->teacher, schedules[i].teacher) == 0 ||
                     strcmp(s->roomNo, schedules[i].roomNo) == 0) &&
                    strcmp(days[d], schedules[i].day) == 0 &&
                    strcmp(times[t], schedules[i].startTime) == 0)
                {
                    conflict = 1;
                    break;
                }
            }
            if (!conflict)
            {
                strcpy(s->day, days[d]);
                strcpy(s->startTime, times[t]);
                return;
            }
        }
    }
}

void resolveConflicts(const char *datFile, const char *txtFile)
{
    FILE *fp = fopen(datFile, "rb");
    if (!fp)
    {
        printf("⚠ No schedule found: %s\n", datFile);
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int count = size / sizeof(Schedule);
    Schedule *schedules = malloc(sizeof(Schedule) * count);
    if (!schedules)
    {
        printf("❌ Memory allocation failed.\n");
        fclose(fp);
        return;
    }

    fread(schedules, sizeof(Schedule), count, fp);
    fclose(fp);

    int fixed = 0;

    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (hasConflict(schedules[i], schedules[j]))
            {
                findNextAvailableSlot(&schedules[j], schedules, count);
                fixed++;
            }
        }
    }

    if (fixed > 0)
    {
        fp = fopen(datFile, "wb");
        fwrite(schedules, sizeof(Schedule), count, fp);
        fclose(fp);

        // Save text version
        FILE *txt = fopen(txtFile, "w");
        fprintf(txt, "Day       Timeslot   Batch    Course          Teacher         Room\n");
        fprintf(txt, "---------------------------------------------------------------\n");
        for (int i = 0; i < count; i++)
        {
            fprintf(txt, "%-10s %-9s %-8s %-15s %-15s %-10s\n",
                    schedules[i].day,
                    schedules[i].startTime,
                    schedules[i].batch,
                    schedules[i].courseCode,
                    schedules[i].teacher,
                    schedules[i].roomNo);
        }
        fclose(txt);

        printf("✅ %d conflicts fixed in %s.\n", fixed, datFile);
    }
    else
    {
        printf("✅ No conflicts found in %s.\n", datFile);
    }

    free(schedules);
}
