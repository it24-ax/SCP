#ifndef CONFIG_H
#define CONFIG_H

/*
- Linked lists

*/

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    int hour;
    int minute;
    int second;
} Time;

typedef struct
{
    Date date;
    Time time;
} TimeStamp;

typedef struct
{
    int name;
    int classification;
    char place[100];
    char description[1024];
} Data;

/* Node */
typedef struct Observation {
    TimeStamp timeStamp;
    Data data;
    struct Observation *next;
} Observation;

/* I month räcka*/
typedef struct {
    Date date;
    Observation *head;
} Month;


#endif // CONFIG_H