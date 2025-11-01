#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "input.h"
#include "assign.h"
#include "conflict.h"
#include "scheduler.h"
#include "exam_scheduler.h"
#include "viewer.h"
#include "save_txt.h"

// ------------------- PAUSE -------------------
void pauseConsole()
{
    printf("\nPress Enter to continue...");
    getchar();
}

// ==================== LOADERS ====================
void loadTeachers()
{
    FILE *fp = fopen("teachers.txt", "r");
    numTeachers = 0;
    if (!fp)
        return;

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        Teacher *t = &teachers[numTeachers];
        sscanf(line, "Name:%[^,], Dept:%[^,], ID:%[^,], Position:%[^,], MaxCourses:%d, MaxCredits:%d, Phone:%[^,], Email:%s",
               t->name, t->department, t->teacherID, t->position,
               &t->maxCourses, &t->maxCredits, t->phone, t->email);
        t->currentCourses = 0;
        t->currentCredits = 0;
        numTeachers++;
        if (numTeachers >= MAX_TEACHERS)
            break;
    }

    fclose(fp);
}

void loadCourses()
{
    FILE *fp = fopen("courses.txt", "r");
    numCourses = 0;
    if (!fp)
        return;

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        Course *c = &courses[numCourses];
        char lab[8];
        sscanf(line, "Code:%[^,], Name:%[^,], Credit:%d, IsLab:%[^,], Teacher:%[^\n]",
               c->code, c->name, &c->credit, lab, c->teacherName);
        c->isLab = (strcmp(lab, "Yes") == 0) ? 1 : 0;
        numCourses++;
        if (numCourses >= MAX_COURSES)
            break;
    }

    fclose(fp);
}

void loadBatches()
{
    FILE *fp = fopen("batches.txt", "r");
    numBatches = 0;
    if (!fp)
        return;

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        if (numBatches >= MAX_BATCHES)
            break;

        Batch *b = &batches[numBatches];
        char coursesStr[256] = {0};
        b->courseCount = 0;

        int readCount = sscanf(line, "Name:%[^,], CourseCount:%d, Courses:%[^\n]",
                               b->name, &b->courseCount, coursesStr);

        if (readCount < 2)
            continue;

        if (readCount == 3 && coursesStr[0] != '\0')
        {
            char *token = strtok(coursesStr, " ");
            int i = 0;
            while (token && i < MAX_BATCH_COURSES)
            {
                strcpy(b->courseCodes[i], token);
                i++;
                token = strtok(NULL, " ");
            }
            b->courseCount = i;
        }
        else
        {
            b->courseCount = 0;
        }

        numBatches++;
    }

    fclose(fp);
}

void loadRooms() {
    FILE *fp = fopen("rooms.txt", "r");
    numRooms = 0;
    if (!fp) return;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        if (numRooms >= MAX_ROOMS) break;
        Room *r = &rooms[numRooms];

        char lab[4], equipmentsStr[256] = {0}, daysStr[256] = {0}, timesStr[256] = {0};

        sscanf(line, "RoomNo:%[^,], IsLabRoom:%[^,], Capacity:%d, Equipments:%[^,], Days:%[^,], Times:%[^\n]",
               r->roomNo, lab, &r->capacity, equipmentsStr, daysStr, timesStr);
        r->isLabRoom = (strcmp(lab, "Yes") == 0) ? 1 : 0;

        // Equipments
        r->equipmentCount = 0;
        char *token = strtok(equipmentsStr, " ");
        while (token && r->equipmentCount < MAX_EQUIPMENTS) {
            strcpy(r->equipments[r->equipmentCount++], token);
            token = strtok(NULL, " ");
        }

        // Days
        r->availableDayCount = 0;
        token = strtok(daysStr, " ");
        while (token && r->availableDayCount < MAX_DAYS) {
            strcpy(r->availableDays[r->availableDayCount++], token);
            token = strtok(NULL, " ");
        }

        // Times
        r->availableTimeCount = 0;
        token = strtok(timesStr, " ");
        while (token && r->availableTimeCount < MAX_TIMESLOTS) {
            strcpy(r->availableTimes[r->availableTimeCount++], token);
            token = strtok(NULL, " ");
        }

        numRooms++;
    }

    fclose(fp);
}
// ==================== SAVERS ====================
void saveTeachers()
{
    FILE *fp = fopen("teachers.txt", "w");
    if (!fp)
        return;
    for (int i = 0; i < numTeachers; i++)
    {
        Teacher *t = &teachers[i];
        fprintf(fp, "Name:%s, Dept:%s, ID:%s, Position:%s, MaxCourses:%d, MaxCredits:%d, Phone:%s, Email:%s\n",
                t->name, t->department, t->teacherID, t->position,
                t->maxCourses, t->maxCredits, t->phone, t->email);
    }
    fclose(fp);
}

void saveCourses()
{
    FILE *fp = fopen("courses.txt", "w");
    if (!fp)
        return;
    for (int i = 0; i < numCourses; i++)
    {
        Course *c = &courses[i];
        fprintf(fp, "Code:%s, Name:%s, Credit:%d, IsLab:%s, Teacher:%s\n",
                c->code, c->name, c->credit, c->isLab ? "Yes" : "No",
                c->teacherName);
    }
    fclose(fp);
}

