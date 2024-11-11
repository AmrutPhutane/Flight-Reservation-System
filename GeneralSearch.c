#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>


struct Flight {
    char fnumber[10];
    char orgcity[50];
    char dstcity[50];
    char days[100];
};

struct Information {
    char orgcity[50];
    char dstcity[50];
    char date[10];
};

typedef struct Information TravelInfo;

void toTitleCase(char* str) {
    int i;
    if (str[0] != '\0') {
        str[0] = toupper(str[0]);
    }
    for (i = 1; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

TravelInfo getinfo() {
    TravelInfo details;

    printf("Enter Departure City\n");
    printf("City: ");
    fgets(details.orgcity, sizeof(details.orgcity), stdin);
    details.orgcity[strcspn(details.orgcity, "\n")] = '\0';
    toTitleCase(details.orgcity);

    printf("Enter Arrival City\n");
    printf("City: ");
    fgets(details.dstcity, sizeof(details.dstcity), stdin);
    details.dstcity[strcspn(details.dstcity, "\n")] = '\0';
    toTitleCase(details.dstcity);

    printf("Enter Journey Date (dd-mm-yyyy): ");
    fgets(details.date, sizeof(details.date), stdin);
    details.date[strcspn(details.date, "\n")] = '\0';
    toTitleCase(details.date);

    return details;
}

char* weekdayon(char* date) {
    char* days[] = {"Sun", "Mon", "Tue", "Wed","Thu", "Fri", "Sat"};
    char result[50];
    struct tm future = {0};
    int day, month, year;

    sscanf(date, "%d-%d-%d", &day, &month, &year);
    future.tm_mday = day;
    future.tm_mon = month - 1;
    future.tm_year = year - 1900;

    time_t now = time(NULL);
    struct tm* current = localtime(&now);
    time_t future_time = mktime(&future);

    if (future_time == -1) {
        sprintf(result, "Invalid date");
        return result;
    }

    if (difftime(future_time, now) <= 0) {
        sprintf(result, "Error: Date must be in the future");
        return result;
    }

    sprintf(result, "%s", days[future.tm_wday]);
    return result;
}

void getflight(TravelInfo* info) {
    struct Flight flight;
    FILE* file = fopen("C:\\Users\\amrut\\OneDrive\\Documents\\GitHub\\Flight-Reservation-System\\Flights.txt", "r");

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char future_day[] =
    char line[1024];
    for (int i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), file) == NULL) break;
    }
    bool found = false;
    while (fgets(line, sizeof(line), file)) {
        char* line_copy = strdup(line);
        if (line_copy == NULL) {
            printf("Memory allocation failed\n");
            continue;
        }

        memset(&flight, 0, sizeof(struct Flight));

        char* token = strtok(line, " \t");
        if (token == NULL) {
            free(line_copy);
            continue;
        }

        strncpy(flight.fnumber, token, sizeof(flight.fnumber) - 1);

        token = strtok(NULL, " \t");
        if (token == NULL) {
            free(line_copy);
            continue;
        }

        token = strtok(NULL, " \t");
        if (token) {
            strncpy(flight.orgcity, token, sizeof(flight.orgcity) - 1);
        }

        token = strtok(NULL, " \t");
        if (token == NULL) {
            free(line_copy);
            continue;
        }

        token = strtok(NULL, " \t");
        if (token) {
            strncpy(flight.dstcity, token, sizeof(flight.dstcity) - 1);
        }

        flight.days[0] = '\0';
        while (token != NULL) {
            token = strtok(NULL, " \t");
            if (token) {
                strcat(flight.days, token);
                strcat(flight.days, " ");
            }
        }

        int match1 = 0, match2 = 0;

        if (strlen(info->orgcity) > 0 && strlen(info->dstcity) > 0) {
            if (strcasecmp(info->orgcity, flight.orgcity) == 0 &&
                strcasecmp(info->dstcity, flight.dstcity) == 0) {
                match1 = 1;
            }
        } else if (strlen(info->orgcity) == 0 && strlen(info->dstcity) > 0) {
            if (strcasecmp(info->dstcity, flight.dstcity) == 0) {
                match1 = 1;
            }
        } else if (strlen(info->orgcity) > 0 && strlen(info->dstcity) == 0) {
            if (strcasecmp(info->orgcity, flight.orgcity) == 0) {
                match1 = 1;
            }
        } else {
            match1 = 1;
        }

        if (match1) {
            found = true;
            if (strstr(flight.days, "Daily") != NULL) {
                match2 = 1;
            } else if (strstr(flight.days, info->dptday) != NULL) {
                match2 = 1;
            }
        }

        if (match1 && match2) {
            printf("%s", line_copy);
        }

        free(line_copy);
    }
    if(!found)
    {
        printf("Flights not available\n");
    }
    fclose(file);
}





int main() {
    TravelInfo travelInfo = getinfo();

    printf("\nSearching for flights...\n");
    getflight(&travelInfo);

    return 0;
}