#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/datatypes.h"
#include "../include/global.h"

/* UI functions */
// En lätt implementation att ta emot val för main meny
int getChoice(int max)
{
    int choice = 0;
    do
    {
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n')
                ;
        }
        else
        {
            if (choice < 1 || choice > max)
            {
                printf("Invalid choice. Try again.\n");
            }
        }

    } while (choice < 1 || choice > max);

    return choice;
}

// Hehe, kom på själv. Funkar bra
void clearScreen(void)
{
    if (system("cls")) // Windows
    {
        system("clear"); // UNIX/Linux
    }
}

// Skriver ut "huvudmenyn"
void printMainMenu(void)
{
    printf("-----------------------\n");
    printf("1. List all observations\n");
    printf("2. Search observations\n");
    printf("3. Add observations\n");
    printf("4. Delete observations\n");
    printf("5. Exit program\n");
    printf("-----------------------\n");
}

// Jämför två datum och tider så att man kan sätta nya observationer i rätt ordning (det blir sorterad)
int compareDates(const TimeStamp *a, const TimeStamp *b)
{
    if (a->date.year != b->date.year)
        return a->date.year < b->date.year;
    if (a->date.month != b->date.month)
        return a->date.month < b->date.month;
    if (a->date.day != b->date.day)
        return a->date.day < b->date.day;
    if (a->time.hour != b->time.hour)
        return a->time.hour < b->time.hour;
    if (a->time.minute != b->time.minute)
        return a->time.minute < b->time.minute;
    return a->time.second < b->time.second;
}

/* Observation functions */

// Skapar en observation
Observation *createObservation(TimeStamp *timeStamp, Data *data)
{
    Observation *observation = malloc(sizeof(Observation));
    if (observation == NULL)
    {
        // Catastrophic failure, out of memory?
        return NULL;
    }

    observation->timeStamp = *timeStamp;
    observation->data = *data;
    observation->next = NULL;
    return observation;
}

// Sätter in en observation i rätta månad/länkade lista
Observation *insertObservation(Month months[], TimeStamp *timeStamp, Data *data)
{
    // Skapa observation
    Observation *newObs = createObservation(timeStamp, data);
    if (newObs == NULL)
    {
        // Catastrophic failure, out of memory? verbatum
        return NULL; // Creation failed
    }

    // Letar för rätt månad (Kan göra till sin egen funktion men eh...)
    int monthIndex = ((timeStamp->date.year - START_YEAR) * MONTHS_IN_A_YEAR) +
                     (timeStamp->date.month - 1);

    // Om index är felaktigt då struntar man att skapa observationen
    if (monthIndex < 0 || monthIndex >= TOTAL_MONTHS)
    {
        free(newObs);
        return NULL;
    }

    // Hämta head av månadens lista
    Observation *head = months[monthIndex].head;

    // Om det finns ingen observation i månaden då sätter man det som head
    if (head == NULL)
    {
        months[monthIndex].head = newObs;
        return newObs;
    }

    // Om observationen är tidigare än head då bytter man head till nya observationen
    if (compareDates(timeStamp, &head->timeStamp))
    {
        newObs->next = head;
        months[monthIndex].head = newObs;
        return newObs;
    }

    // Hitta rätt plats att sätta in observationen i läkade listan
    Observation *current = head;
    while (current->next != NULL && compareDates(&current->next->timeStamp, timeStamp))
    {
        current = current->next;
    }

    // Sätt in observationen och flytta pekare
    newObs->next = current->next;
    current->next = newObs;
    return newObs;
}

/* Run functions */

// Sätt alla månder med head pekare i räckan
int runPredefinedMonths(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Fyll array med månader
    for (int i = 0; i < TOTAL_MONTHS; i++)
    {
        months[i].date.year = START_YEAR + i / MONTHS_IN_A_YEAR;
        months[i].date.month = i % MONTHS_IN_A_YEAR + 1;
        months[i].date.day = 1;
        months[i].head = NULL;
        // Debug
        // printf("Month %d: %d-%d-%d\n", i + 1, months[i].date.year, months[i].date.month, months[i].date.day);
    }
    printf("-- Predefined months added\n");
    return 1;
}

