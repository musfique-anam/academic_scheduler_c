// scheduler.c
#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "globals.h"
#include "structures.h"

// Function to generate class routine
void generateClassRoutine() {
    if (numBatches == 0 || numCourses == 0 || numTeachers == 0 || numRooms == 0) {
        printf("Cannot generate class routine. Make sure batches, courses, teachers, and rooms exist.\n");
        return;
    }

    FILE *fp = fopen("routine.txt", "w");
    FILE *fpDat = fopen("schedule.dat", "wb");

    if (!fp || !fpDat) {
        printf("Error opening routine files.\n");
        if (fp) fclose(fp);
        if (fpDat) fclose(fpDat);
        return;
    }

    fprintf(fp, "📅 Class Routine\n\n");

    for (int b = 0; b < numBatches; b++) {
        fprintf(fp, "Batch: %s\n", batches[b].name);

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

            if (!course) continue;

            // Assign teacher
            Teacher *teacher = NULL;
            for (int t = 0; t < numTeachers; t++) {
                if (strcmp(teachers[t].name, course->teacherName) == 0) {
                    teacher = &teachers[t];
                    break;
                }
            }

            // Assign room
            Room *room = NULL;
            for (int r = 0; r < numRooms; r++) {
                if (course->isLab && rooms[r].isLabRoom) {
                    room = &rooms[r];
                    break;
                } else if (!course->isLab && !rooms[r].isLabRoom) {
                    room = &rooms[r];
                    break;
                }
            }

            // Generate schedule (dummy example: day and time assigned sequentially)
            Schedule s;
            memset(&s, 0, sizeof(Schedule));

            strncpy(s.courseCode, course->code, MAX_COURSE_CODE_LEN - 1);
            s.courseCode[MAX_COURSE_CODE_LEN - 1] = '\0';

            strncpy(s.batch, batches[b].name, MAX_BATCH_NAME_LEN - 1);
            s.batch[MAX_BATCH_NAME_LEN - 1] = '\0';

            strncpy(s.day, "Saturday", MAX_DAY_LEN - 1); // For now, assign Saturday
            s.day[MAX_DAY_LEN - 1] = '\0';

            strncpy(s.startTime, "10:00", MAX_TIME_LEN - 1); // Dummy time
            s.startTime[MAX_TIME_LEN - 1] = '\0';

            if (room)
                strncpy(s.roomNo, room->roomNo, MAX_ROOM_NO_LEN - 1);
            else
                strncpy(s.roomNo, "TBD", MAX_ROOM_NO_LEN - 1);
            s.roomNo[MAX_ROOM_NO_LEN - 1] = '\0';

            if (teacher)
                strncpy(s.teacher, teacher->name, MAX_TEACHER_NAME_LEN - 1);
            else
                strncpy(s.teacher, "TBD", MAX_TEACHER_NAME_LEN - 1);
            s.teacher[MAX_TEACHER_NAME_LEN - 1] = '\0';

            s.isExam = 0;
            s.isLab = course->isLab;
            s.duration = course->credit; // Assume 1 credit = 1.5 hours (you can adjust)

            // Write to text file
            fprintf(fp, "-----------------------------------------\n");
            fprintf(fp, "Course: %s (%s)\nBatch: %s\nTeacher: %s\nRoom: %s\nDay: %s\nTime: %s\nDuration: %d hours\n",
                    course->name, course->code, s.batch, s.teacher, s.roomNo, s.day, s.startTime, s.duration);

            // Write to binary file
            fwrite(&s, sizeof(Schedule), 1, fpDat);
        }
        fprintf(fp, "-----------------------------------------\n");
    }

    fclose(fp);
    fclose(fpDat);

    printf("✅ Class routine generated successfully in routine.txt and schedule.dat\n");
}

// Additional functions can go here if needed
