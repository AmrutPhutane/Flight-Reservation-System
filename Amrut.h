// Amrut.h
#ifndef AMRUT_H
#define AMRUT_H

char* cleaner(const char text[]);
int* vectorize(char text[]);
double similarity(const int vector1[26], const int vector2[26]);
int find_keyword(char cleaned_output[], struct chadbot services[], int keywords_count);
int update(struct chadbot services[], int services_count);
char* replier(int line_number);


#endif // AMRUT_H
