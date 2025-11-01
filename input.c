#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "structures.h"
#include "globals.h"
#include "save_txt.h"

#define MAX_SLOTS 4

// ------------------ TEACHER FUNCTIONS ------------------ //

void addTeacher() {
    Teacher t;
    getchar(); // clear newline

    printf("Enter teacher name: ");
    fgets(t.name, sizeof(t.name), stdin);
    t.name[strcspn(t.name, "\n")] = 0;

    printf("Enter department: ");
    fgets(t.department, sizeof(t.department), stdin);
    t.department[strcspn(t.department, "\n")] = 0;

    printf("Enter teacher ID: ");
    fgets(t.teacherID, sizeof(t.teacherID), stdin);
    t.teacherID[strcspn(t.teacherID, "\n")] = 0;

    printf("Enter position: ");
    fgets(t.position, sizeof(t.position), stdin);
    t.position[strcspn(t.position, "\n")] = 0;

    // Available days
    printf("Select available days (0=Friday ... 4=Tuesday) separated by space: ");
    for (int i = 0; i < MAX_DAYS; i++) t.availableDays[i] = 0;
    char line[200];
    fgets(line, sizeof(line), stdin);
    char *token = strtok(line, " ");
    while (token) {
        int day = atoi(token);
        if (day >= 0 && day < MAX_DAYS) t.availableDays[day] = 1;
        token = strtok(NULL, " ");
    }

    // Available time slots
    printf("Select available time slots (1=9:30-11,2=11:10-12:40,3=2:00-3:30,4=3:40-5:00) separated by space: ");
    for (int i = 0; i < MAX_SLOTS; i++) t.availableSlots[i] = 0;
    fgets(line, sizeof(line), stdin);
    token = strtok(line, " ");
    while (token) {
        int slot = atoi(token);
        if (slot >= 1 && slot <= MAX_SLOTS) t.availableSlots[slot - 1] = 1;
        token = strtok(NULL, " ");
    }

    printf("Enter maximum number of courses: ");
    scanf("%d", &t.maxCourses);
    if (t.maxCourses > MAX_COURSES_PER_TEACHER) t.maxCourses = MAX_COURSES_PER_TEACHER;
    getchar();

    for (int i = 0; i < t.maxCourses; i++) {
        printf("Enter course code %d: ", i + 1);
        fgets(t.courseCodes[i], sizeof(t.courseCodes[i]), stdin);
        t.courseCodes[i][strcspn(t.courseCodes[i], "\n")] = 0;
    }

    printf("Enter maximum credits: ");
    scanf("%d", &t.maxCredits);
    getchar();

    printf("Enter phone number: ");
    fgets(t.phone, sizeof(t.phone), stdin);
    t.phone[strcspn(t.phone, "\n")] = 0;

    printf("Enter email: ");
    fgets(t.email, sizeof(t.email), stdin);
    t.email[strcspn(t.email, "\n")] = 0;

    t.slotCount = 0;
    t.currentCourses = 0;

    FILE *f = fopen("teachers.dat", "ab");
    if (f) { fwrite(&t, sizeof(Teacher), 1, f); fclose(f); }

    if (numTeachers < MAX_TEACHERS) teachers[numTeachers++] = t;

    saveTeachersToTxt();
    printf("✅ Teacher added successfully! Total teachers: %d\n", numTeachers);
}

void deleteTeacher() {
    if (numTeachers == 0) { printf("No teachers available to delete.\n"); return; }

    char id[20];
    printf("Enter Teacher ID to delete: ");
    getchar();
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < numTeachers; i++) {
        if (strcmp(teachers[i].teacherID, id) == 0) { index = i; break; }
    }
    if (index == -1) { printf("Teacher ID not found.\n"); return; }

    for (int i = index; i < numTeachers - 1; i++) teachers[i] = teachers[i + 1];
    numTeachers--;

    FILE *f = fopen("teachers.dat", "wb");
    if (f) { fwrite(teachers, sizeof(Teacher), numTeachers, f); fclose(f); }

    saveTeachersToTxt();
    printf("✅ Teacher with ID %s deleted successfully.\n", id);
}

