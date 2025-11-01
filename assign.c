#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "assign.h"
#include "globals.h"
#include "save_txt.h" // <-- Include this for saving .txt

void assignTeacherToCourse()
{
    if (numCourses == 0 || numTeachers == 0)
    {
        printf("No courses or teachers available to assign.\n");
        return;
    }

    printf("Available courses:\n");
    for (int i = 0; i < numCourses; i++)
        printf("%d. %s (%s)\n", i + 1, courses[i].name, courses[i].code);

    char courseCode[10];
    printf("Enter course code to assign teacher: ");
    scanf(" %9s", courseCode);
    getchar();

    int courseIdx = -1;
    for (int i = 0; i < numCourses; i++)
    {
        if (strcmp(courses[i].code, courseCode) == 0)
        {
            courseIdx = i;
            break;
        }
    }
    if (courseIdx == -1)
    {
        printf("Course not found.\n");
        return;
    }

    Course *c = &courses[courseIdx];
    int preferredTeacherIdx = -1;

    // Check if preferred teacher exists
    if (c->teacherName[0])
    {
        for (int i = 0; i < numTeachers; i++)
        {
            if (strcmp(teachers[i].name, c->teacherName) == 0)
            {
                preferredTeacherIdx = i;
                break;
            }
        }
    }

    int assignIdx = -1;

    if (preferredTeacherIdx != -1)
    {
        assignIdx = preferredTeacherIdx;
        printf("Preferred teacher '%s' assigned to course %s.\n", teachers[assignIdx].name, c->code);
    }
    else
    {
        // Show teacher list to choose
        printf("Available teachers:\n");
        for (int i = 0; i < numTeachers; i++)
        {
            printf("%d. %s (Dept: %s)\n", i + 1, teachers[i].name, teachers[i].department);
        }
        int tChoice;
        printf("Enter teacher number to assign: ");
        scanf("%d", &tChoice);
        getchar();

        if (tChoice < 1 || tChoice > numTeachers)
        {
            printf("Invalid choice.\n");
            return;
        }
        assignIdx = tChoice - 1;
    }

    strcpy(c->teacherName, teachers[assignIdx].name);

    // Save changes to .dat
    FILE *fp = fopen("courses.dat", "wb");
    if (fp)
    {
        fwrite(courses, sizeof(Course), numCourses, fp);
        fclose(fp);
    }

    // Save changes to .txt
    saveCoursesToTxt();

    printf("✅ Teacher assigned successfully.\n");
}

void assignCoursesToBatch()
{
    char batchName[50];
    int courseCount;

    FILE *fBatch = fopen("batches.dat", "r+b");
    if (!fBatch)
    {
        printf("No batches found. Add batches first.\n");
        return;
    }

    printf("Enter batch name: ");
    getchar();
    fgets(batchName, sizeof(batchName), stdin);
    batchName[strcspn(batchName, "\n")] = 0;

    Batch b;
    int found = 0;

    while (fread(&b, sizeof(Batch), 1, fBatch))
    {
        if (strcmp(b.name, batchName) == 0)
        {
            found = 1;
            printf("Enter number of courses to assign (max %d): ", MAX_COURSES_PER_BATCH);
            scanf("%d", &courseCount);

            if (courseCount < 1 || courseCount > MAX_COURSES_PER_BATCH)
            {
                printf("Invalid number of courses.\n");
                fclose(fBatch);
                return;
            }

            for (int i = 0; i < courseCount; i++)
            {
                printf("Enter course code %d: ", i + 1);
                scanf(" %9s", b.courseCodes[i]);
            }
            b.courseCount = courseCount;

            fseek(fBatch, -sizeof(Batch), SEEK_CUR);
            fwrite(&b, sizeof(Batch), 1, fBatch);

            // Save batch to .txt
            saveBatchesToTxt();

            printf("✅ Courses assigned successfully to batch %s.\n", batchName);
            break;
        }
    }

    if (!found)
    {
        printf("Batch not found.\n");
    }

    fclose(fBatch);
}