// Sätt in observationer för att testa
int runPredefinedObservations(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Sätt in observationer. Skapar timestamp och data som man kan sätta in i insertObservation funktionen
    TimeStamp ts1 = {{1, 1, 2024}, {0, 0, 0}};
    Data data1 = {1, 1, "Stockholm", "First ever observation"};
    Observation *obs1 = insertObservation(months, &ts1, &data1);
    if (!obs1)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    TimeStamp ts2 = {{15, 1, 2031}, {12, 0, 0}};
    Data data2 = {1002, 2, "Uppsala", "A observation"};
    Observation *obs2 = insertObservation(months, &ts2, &data2);
    if (!obs2)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    TimeStamp ts3 = {{1, 5, 2031}, {14, 0, 0}};
    Data data3 = {2001, 1, "Gothenburg", "A observation"};
    Observation *obs3 = insertObservation(months, &ts3, &data3);
    if (!obs3)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    TimeStamp ts4 = {{15, 5, 2031}, {15, 0, 0}};
    Data data4 = {2004, 1, "Rinkoping", "A observation"};
    Observation *obs4 = insertObservation(months, &ts4, &data4);
    if (!obs4)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    TimeStamp ts5 = {{22, 5, 2031}, {13, 20, 15}};
    Data data5 = {2023, 1, "Saudi Arabia", "A observation"};
    Observation *obs5 = insertObservation(months, &ts5, &data5);
    if (!obs5)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    TimeStamp ts6 = {{1, 12, 2033}, {23, 59, 59}};
    Data data6 = {5001, 1, "Bengal", "Last observation"};
    Observation *obs6 = insertObservation(months, &ts6, &data6);
    if (!obs6)
    {
        printf("Failed to add observation\n");
        return 0;
    }

    printf("-- Predefined observations added\n");
    return 1;
}

int runListAllObservations(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Skriva ut alla månader och deras länkade listor
    printf("-- List all months\n");
    for (int i = 0; i < TOTAL_MONTHS; i++)
    {
        // Lista alla observationer i månaden
        if (months[i].head != NULL)
        {
            printf("Month: %d-%d-%d\n", months[i].date.year, months[i].date.month, months[i].date.day);
            Observation *current = months[i].head;
            while (current != NULL)
            {
                printf("-----------------------\n");
                printf("\033[43m Observation: \033[0m\n");
                printf("  Name: %d\n", current->data.name);
                printf("  Classification: %d\n", current->data.classification);
                printf("  Place: %s\n", current->data.place);
                printf("  Description: %s\n", current->data.description);
                printf("  Date: %d-%d-%d\n", current->timeStamp.date.day, current->timeStamp.date.month, current->timeStamp.date.year);
                printf("  Time: %d:%d:%d\n", current->timeStamp.time.hour, current->timeStamp.time.minute, current->timeStamp.time.second);
                current = current->next;
            }
        }
        else
        {
            // Inget observationer i månaden (Man behöver inte skriva ut men ser bra ut tycker man)
            printf("No observations in Month: %d-%d-%d\n", months[i].date.year, months[i].date.month, months[i].date.day);
        }
    }

    printf("-- Listed all observations\n");
    return 1;
}

// Leta observation med år och månad bara.
int runSearchObservationsByMonth(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Användare skriver in
    Date searchMonth;
    printf("Enter date to search for (yyyy-mm): ");

    do
    {
        scanf("%d-%d", &searchMonth.year, &searchMonth.month);
    } while (searchMonth.year < START_YEAR || searchMonth.year > START_YEAR + YEARS || searchMonth.month < 1 || searchMonth.month > 12);

    // Kunde vara sin egen funktion
    int monthIndex = ((searchMonth.year - START_YEAR) * MONTHS_IN_A_YEAR) + (searchMonth.month - 1);

    // Felaktig input
    if (monthIndex < 0 || monthIndex >= TOTAL_MONTHS)
    {
        printf("Out of range date.\n");
    }

    // Skriv ut länkade lista i månaden
    if (months[monthIndex].head != NULL)
    {
        Observation *current = months[monthIndex].head;
        while (current != NULL)
        {
            printf("-----------------------\n");
            printf("\033[43m Observation: \033[0m\n");
            printf("  Name: %d\n", current->data.name);
            printf("  Classification: %d\n", current->data.classification);
            printf("  Place: %s\n", current->data.place);
            printf("  Description: %s\n", current->data.description);
            printf("  Date: %d-%d-%d\n", current->timeStamp.date.day, current->timeStamp.date.month, current->timeStamp.date.year);
            printf("  Time: %d:%d:%d\n", current->timeStamp.time.hour, current->timeStamp.time.minute, current->timeStamp.time.second);
            current = current->next;
        }

        printf("-- Searched observations by date\n");
        return 1;
    }

    // Inga observationer i månaden
    printf("No observations in Month: %d-%d-%d\n", months[monthIndex].date.year, months[monthIndex].date.month, months[monthIndex].date.day);
    printf("-- Searched observations by date\n");
    return 1;
}

