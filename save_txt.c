#include <stdio.h>
#include "structures.h"
#include "globals.h"

void saveTeachersToTxt() {
    FILE *fp = fopen("teachers.txt", "w");
    if (!fp) return;

    fprintf(fp, "================= TEACHERS LIST =================\n");
    for (int i = 0; i < numTeachers; i++) {
        fprintf(fp, "\nTeacher ID   : %s\n", teachers[i].teacherID);
        fprintf(fp, "Name         : %s\n", teachers[i].name);
        fprintf(fp, "Department   : %s\n", teachers[i].department);
        fprintf(fp, "Position     : %s\n", teachers[i].position);
        fprintf(fp, "-----------------------------------------------\n");
    }
    fclose(fp);
}

void saveCoursesToTxt() {
    FILE *fp = fopen("courses.txt", "w");
    if (!fp) return;

    fprintf(fp, "================= COURSES LIST ==================\n");
    for (int i = 0; i < numCourses; i++) {
        fprintf(fp, "\nCourse Code     : %s\n", courses[i].code);
        fprintf(fp, "Course Name     : %s\n", courses[i].name);
        fprintf(fp, "Credit Hours    : %d\n", courses[i].credit);
        fprintf(fp, "Lab Course      : %s\n", courses[i].isLab ? "Yes" : "No");
        fprintf(fp, "Preferred Teacher: %s\n", courses[i].teacherName[0] ? courses[i].teacherName : "None");
        fprintf(fp, "-----------------------------------------------\n");
    }
    fclose(fp);
}

void saveBatchesToTxt() {
    FILE *fp = fopen("batches.txt", "w");
    if (!fp) return;

    fprintf(fp, "================= BATCHES LIST ==================\n");
    for (int i = 0; i < numBatches; i++) {
        fprintf(fp, "\nBatch Name  : %s\n", batches[i].name);
        fprintf(fp, "Courses     : ");
        for (int j = 0; j < batches[i].courseCount; j++) {
            fprintf(fp, "%s", batches[i].courseCodes[j]);
            if (j < batches[i].courseCount - 1) fprintf(fp, ", ");
        }
        fprintf(fp, "\n-----------------------------------------------\n");
    }
    fclose(fp);
}

void saveRoomsToTxt() {
    FILE *fp = fopen("rooms.txt", "w");
    if (!fp) return;

    fprintf(fp, "================= ROOMS LIST ====================\n");
    for (int i = 0; i < numRooms; i++) {
        fprintf(fp, "\nRoom No   : %s\n", rooms[i].roomNo);
        fprintf(fp, "Lab Room  : %s\n", rooms[i].isLabRoom ? "Yes" : "No");
        fprintf(fp, "-----------------------------------------------\n");
    }
    fclose(fp);
}
