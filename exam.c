// exam.c
#include <stdio.h>
#include <string.h>
#include "exam.h"
#include "globals.h"
#include "structures.h"

void generateExamRoutine() {
    if (numBatches == 0 || numCourses == 0) {
        printf("Cannot generate exam routine. Make sure batches and courses exist.\n");
        return;
    }

    FILE *fpTxt = fopen("exam_routine.txt", "w");
    FILE *fpDat = fopen("exam_schedule.dat", "wb"); // binary file

    if (!fpTxt || !fpDat) {
        printf("Error opening exam routine files.\n");
        if (fpTxt) fclose(fpTxt);
        if (fpDat) fclose(fpDat);
        return;
    }

    fprintf(fpTxt, "📅 Exam Routine\n\n");

    for (int b = 0; b < numBatches; b++) {
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

                // Prepare schedule struct
                Schedule s;
                memset(&s, 0, sizeof(Schedule));

                strncpy(s.courseCode, course->code, MAX_COURSE_CODE_LEN - 1);
                s.courseCode[MAX_COURSE_CODE_LEN - 1] = '\0';

                strncpy(s.batch, batches[b].name, MAX_BATCH_NAME_LEN - 1);
                s.batch[MAX_BATCH_NAME_LEN - 1] = '\0';

                strncpy(s.day, "ExamDay", MAX_DAY_LEN - 1);
                s.day[MAX_DAY_LEN - 1] = '\0';

                strncpy(s.startTime, "09:00", MAX_TIME_LEN - 1);
                s.startTime[MAX_TIME_LEN - 1] = '\0';

                strncpy(s.roomNo, "ExamRoom", MAX_ROOM_NO_LEN - 1);
                s.roomNo[MAX_ROOM_NO_LEN - 1] = '\0';

                strncpy(s.teacher, course->teacherName[0] ? course->teacherName : "TBD", MAX_TEACHER_NAME_LEN - 1);
                s.teacher[MAX_TEACHER_NAME_LEN - 1] = '\0';

                s.isExam = 1;
                s.isLab = course->isLab;
                s.duration = 3; // Dummy duration

                fwrite(&s, sizeof(Schedule), 1, fpDat);
            }
        }
        fprintf(fpTxt, "-----------------------------------------\n");
    }

    fclose(fpTxt);
    fclose(fpDat);

    printf("✅ Exam routine generated successfully in exam_routine.txt and exam_schedule.dat\n");
}
