#include <stdio.h>
#include "keywords.h"


void main()
{
    char text[100];
    printf("Enter text: ");
    gets(text);
    cleaned_text(text);
}