void updateTeacher() {
    if (numTeachers == 0) { printf("No teachers available to update.\n"); return; }

    char id[20];
    printf("Enter Teacher ID to update: ");
    getchar();
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < numTeachers; i++) {
        if (strcmp(teachers[i].teacherID, id) == 0) { index = i; break; }
    }
    if (index == -1) { printf("Teacher ID not found.\n"); return; }

    Teacher *t = &teachers[index];
    char line[200];
    printf("Updating teacher: %s\n", t->name);

    printf("Enter new name (or Enter to keep '%s'): ", t->name);
    fgets(line, sizeof(line), stdin);
    if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(t->name, line); }

    printf("Enter new department (or Enter to keep '%s'): ", t->department);
    fgets(line, sizeof(line), stdin);
    if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(t->department, line); }

    printf("Enter new position (or Enter to keep '%s'): ", t->position);
    fgets(line, sizeof(line), stdin);
    if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(t->position, line); }

    FILE *f = fopen("teachers.dat", "wb");
    if (f) { fwrite(teachers, sizeof(Teacher), numTeachers, f); fclose(f); }

    saveTeachersToTxt();
    printf("✅ Teacher updated successfully.\n");
}

// ------------------ COURSE FUNCTIONS ------------------ //

void addCourse() {
    Course c;
    getchar(); // clear newline

    printf("Enter course code: "); scanf(" %9s", c.code); getchar();
    printf("Enter course name: "); fgets(c.name, sizeof(c.name), stdin); c.name[strcspn(c.name, "\n")] = 0;
    printf("Enter credit hours: "); scanf("%d", &c.credit);
    printf("Is this a lab course? (1 = Yes, 0 = No): "); scanf("%d", &c.isLab); getchar();
    printf("Enter preferred teacher name: "); fgets(c.teacherName, sizeof(c.teacherName), stdin); c.teacherName[strcspn(c.teacherName, "\n")] = 0;

    FILE *fp = fopen("courses.dat", "ab");
    if (fp) { fwrite(&c, sizeof(Course), 1, fp); fclose(fp); }

    if (numCourses < MAX_COURSES) courses[numCourses++] = c;

    saveCoursesToTxt();
    printf("✅ Course added successfully! Total courses: %d\n", numCourses);
}

void editCourse() {
    if (numCourses == 0) { printf("No courses available to edit.\n"); return; }

    char code[10];
    printf("Enter course code to edit: "); scanf(" %9s", code); getchar();

    int idx = -1;
    for (int i = 0; i < numCourses; i++)
        if (strcmp(courses[i].code, code) == 0) { idx = i; break; }

    if (idx == -1) { printf("Course not found.\n"); return; }

    Course *c = &courses[idx];
    char line[100];
    printf("Editing course: %s (%s)\n", c->name, c->code);

    printf("Enter new name (or Enter to keep '%s'): ", c->name); fgets(line, sizeof(line), stdin);
    if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(c->name, line); }

    printf("Enter new credit hours (current %d, 0 to keep): ", c->credit);
    int credit; scanf("%d", &credit); getchar(); if (credit > 0) c->credit = credit;

    printf("Is this a lab course? (1=Yes,0=No, -1 to keep current %d): ", c->isLab);
    int lab; scanf("%d", &lab); getchar(); if (lab == 0 || lab == 1) c->isLab = lab;

    printf("Enter preferred teacher name (or Enter to keep '%s'): ", c->teacherName[0] ? c->teacherName : "None");
    fgets(line, sizeof(line), stdin); if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(c->teacherName, line); }

    FILE *fp = fopen("courses.dat", "wb"); 
    if (fp) { fwrite(courses, sizeof(Course), numCourses, fp); fclose(fp); }

    saveCoursesToTxt();
    printf("✅ Course updated successfully.\n");
}

