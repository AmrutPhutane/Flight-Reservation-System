#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 256
#define MAX_STRING 50
#define MAX_FLIGHTS 500  // Increased to handle more flights

typedef struct {
    char flight_number[10];
    char origin_code[4];
    char origin_city[MAX_STRING];
    char destination_code[4];
    char destination_city[MAX_STRING];
    char departure_time[6];
    char arrival_time[6];
    char duration[10];
    char operating_days[MAX_STRING];
} Flight;

// Global variables
Flight *flights = NULL;  // Dynamic array for flights
int num_flights = 0;

// Function to convert string to lowercase
void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to trim whitespace from both ends
void trim(char *str) {
    int i, j;
    
    // Trim leading spaces
    for (i = 0; str[i] && isspace((unsigned char)str[i]); i++);
    
    if (i > 0) {
        for (j = 0; str[i]; i++, j++) {
            str[j] = str[i];
        }
        str[j] = '\0';
    }
    
    // Trim trailing spaces
    for (i = strlen(str) - 1; i >= 0 && isspace((unsigned char)str[i]); i--);
    str[i + 1] = '\0';
}

// Function to parse a line from file into Flight struct
int parseLine(char *line, Flight *flight) {
    char *token;
    int field = 0;
    
    // Skip header lines
    if (strstr(line, "Flight") != NULL || strstr(line, "----") != NULL) {
        return 0;
    }
    
    token = strtok(line, "\t");  // Assuming tab-separated values
    while (token != NULL && field < 9) {
        trim(token);
        switch(field) {
            case 0: strcpy(flight->flight_number, token); break;
            case 1: strcpy(flight->origin_code, token); break;
            case 2: strcpy(flight->origin_city, token); break;
            case 3: strcpy(flight->destination_code, token); break;
            case 4: strcpy(flight->destination_city, token); break;
            case 5: strcpy(flight->departure_time, token); break;
            case 6: strcpy(flight->arrival_time, token); break;
            case 7: strcpy(flight->duration, token); break;
            case 8: strcpy(flight->operating_days, token); break;
        }
        token = strtok(NULL, "\t");
        field++;
    }
    
    return (field == 9);  // Return 1 if all fields were parsed successfully
}

// Function to load flights from file
int loadFlightsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE];
    int max_flights = 100;  // Initial capacity
    flights = malloc(max_flights * sizeof(Flight));
    
    if (!flights) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return 0;
    }
    
    // Skip header lines
    fgets(line, sizeof(line), file);  // Skip header
    fgets(line, sizeof(line), file);  // Skip separator line
    
    while (fgets(line, sizeof(line), file)) {
        // Resize array if needed
        if (num_flights >= max_flights) {
            max_flights *= 2;
            Flight *temp = realloc(flights, max_flights * sizeof(Flight));
            if (!temp) {
                printf("Error: Memory reallocation failed\n");
                free(flights);
                fclose(file);
                return 0;
            }
            flights = temp;
        }
        
        // Parse line and add flight
        if (parseLine(line, &flights[num_flights])) {
            num_flights++;
        }
    }
    
    fclose(file);
    return 1;
}

// Function to check if a flight operates on a given day
int operatesOnDay(const char* operating_days, const char* search_day) {
    char days_lower[MAX_STRING];
    char search_lower[MAX_STRING];
    
    strcpy(days_lower, operating_days);
    strcpy(search_lower, search_day);
    
    toLowerCase(days_lower);
    toLowerCase(search_lower);
    
    return (strstr(days_lower, "daily") != NULL || strstr(days_lower, search_lower) != NULL);
}

// Function to search and sort flights by departure time
void searchFlights(const char* origin, const char* destination, const char* day) {
    int found = 0;
    char search_origin[MAX_STRING], search_dest[MAX_STRING];
    Flight *matching_flights = malloc(num_flights * sizeof(Flight));
    int match_count = 0;
    
    if (!matching_flights) {
        printf("Error: Memory allocation failed\n");
        return;
    }
    
    strcpy(search_origin, origin);
    strcpy(search_dest, destination);
    toLowerCase(search_origin);
    toLowerCase(search_dest);

    // Find matching flights
    for (int i = 0; i < num_flights; i++) {
        char curr_origin_code[4], curr_origin_city[MAX_STRING];
        char curr_dest_code[4], curr_dest_city[MAX_STRING];
        
        strcpy(curr_origin_code, flights[i].origin_code);
        strcpy(curr_origin_city, flights[i].origin_city);
        strcpy(curr_dest_code, flights[i].destination_code);
        strcpy(curr_dest_city, flights[i].destination_city);
        
        toLowerCase(curr_origin_code);
        toLowerCase(curr_origin_city);
        toLowerCase(curr_dest_code);
        toLowerCase(curr_dest_city);

        if ((*search_origin == '\0' || strcmp(curr_origin_code, search_origin) == 0 || strcmp(curr_origin_city, search_origin) == 0) &&
            (*search_dest == '\0' || strcmp(curr_dest_code, search_dest) == 0 || strcmp(curr_dest_city, search_dest) == 0) &&
            (*day == '\0' || operatesOnDay(flights[i].operating_days, day))) {
            
            matching_flights[match_count++] = flights[i];
        }
    }

    // Sort matching flights by departure time (simple bubble sort)
    for (int i = 0; i < match_count - 1; i++) {
        for (int j = 0; j < match_count - i - 1; j++) {
            if (strcmp(matching_flights[j].departure_time, matching_flights[j + 1].departure_time) > 0) {
                Flight temp = matching_flights[j];
                matching_flights[j] = matching_flights[j + 1];
                matching_flights[j + 1] = temp;
            }
        }
    }

    // Print results
    printf("\nFlight   From  Departure City       To    Arrival City         Dep    Arr    Duration Operating Days\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < match_count; i++) {
        printf("%-8s %-5s %-18s %-5s %-18s %-6s %-6s %-8s %s\n",
            matching_flights[i].flight_number,
            matching_flights[i].origin_code,
            matching_flights[i].origin_city,
            matching_flights[i].destination_code,
            matching_flights[i].destination_city,
            matching_flights[i].departure_time,
            matching_flights[i].arrival_time,
            matching_flights[i].duration,
            matching_flights[i].operating_days);
    }

    if (match_count == 0) {
        printf("\nNo flights found matching your criteria.\n");
    } else {
        printf("\nFound %d matching flights.\n", match_count);
    }

    free(matching_flights);
}

void cleanupFlights() {
    if (flights) {
        free(flights);
        flights = NULL;
    }
}

int main() {
    char origin[MAX_STRING] = "";
    char destination[MAX_STRING] = "";
    char day[MAX_STRING] = "";
    char filename[MAX_STRING];

    printf("Enter the flight data file name: ");
    scanf("%s", filename);
    getchar();  // Consume newline

    if (!loadFlightsFromFile(filename)) {
        printf("Failed to load flights data. Exiting...\n");
        return 1;
    }

    printf("Successfully loaded %d flights.\n\n", num_flights);
    printf("Welcome to Flight Search System\n");
    printf("Enter search criteria (press Enter to skip):\n");
    
    printf("From (city or code): ");
    fgets(origin, MAX_STRING, stdin);
    origin[strcspn(origin, "\n")] = 0;
    
    printf("To (city or code): ");
    fgets(destination, MAX_STRING, stdin);
    destination[strcspn(destination, "\n")] = 0;
    
    printf("Day of travel: ");
    fgets(day, MAX_STRING, stdin);
    day[strcspn(day, "\n")] = 0;

    searchFlights(origin, destination, day);
    cleanupFlights();

    return 0;
}