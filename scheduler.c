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

    // Table header
    fprintf(fp, "Day       Timeslot   Batch    Course          Teacher         Room\n");
    fprintf(fp, "---------------------------------------------------------------\n");

    const char *days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday"};
    const char *timeSlots[] = {"10:00-11:30", "11:30-13:00", "14:00-15:30", "15:30-17:00"};
    int numDays = 6;
    int numSlots = 4;
    int dayIdx = 0, slotIdx = 0;

    for (int b = 0; b < numBatches; b++) {
        for (int c = 0; c < batches[b].courseCount && c < MAX_COURSES_PER_BATCH; c++) {

            char *courseCode = batches[b].courseCodes[c];

            // Find course
            Course *course = NULL;
            for (int i = 0; i < numCourses; i++) {
                if (strcmp(courses[i].code, courseCode) == 0) {
                    course = &courses[i];
                    break;
                }
            }
            if (!course) continue;

            // Find teacher
            Teacher *teacher = NULL;
            for (int t = 0; t < numTeachers; t++) {
                if (strcmp(teachers[t].name, course->teacherName) == 0) {
                    teacher = &teachers[t];
                    break;
                }
            }

            // Find room
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

            // Fill schedule struct
            Schedule s;
            memset(&s, 0, sizeof(Schedule));
            strncpy(s.courseCode, course->code, MAX_COURSE_CODE_LEN - 1);
            strncpy(s.batch, batches[b].name, MAX_BATCH_NAME_LEN - 1);
            strncpy(s.day, days[dayIdx], MAX_DAY_LEN - 1);
            strncpy(s.startTime, timeSlots[slotIdx], MAX_TIME_LEN - 1);
            if (room) strncpy(s.roomNo, room->roomNo, MAX_ROOM_NO_LEN - 1);
            else strncpy(s.roomNo, "TBD", MAX_ROOM_NO_LEN - 1);
            if (teacher) strncpy(s.teacher, teacher->name, MAX_TEACHER_NAME_LEN - 1);
            else strncpy(s.teacher, "TBD", MAX_TEACHER_NAME_LEN - 1);
            s.isLab = course->isLab;
            s.duration = course->credit;

            // Write to text file in table format
            fprintf(fp, "%-10s %-10s %-8s %-15s %-15s %-5s\n",
                    s.day, s.startTime, s.batch, course->name, s.teacher, s.roomNo);

            // Write to binary file
            fwrite(&s, sizeof(Schedule), 1, fpDat);

            // Move to next slot/day
            slotIdx++;
            if (slotIdx >= numSlots) {
                slotIdx = 0;
                dayIdx = (dayIdx + 1) % numDays;
            }
        }
    }

    fclose(fp);
    fclose(fpDat);

    printf("✅ Class routine generated successfully in routine.txt and schedule.dat\n");
}

// Additional functions can go here if needed