void deleteCourse() {
    if (numCourses == 0) { printf("No courses available to delete.\n"); return; }

    char code[10];
    printf("Enter course code to delete: "); scanf(" %9s", code); getchar();

    int idx = -1;
    for (int i = 0; i < numCourses; i++)
        if (strcmp(courses[i].code, code) == 0) { idx = i; break; }

    if (idx == -1) { printf("Course not found.\n"); return; }

    for (int i = idx; i < numCourses - 1; i++) courses[i] = courses[i + 1];
    numCourses--;

    FILE *fp = fopen("courses.dat", "wb"); 
    if (fp) { fwrite(courses, sizeof(Course), numCourses, fp); fclose(fp); }

    saveCoursesToTxt();
    printf("✅ Course deleted successfully.\n");
}



// ------------------ BATCH FUNCTIONS ------------------ //

void addBatch() {
    Batch b;
    printf("Enter batch name: "); getchar(); fgets(b.name, sizeof(b.name), stdin); b.name[strcspn(b.name, "\n")] = 0;

    printf("Enter number of courses: "); scanf("%d", &b.courseCount);
    if (b.courseCount > MAX_COURSES_PER_BATCH) b.courseCount = MAX_COURSES_PER_BATCH;
    for (int i = 0; i < b.courseCount; i++) { printf("Enter course code %d: ", i + 1); scanf(" %9s", b.courseCodes[i]); }

    FILE *fp = fopen("batches.dat", "ab");
    if (fp) { fwrite(&b, sizeof(Batch), 1, fp); fclose(fp); }

    if (numBatches < MAX_BATCHES) batches[numBatches++] = b;

    saveBatchesToTxt();
    printf("✅ Batch added successfully! Total batches: %d\n", numBatches);
}

void editBatch() {
    if (numBatches == 0) { printf("No batches available to edit.\n"); return; }

    char name[50];
    printf("Enter batch name to edit: "); getchar(); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < numBatches; i++)
        if (strcmp(batches[i].name, name) == 0) { index = i; break; }

    if (index == -1) { printf("Batch not found.\n"); return; }

    Batch *b = &batches[index];
    char line[200];

    printf("Enter new batch name (or Enter to keep '%s'): ", b->name);
    fgets(line, sizeof(line), stdin);
    if (line[0] != '\n') { line[strcspn(line, "\n")] = 0; strcpy(b->name, line); }

    printf("Update courses? (y/n): ");
    char ch = getchar(); getchar();
    if (ch == 'y' || ch == 'Y') {
        printf("Enter number of courses: "); scanf("%d", &b->courseCount); getchar();
        if (b->courseCount > MAX_COURSES_PER_BATCH) b->courseCount = MAX_COURSES_PER_BATCH;

        for (int i = 0; i < b->courseCount; i++) { printf("Enter course code %d: ", i + 1); scanf(" %9s", b->courseCodes[i]); }
    }

    FILE *fp = fopen("batches.dat", "wb"); 
    if (fp) { fwrite(batches, sizeof(Batch), numBatches, fp); fclose(fp); }

    saveBatchesToTxt();
    printf("✅ Batch updated successfully.\n");
}

void deleteBatch() {
    if (numBatches == 0) { printf("No batches available to delete.\n"); return; }

    char name[50];
    printf("Enter batch name to delete: "); getchar(); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;

    int index = -1;
    for (int i = 0; i < numBatches; i++)
        if (strcmp(batches[i].name, name) == 0) { index = i; break; }

    if (index == -1) { printf("Batch not found.\n"); return; }

    for (int i = index; i < numBatches - 1; i++) batches[i] = batches[i + 1];
    numBatches--;

    FILE *fp = fopen("batches.dat", "wb"); 
    if (fp) { fwrite(batches, sizeof(Batch), numBatches, fp); fclose(fp); }

    saveBatchesToTxt();
    printf("✅ Batch '%s' deleted successfully.\n", name);
}



// ------------------ ROOM FUNCTIONS ------------------ //

// Predefined days and time slots
const char *DAYS[MAX_DAYS] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
const char *TIME_SLOTS[] = {"09:30-11:00", "11:10-13:00", "14:00-15:30", "15:30-17:00"};

