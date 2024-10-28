//
// Created by amrut on 28-10-2024.
//
#include <stdio.h>
#include "keywords.h"


void cleaned_text(char text[100])
{
    int i, j=0;
    char output[100];
    for(i=0; text[i]!='\0'; i++)
    {
        if(text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] = text[i] + 32;
            output[j++] = text[i];
        }
        else if(text[i] >= 'a' && text[i] <= 'z'||
                text[i] == ' ' ||
                text[i] >= '0' && text[i] <= '9')
        {
            output[j++] = text[i];
        }


    }
    output[j]='\0';
    printf("%s", output);
}

void tokenize(char text[100])
{
    int i, j=0,k;
    char output[100];
    for(i=0; text[i]!='\0'; i++)
    {
        k=0;
        if(text[i] == ' ')
        {
            for(j=i; j<text[i]; j++)
        }
    }
}


