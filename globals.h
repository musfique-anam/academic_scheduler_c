#ifndef GLOBALS_H
#define GLOBALS_H

#include "structures.h"

#define MAX_TEACHERS 50
#define MAX_COURSES 100
#define MAX_BATCHES 100
#define MAX_ROOMS 50
#define MAX_COURSES_PER_BATCH 20
#define MAX_BATCH_COURSES 10  // or however many courses a batch can have


extern Teacher teachers[MAX_TEACHERS];
extern int numTeachers;

extern Course courses[MAX_COURSES];
extern int numCourses;

extern Batch batches[MAX_BATCHES];
extern int numBatches;

extern Room rooms[MAX_ROOMS];
extern int numRooms;

extern int numCourses;

#endif // GLOBALS_H