// ---------------- ADD ROOM ---------------- //
void addRoom() {
    if (numRooms >= MAX_ROOMS) {
        printf("Room limit reached!\n");
        return;
    }

    Room *r = &rooms[numRooms];
    printf("Enter Room Number: ");
    scanf("%s", r->roomNo);

    printf("Is Lab Room? (1=Yes, 0=No): ");
    scanf("%d", &r->isLabRoom);

    printf("Enter Room Capacity: ");
    scanf("%d", &r->capacity);

    // Equipments
    r->equipmentCount = 0;
    char choice;
    do {
        printf("Enter equipment name: ");
        scanf("%s", r->equipments[r->equipmentCount++]);
        if (r->equipmentCount >= MAX_EQUIPMENTS) break;
        printf("Add another equipment? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    // Available Days
    r->availableDayCount = 0;
    printf("Select available days (1=Saturday ... 7=Friday), enter 0 to finish:\n");
    int day;
    while (1) {
        scanf("%d", &day);
        if (day == 0) break;
        if (day >= 1 && day <= MAX_DAYS) {
            strcpy(r->availableDays[r->availableDayCount++], DAYS[day-1]);
        }
    }

    // Available Time Slots
    r->availableTimeCount = 0;
    printf("Select available time slots (1=09:30-11:00, 2=11:10-13:00, 3=14:00-15:30, 4=15:30-17:00), enter 0 to finish:\n");
    int slot;
    while (1) {
        scanf("%d", &slot);
        if (slot == 0) break;
        if (slot >= 1 && slot <= 4) {
            strcpy(r->availableTimes[r->availableTimeCount++], TIME_SLOTS[slot-1]);
        }
    }

    numRooms++;
    printf("✅ Room added successfully.\n");
}



// ---------------- DELETE ROOM ---------------- //
void deleteRoom() {
    char roomNo[20];
    printf("Enter room number to delete: ");
    scanf("%s", roomNo);

    int found = 0;
    for (int i = 0; i < numRooms; i++) {
        if (strcmp(rooms[i].roomNo, roomNo) == 0) {
            for (int j = i; j < numRooms - 1; j++)
                rooms[j] = rooms[j + 1];
            numRooms--;
            printf("✅ Room deleted successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Room not found.\n");
}


// ---------------- EDIT ROOM ---------------- //
void editRoom() {
    char roomNo[20];
    printf("Enter room number to edit: ");
    scanf("%s", roomNo);

    int found = 0;
    for (int i = 0; i < numRooms; i++) {
        if (strcmp(rooms[i].roomNo, roomNo) == 0) {
            Room *r = &rooms[i];
            found = 1;

            printf("Editing Room %s\n", r->roomNo);

            printf("Is Lab Room? (1=Yes, 0=No): ");
            scanf("%d", &r->isLabRoom);

            printf("Enter Room Capacity: ");
            scanf("%d", &r->capacity);

            // Equipments
            r->equipmentCount = 0;
            char choice;
            do {
                printf("Enter equipment name: ");
                scanf("%s", r->equipments[r->equipmentCount++]);
                if (r->equipmentCount >= MAX_EQUIPMENTS) break;
                printf("Add another equipment? (y/n): ");
                scanf(" %c", &choice);
            } while (choice == 'y' || choice == 'Y');

            // Available Days
            r->availableDayCount = 0;
            printf("Select available days (1=Saturday ... 7=Friday), enter 0 to finish:\n");
            int day;
            while (1) {
                scanf("%d", &day);
                if (day == 0) break;
                if (day >= 1 && day <= MAX_DAYS) {
                    strcpy(r->availableDays[r->availableDayCount++], DAYS[day-1]);
                }
            }

            // Available Time Slots
            r->availableTimeCount = 0;
            printf("Select available time slots (1=09:30-11:00, 2=11:10-13:00, 3=14:00-15:30, 4=15:30-17:00), enter 0 to finish:\n");
            int slot;
            while (1) {
                scanf("%d", &slot);
                if (slot == 0) break;
                if (slot >= 1 && slot <= 4) {
                    strcpy(r->availableTimes[r->availableTimeCount++], TIME_SLOTS[slot-1]);
                }
            }

            printf("✅ Room updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Room not found.\n");
    }
}