int runAddObservations(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Användare skriver in input
    TimeStamp ObservationDate;
    Data ObservationData;

    do
    {
        printf("Enter date (DD-MM-YYYY): ");
        scanf("%d-%d-%d", &ObservationDate.date.day, &ObservationDate.date.month, &ObservationDate.date.year);
    } while (ObservationDate.date.year < START_YEAR || ObservationDate.date.year > START_YEAR + YEARS || ObservationDate.date.month < 1 || ObservationDate.date.month > 12 || ObservationDate.date.day < 1 || ObservationDate.date.day > 31);

    do
    {
        printf("Enter time (hh:mm:ss): ");
        scanf("%d:%d:%d", &ObservationDate.time.hour, &ObservationDate.time.minute, &ObservationDate.time.second);
    } while (ObservationDate.time.hour < 0 || ObservationDate.time.hour > 23 || ObservationDate.time.minute < 0 || ObservationDate.time.minute > 59 || ObservationDate.time.second < 0 || ObservationDate.time.second > 59);

    do
    {
        printf("Enter name (Number): ");
        scanf("%d", &ObservationData.name);
    } while (ObservationData.name < 1);

    do
    {
        printf("Enter classification (0-2): ");
        scanf("%d", &ObservationData.classification);
    } while (ObservationData.classification < 0 || ObservationData.classification > 2);

    do
    {
        printf("Enter place (Text): ");
        scanf("%s", ObservationData.place);
    } while (strlen(ObservationData.place) < 1 || strlen(ObservationData.place) > 100);

    do
    {
        printf("Enter description (Text): ");
        scanf("%s", ObservationData.description);
    } while (strlen(ObservationData.description) < 1 || strlen(ObservationData.description) > 500);

    // Gör till en ny observation
    Observation *newObs = insertObservation(months, &ObservationDate, &ObservationData);

    // Om det inte gick att sätta in observationen, då struntar de
    if (newObs == NULL)
    {
        free(newObs);
        printf("Failed to add observation\n");
    }

    printf("-- Added observation\n");
    return 1;
}

int runDeleteObservations(Month months[])
{
    if (months == NULL)
    {
        return 0;
    }

    // Input från användare att radera en datum eller hela månaden
    int choice;
    do
    {
        printf("1. Delete obaservation/s in a specific date: \n");
        printf("2. Delete all observations in a month: \n ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 2);

    if (choice == 1)
    {
        // Radera observationer för en specifik datum (Kan vara flera i en datum)
        Date searchDate;
        printf("Enter date to delete (DD-MM-YYYY): ");
        scanf("%d-%d-%d", &searchDate.day, &searchDate.month, &searchDate.year);

        // Igen.. igen.. igen..
        int monthIndex = ((searchDate.year - START_YEAR) * MONTHS_IN_A_YEAR) +
                         (searchDate.month - 1);

        if (monthIndex < 0 || monthIndex >= TOTAL_MONTHS)
        {
            printf("Date out of range\n");
            return 0;
        }

        // Nuvarande och previous att see om det är head eller inte, och hur många man raderade :)
        Observation *current = months[monthIndex].head;
        Observation *prev = NULL;
        int deleted = 0;

        while (current != NULL)
        {
            if (current->timeStamp.date.day == searchDate.day)
            {
                // Om det finns inga förre då är det en head
                if (prev == NULL)
                {
                    months[monthIndex].head = current->next;
                    free(current);
                    current = months[monthIndex].head;
                }
                // Om man raderar efter head
                else
                {
                    prev->next = current->next;
                    free(current);
                    current = prev->next;
                }
                deleted++;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }

        if (deleted > 0)
        {
            printf("Deleted %d observation(s)\n", deleted);
        }
        else
        {
            printf("No observations found for date %d-%d-%d\n",
                   searchDate.day, searchDate.month, searchDate.year);
        }
    }
    else if (choice == 2)
    {
        // Radera alla observationer i en månad
        Date searchMonth;
        do
        {
            printf("Enter date to search for (yyyy-mm): ");
            scanf("%d-%d", &searchMonth.year, &searchMonth.month);
        } while (searchMonth.year < START_YEAR || searchMonth.year > START_YEAR + YEARS || searchMonth.month < 1 || searchMonth.month > 12);

        // Ahhh, borde ha gjort till en egen funktion
        int monthIndex = ((searchMonth.year - START_YEAR) * MONTHS_IN_A_YEAR) + (searchMonth.month - 1);

        // Felaktig input
        if (monthIndex < 0 || monthIndex >= TOTAL_MONTHS)
        {
            printf("Out of range date.\n");
        }
        else
        {
            // Radera alla observationer i månaden och sätter head till Null
            Observation *current = months[monthIndex].head;
            while (current != NULL)
            {
                Observation *next = current->next;
                free(current);
                current = next;
            }
            months[monthIndex].head = NULL;
        }
    }

    printf("-- Deleted observation\n");
    return 1;
}