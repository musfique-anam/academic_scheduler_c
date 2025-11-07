#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "globals.h"
#include "exam_scheduler.h"

// Helper: calculate date with offset days
void getNextDate(const char *startDate, int offsetDays, char *outDate, int maxLen) {
    struct tm tmDate;
    memset(&tmDate, 0, sizeof(tmDate));

    // Parse YYYY-MM-DD
    sscanf(startDate, "%d-%d-%d", &tmDate.tm_year, &tmDate.tm_mon, &tmDate.tm_mday);
    tmDate.tm_year -= 1900;  // tm_year is years since 1900
    tmDate.tm_mon -= 1;      // tm_mon: 0-11

    tmDate.tm_mday += offsetDays;
    mktime(&tmDate);  // normalize the date

    snprintf(outDate, maxLen, "%04d-%02d-%02d",
             tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday);
}

// Main function to generate exam routine
void generateExamRoutineWithStartDate(const char *startDate) {
    if (numBatches == 0 || numCourses == 0) {
        printf("Cannot generate exam routine. Make sure batches and courses exist.\n");
        return;
    }

    FILE *fpTxt = fopen("exam_routine.txt", "w");
    FILE *fpDat = fopen("exam_schedule.dat", "wb");

    if (!fpTxt || !fpDat) {
        printf("Error opening exam routine files.\n");
        if (fpTxt) fclose(fpTxt);
        if (fpDat) fclose(fpDat);
        return;
    }

    fprintf(fpTxt, "📅 Exam Routine\n\n");

    int dayOffset = 0;  // increment per batch

    for (int b = 0; b < numBatches; b++) {
        char examDate[20];
        getNextDate(startDate, dayOffset, examDate, sizeof(examDate));

        fprintf(fpTxt, "Batch: %s\n", batches[b].name);

        for (int c = 0; c < batches[b].courseCount && c < MAX_COURSES_PER_BATCH; c++) {
            char *courseCode = batches[b].courseCodes[c];

            // Find course details
            Course *course = NULL;
            for (int i = 0; i < numCourses; i++) {
                if (strcmp(courses[i].code, courseCode) == 0) {
                    course = &courses[i];
                    break;
                }
            }

            if (course) {
                fprintf(fpTxt, "  Course: %s (%s)\n", course->name, course->code);
                fprintf(fpTxt, "  Date: %s\n", examDate);
                fprintf(fpTxt, "  Time: 09:00\n");
                fprintf(fpTxt, "  Teacher: %s\n", course->teacherName[0] ? course->teacherName : "TBD");
                fprintf(fpTxt, "  Room: ExamRoom\n");
                fprintf(fpTxt, "-----------------------------------------\n");

                Schedule s;
                memset(&s, 0, sizeof(Schedule));

                strncpy(s.courseCode, course->code, MAX_COURSE_CODE_LEN - 1);
                strncpy(s.batch, batches[b].name, MAX_BATCH_NAME_LEN - 1);
                strncpy(s.day, examDate, MAX_DAY_LEN - 1);
                strncpy(s.startTime, "09:00", MAX_TIME_LEN - 1);
                strncpy(s.roomNo, "ExamRoom", MAX_ROOM_NO_LEN - 1);
                strncpy(s.teacher, course->teacherName[0] ? course->teacherName : "TBD", MAX_TEACHER_NAME_LEN - 1);

                s.isExam = 1;
                s.isLab = course->isLab;
                s.duration = 3;

                fwrite(&s, sizeof(Schedule), 1, fpDat);
            }
        }

        dayOffset++; // next batch exam on next day
    }

    fclose(fpTxt);
    fclose(fpDat);

    printf("✅ Exam routine generated successfully starting from %s.\n", startDate);
}
