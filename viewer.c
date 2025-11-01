// viewer.c
#include <stdio.h>
#include <string.h>
#include "viewer.h"
#include "globals.h"
#include "structures.h"
#include "input.h"  // for update/delete functions

// ---------------- TEACHERS MENU ---------------- //
void viewTeachersMenu() {
    int choice;
    do {
        printf("\n👩‍🏫 Teachers Menu\n");
        printf("1. View all teachers\n");
        printf("2. Update teacher\n");
        printf("3. Delete teacher\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch(choice) {
            case 1:
                if (numTeachers == 0) { printf("No teachers available.\n"); break; }
                for (int i=0;i<numTeachers;i++) {
                    printf("-----------------------------------------\n");
                    printf("Name: %s\nID: %s\nDepartment: %s\nPosition: %s\n",
                        teachers[i].name, teachers[i].teacherID, teachers[i].department, teachers[i].position);
                }
                printf("-----------------------------------------\n");
                break;
            case 2:
                updateTeacher();
                break;
            case 3:
                deleteTeacher();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

// ---------------- COURSES MENU ---------------- //
void viewCoursesMenu() {
    int choice;
    do {
        printf("\n📚 Courses Menu\n");
        printf("1. View all courses\n");
        printf("2. Update course\n");
        printf("3. Delete course\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                if (numCourses == 0) { printf("No courses available.\n"); break; }
                for (int i=0;i<numCourses;i++) {
                    printf("-----------------------------------------\n");
                    printf("Code: %s\nName: %s\nCredit: %d\nLab: %s\nPreferred Teacher: %s\n",
                        courses[i].code, courses[i].name, courses[i].credit,
                        courses[i].isLab ? "Yes":"No", courses[i].teacherName[0] ? courses[i].teacherName : "None");
                }
                printf("-----------------------------------------\n");
                break;
            case 2:
                editCourse();
                break;
            case 3:
                deleteCourse();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 0);
}

// ---------------- BATCHES MENU ---------------- //
void viewBatchesMenu() {
    int choice;
    do {
        printf("\n📦 Batches Menu\n");
        printf("1. View all batches\n");
        printf("2. Update batch\n");
        printf("3. Delete batch\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                if (numBatches==0) { printf("No batches available.\n"); break; }
                for (int i=0;i<numBatches;i++) {
                    printf("-----------------------------------------\n");
                    printf("Batch: %s\nCourses: ", batches[i].name);
                    if (batches[i].courseCount==0) printf("None");
                    else for (int j=0;j<batches[i].courseCount && j<MAX_COURSES_PER_BATCH;j++)
                        printf("%s ", batches[i].courseCodes[j]);
                    printf("\n-----------------------------------------\n");
                }
                break;
            case 2:
                editBatch();
                break;
            case 3:
                deleteBatch();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice!=0);
}

// ---------------- ROOMS MENU ---------------- //
void viewRoomsMenu() {
    int choice;
    do {
        printf("\n🏫 Rooms Menu\n");
        printf("1. View all rooms\n");
        printf("2. Update room\n");
        printf("3. Delete room\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                if (numRooms==0) { printf("No rooms available.\n"); break; }
                for(int i=0;i<numRooms;i++){
                    Room *r = &rooms[i];
                    printf("-----------------------------------------\n");
                    printf("Room No: %s\nLab Room: %s\nCapacity: %d\nEquipments: ",
                        r->roomNo, r->isLabRoom?"Yes":"No", r->capacity);
                    for(int e=0;e<r->equipmentCount;e++){
                        printf("%s", r->equipments[e]);
                        if(e<r->equipmentCount-1) printf(", ");
                    }
                    printf("\nAvailable Days: ");
                    for(int d=0;d<r->availableDayCount;d++){
                        printf("%s", r->availableDays[d]);
                        if(d<r->availableDayCount-1) printf(", ");
                    }
                    printf("\nTime Slots: ");
                    for(int t=0;t<r->availableTimeCount;t++){
                        printf("%s", r->availableTimes[t]);
                        if(t<r->availableTimeCount-1) printf(", ");
                    }
                    printf("\n-----------------------------------------\n");
                }
                break;
            case 2:
                editRoom();
                break;
            case 3:
                deleteRoom();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice!=0);
}

// ---------------- TEACHER ASSIGNMENTS MENU ---------------- //
void viewTeacherAssignmentsMenu() {
    int choice;
    do {
        printf("\n📝 Teacher Assignments Menu\n");
        printf("1. View assignments\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                if(numTeachers==0) { printf("No teachers available.\n"); break; }
                for(int i=0;i<numTeachers;i++){
                    printf("-----------------------------------------\n");
                    printf("Teacher: %s (%s)\nAssigned Courses: ", teachers[i].name, teachers[i].teacherID);
                    int count=0;
                    for(int j=0;j<numCourses;j++){
                        if(strcmp(courses[j].teacherName, teachers[i].teacherID)==0 ||
                           strcmp(courses[j].teacherName, teachers[i].name)==0){
                            printf("%s ", courses[j].code);
                            count++;
                        }
                    }
                    if(count==0) printf("None");
                    printf("\n-----------------------------------------\n");
                }
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice!=0);
}

// ---------------- BATCH ASSIGNMENTS MENU ---------------- //
void viewBatchAssignmentsMenu() {
    int choice;
    do {
        printf("\n📋 Batch Assignments Menu\n");
        printf("1. View assignments\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                if(numBatches==0) { printf("No batches available.\n"); break; }
                for(int i=0;i<numBatches;i++){
                    printf("-----------------------------------------\n");
                    printf("Batch: %s\nCourses: ", batches[i].name);
                    if(batches[i].courseCount==0) printf("None");
                    else for(int j=0;j<batches[i].courseCount && j<MAX_COURSES_PER_BATCH;j++)
                        printf("%s ", batches[i].courseCodes[j]);
                    printf("\n-----------------------------------------\n");
                }
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice!=0);
}

// ---------------- WRAPPERS FOR MAIN.C ---------------- //
void viewCourses() { viewCoursesMenu(); }
void viewBatches() { viewBatchesMenu(); }
void viewRooms() { viewRoomsMenu(); }
void viewTeachers() { viewTeachersMenu(); }
void viewTeacherAssignments() { viewTeacherAssignmentsMenu(); }
void viewBatchAssignments() { viewBatchAssignmentsMenu(); }
