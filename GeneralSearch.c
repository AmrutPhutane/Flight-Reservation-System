// #include <stdio.h>
// #include <ctype.h>
// #include <string.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <math.h>
// #include <time.h>
//
// struct travel_info
// {
//     char* departure_city;
//     char* arrival_city;
//     char* departure_date;
//
// };
//
// typedef struct travel_info Struct;
//
// Struct get_travel_info()
// {
//     char* departure_city;
//     char* arrival_city;
//     char* departure_date;
//
//     printf("Currently, please choose one of these: Chennai(MAA) Mumbai(BOM) Delhi(DEL) Bangalore(BLR) Kolkata(CCU) Hyderabad(HYD)\n");
//     printf("Currently, we have all the international cities available.\n");
//     printf("If you have thought of journey date, please type in form of dd/mm/yyyy \n");
//     printf("Please write the Departure City[space]Arrival City[space]Date\n");
//     scanf("%s %s %s", departure_city, arrival_city, departure_date);
//
//     Struct info;
//     info.departure_city = departure_city;
//     info.arrival_city = arrival_city;
//     info.departure_date = departure_date;
//
//     return info;
//
// }
//
// int main()
// {
//     time_t t;
//     time(&t);
//     char* date = ctime(&t);
//     char* day = strtok(date, " ");
//
//     char* d_city;
//     char* a_city;
//     char* d_date;
//
//     Struct travel_info;
//     travel_info = get_travel_info();
//     printf("Departure City: %s\n", travel_info.departure_city);
//     printf("Arrival City: %s\n", travel_info.arrival_city);
//     printf("Departure date: %s\n", travel_info.departure_date);
//
//
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct travel_info {
    char departure_city[20];
    char arrival_city[20];
    char departure_date[11];
};

typedef struct travel_info TravelInfo;

TravelInfo get_travel_info() {
    TravelInfo info;

    printf("Enter Departure City\n");
    printf("Currently, please choose one of these: Chennai(MAA), Mumbai(BOM), Delhi(DEL), Bangalore(BLR), Kolkata(CCU), Hyderabad(HYD)\n");
    printf("City: ");
    scanf("%19s", info.departure_city);

    printf("Enter Arrival City\n");
    printf("Currently, we have all the international cities available.\n");
    printf("City: ");
    scanf("%19s", info.arrival_city);

    printf("If you have thought of journey date, please type in form of dd/mm/yyyy \n");
    printf("Date: ");
    scanf("%10s", info.departure_date);

    return info;
}

char* get_flights(struct travel_info)
{
    char*
}

int main() {
    time_t t;
    time(&t);
    char* date = ctime(&t);
    printf("Current date and time: %s", date); // Print current date and time for reference

    TravelInfo travel_info = get_travel_info();

    printf("Departure City: %s\n", travel_info.departure_city);
    printf("Arrival City: %s\n", travel_info.arrival_city);
    printf("Departure date: %s\n", travel_info.departure_date);

    return 0;
}




