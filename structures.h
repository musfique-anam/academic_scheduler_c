#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_COURSES_PER_BATCH 20
#define MAX_COURSE_CODE_LEN 10
#define MAX_TEACHER_NAME_LEN 50
#define MAX_BATCH_NAME_LEN 50
#define MAX_ROOM_NO_LEN 10
#define MAX_DAY_LEN 10
#define MAX_TIME_LEN 6
#define MAX_AVAILABLE_DAYS 4
#define MAX_AVAILABLE_SLOTS 5
#define MAX_COURSE_NAME_LEN 50
#define MAX_SLOTS 4
#define MAX_COURSES_PER_TEACHER 10
#define MAX_EQUIPMENTS 5
#define MAX_TIMESLOTS 10

#ifndef MAX_DAYS
#define MAX_DAYS 7
#endif

typedef struct {
    char name[50];
    char department[50];
    char teacherID[10];
    char position[50];
    int availableDays[MAX_DAYS];      // 0=Friday, 1=Saturday, ..., 6=Thursday
    int availableSlots[MAX_SLOTS];    // 1 = available, 0 = not available
    int maxCourses;
    char courseCodes[MAX_COURSES_PER_TEACHER][10];
    int currentCourses;
    int maxCredits;
    int currentCredits;               // for scheduler calculations
    char phone[20];
    char email[50];
    int slotCount;
} Teacher;

typedef struct {
    char code[MAX_COURSE_CODE_LEN];
    char name[MAX_COURSE_NAME_LEN];
    int credit;
    int isLab;
    char teacherName[MAX_TEACHER_NAME_LEN];
} Course;

typedef struct {
    char name[MAX_BATCH_NAME_LEN];
    int courseCount;
    char courseCodes[MAX_COURSES_PER_BATCH][MAX_COURSE_CODE_LEN];
} Batch;

typedef struct {
    char roomNo[20];
    int isLabRoom;                      // 1 = Yes, 0 = No
    int capacity;                       // number of students
    char equipments[MAX_EQUIPMENTS][30]; // e.g., Projector, PC, Whiteboard
    int equipmentCount;
    char availableDays[MAX_DAYS][10];    // e.g., "Saturday", "Sunday"
    int availableDayCount;
    char availableTimes[MAX_TIMESLOTS][10]; // e.g., "09:00-10:30"
    int availableTimeCount;
} Room;

typedef struct {
    char courseCode[MAX_COURSE_CODE_LEN];
    char batch[MAX_BATCH_NAME_LEN];
    char teacher[MAX_TEACHER_NAME_LEN];
    char roomNo[MAX_ROOM_NO_LEN];
    char day[MAX_DAY_LEN];
    char startTime[MAX_TIME_LEN];
    int isExam;
    int isLab;
    int duration;
} Schedule;

#endif // STRUCTURES_H