void saveBatches()
{
    FILE *fp = fopen("batches.txt", "w");
    if (!fp)
        return;
    for (int i = 0; i < numBatches; i++)
    {
        Batch *b = &batches[i];
        fprintf(fp, "Name:%s, CourseCount:%d, Courses:", b->name, b->courseCount);
        for (int j = 0; j < b->courseCount; j++)
            fprintf(fp, "%s ", b->courseCodes[j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void saveRooms() {
    FILE *fp = fopen("rooms.txt", "w");
    if (!fp) return;

    for (int i = 0; i < numRooms; i++) {
        Room *r = &rooms[i];
        fprintf(fp, "RoomNo:%s, IsLabRoom:%s, Capacity:%d, Equipments:",
                r->roomNo, r->isLabRoom ? "Yes" : "No", r->capacity);
        for (int j = 0; j < r->equipmentCount; j++) fprintf(fp, "%s ", r->equipments[j]);
        fprintf(fp, ", Days:");
        for (int j = 0; j < r->availableDayCount; j++) fprintf(fp, "%s ", r->availableDays[j]);
        fprintf(fp, ", Times:");
        for (int j = 0; j < r->availableTimeCount; j++) fprintf(fp, "%s ", r->availableTimes[j]);
        fprintf(fp, "\n");
    }

    fclose(fp);
}
// ==================== LOAD / SAVE ALL ====================
void loadAllData()
{
    loadTeachers();
    loadCourses();
    loadBatches();
    loadRooms();
}

void saveAllData()
{
    saveTeachers();
    saveCourses();
    saveBatches();
    saveRooms();
}

// ==================== MAIN ====================
int main()
{
    loadAllData();

    printf("\n ========================================= \n");
    printf("\n Welcome to Academic Scheduler! \n");
    
  

    int choice;

    while (1)
    {
       
        printf("1. Teachers Menu\n");
        printf("2. Courses Menu\n");
        printf("3. Batches Menu\n");
        printf("4. Rooms Menu\n");
        printf("5. Assign Teacher to Courses\n");
        printf("6. Assign Courses to Batches\n");
        printf("7. Generate Class Routine\n");
        printf("8. Detect Conflicts\n");
        printf("9. Generate Exam Routine\n");
        printf("10. View Section\n");
        printf("0. Exit\n");
        printf("==============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            char sub;
            printf("a. Add Teacher\nb. Delete Teacher\nc. Update Teacher\nChoose: ");
            scanf(" %c", &sub);
            getchar();
            if (sub == 'a' || sub == 'A')
                addTeacher();
            else if (sub == 'b' || sub == 'B')
                deleteTeacher();
            else if (sub == 'c' || sub == 'C')
                updateTeacher();
            else
                printf("Invalid choice!\n");
            saveTeachers();
            break;
        }
        case 2:
        {
            char sub;
            printf("a. Add Course\nb. Edit Course\nc. Delete Course\nd. View Courses\nChoose: ");
            scanf(" %c", &sub);
            getchar();
            if (sub == 'a' || sub == 'A')
                addCourse();
            else if (sub == 'b' || sub == 'B')
                editCourse();
            else if (sub == 'c' || sub == 'C')
                deleteCourse();
            else if (sub == 'd' || sub == 'D')
                viewCourses();
            else
                printf("Invalid choice!\n");
            saveCourses();
            break;
        }
        case 3:
        {
            char sub;
            printf("a. Add Batch\nb. Edit Batch\nc. Delete Batch\nd. View Batches\nChoose: ");
            scanf(" %c", &sub);
            getchar();
            if (sub == 'a' || sub == 'A')
                addBatch();
            else if (sub == 'b' || sub == 'B')
                editBatch();
            else if (sub == 'c' || sub == 'C')
                deleteBatch();
            else if (sub == 'd' || sub == 'D')
                viewBatches();
            else
                printf("Invalid choice!\n");
            saveBatches();
            break;
        }
        case 4:
        {
            char sub;
            printf("a. Add Room\nb. Edit Room\nc. Delete Room\nd. View Rooms\nChoose: ");
            scanf(" %c", &sub);
            getchar();
            if (sub == 'a' || sub == 'A')
                addRoom();
            else if (sub == 'b' || sub == 'B')
                editRoom();
            else if (sub == 'c' || sub == 'C')
                deleteRoom();
            else if (sub == 'd' || sub == 'D')
                viewRooms();
            else
                printf("Invalid choice!\n");
            saveRooms();
            break;
        }
        case 5:
            assignTeacherToCourse();
            saveTeachers();
            break;
        case 6:
            assignCoursesToBatch();
            saveBatches();
            break;
        case 7:
            generateClassRoutine();
            break;
        case 8:
        {
            int conflictChoice;
            printf("\nSelect conflict type to detect:\n");
            printf("1. Class routine conflicts\n");
            printf("2. Exam routine conflicts\n");
            printf("Enter choice: ");
            scanf("%d", &conflictChoice);
            getchar();

            switch (conflictChoice)
            {
            case 1:
                detectClassConflicts(); // fixed function name
                break;
            case 2:
                detectExamConflicts();
                break;
            default:
                printf("Invalid choice.\n");
            }
            break;
        }
        case 9:
            generateExamRoutine();
            break;
        case 10:
            {
    int viewChoice;
    printf("\nSelect view option:\n");
    printf("1. Teachers\n2. Courses\n3. Batches\n4. Rooms\n5. Teacher Assignments\n6. Batch Assignments\n");
    printf("Enter choice: ");
    scanf("%d", &viewChoice);
    getchar();

    switch (viewChoice) {
        case 1: viewTeachers(); break;
        case 2: viewCourses(); break;
        case 3: viewBatches(); break;
        case 4: viewRooms(); break;
        case 5: viewTeacherAssignments(); break;
        case 6: viewBatchAssignments(); break;
        default: printf("Invalid choice.\n");
    }
    break;
}
        case 0:
            saveAllData();
            printf("Data saved. Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice! Enter 0-10.\n");
        }

        pauseConsole();
    }

    return 0;
}
