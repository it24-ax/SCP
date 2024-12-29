#include <stdio.h>

#include "../include/utils.h"
#include "../include/global.h"

// run command: root: gcc src/*.c

int main(void)
{
    // Räcka med hårdkodad storlek (TOTAL_MONTHS finns i global.h)
    Month months[TOTAL_MONTHS];

    // Sätter in alla månader med tomma pekare till länkade listor
    if (!runPredefinedMonths(months))
    {
        printf("Error adding predefined months\n");
        return 0;
    }

    // Sätter in observationer att testa med
    if (!runPredefinedObservations(months))
    {
        printf("Error adding predefined observations\n");
        return 0;
    }

    do
    {
        printMainMenu();
        int choice = getChoice(5);
        switch (choice)
        {
        case 1:
            clearScreen();
            /* Run list all observations*/
            // Listar alla månader och observationer om det finns
            if(!runListAllObservations(months)){
                printf("Error listing all observations\n");
                return 0;
            }
            break;
        case 2:
            clearScreen();
            /* Run search observations*/
            // Söker efter observationer i en viss månad (Hade att man kunde leta med datum men inte i uppgiften)
            if(!runSearchObservationsByMonth(months)){
                printf("Error searching observations\n");
                return 0;
            }
            break;
        case 3:
            clearScreen();
            /* Run add observations*/
            // Lägger till en observation på rätt plats i länkad lista
            if(!runAddObservations(months)){
                printf("Error adding observation\n");
                return 0;
            }
            break;
        case 4:
            clearScreen();
            /* Run delete observations*/
            // Tar bort observation på en datum (Flyttar pekare om det fanns observationer efter) eller hela månad
            if(!runDeleteObservations(months)){
                printf("Error deleting observation\n");
                return 0;
            }
            break;
        case 5:
            // Hejdå!
            printf("Exit\n");
            return 0;
        default:
            break;
        }
    } while (1);
}