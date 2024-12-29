#ifndef UTILS_H
#define UTILS_H

#include "../include/datatypes.h"

/* UI */
int getChoice(int max);
void clearScreen();

void printMainMenu(void);

/* Run */

int runPredefinedMonths(Month months[]);
int runPredefinedObservations(Month months[]);
int runListAllObservations(Month months[]);
int runSearchObservationsByMonth(Month months[]);
int runAddObservations(Month months[]);
int runDeleteObservations(Month months[]);


#endif // UTILS_H