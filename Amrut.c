// Created by amrut on 28-10-2024.
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////
char* cleaner(char text[100])
{
    int j = 0;
    char lowercase_output[100], cleaned_output[100];
    cleaned_output[0] = '\0';

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i]) ) {
            lowercase_output[j++] = tolower(text[i]);
        }
    }
    lowercase_output[j] = '\0';
    //printf("%s\n", lowercase_output);
    /*
    const char stop_words[] = {
        "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "you're",
        "you've", "you'll", "you'd", "your", "yours", "yourself", "yourselves", "he",
        "him", "his", "himself", "she", "she's", "her", "hers", "herself", "it", "it's",
        "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which",
        "who", "whom", "this", "that", "that'll", "these", "those", "am", "is", "are",
        "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does",
        "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until",
        "while", "of", "at", "by", "for", "with", "about", "against", "between", "into",
        "through", "during", "before", "after", "above", "below", "to", "from", "up",
        "down", "in", "out", "on", "off", "over", "under", "again", "further", "then",
        "once", "here", "there", "when", "where", "why", "how", "all", "any", "both",
        "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only",
        "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don",
        "don't", "should", "should've", "now", "d", "ll", "m", "o", "re", "ve", "y", "ain",
        "aren", "aren't", "couldn", "couldn't", "didn", "didn't", "doesn", "doesn't",
        "hadn", "hadn't", "hasn", "hasn't", "haven", "haven't", "isn", "isn't", "ma",
        "mightn", "mightn't", "mustn", "mustn't", "needn", "needn't", "shan", "shan't",
        "shouldn", "shouldn't", "wasn", "wasn't", "weren", "weren't", "won", "won't",
        "wouldn", "wouldn't"
    };
    int stopword_count = sizeof(stop_words) / sizeof(stop_words[0]);

    char token = strtok(lowercase_output, " ");
    while (token != NULL)
    {
        bool stop_word = false;

        for (int i = 0; i < stopword_count; i++)
        {
            if (strcmp(token, stop_words[i]) == 0)
            {
                stop_word = true;
                break;
            }
        }

        if (!stop_word)
        {
            strcat(cleaned_output, token);
            strcat(cleaned_output, " ");
        }

        token = strtok(NULL, " ");
    }
    */
    return lowercase_output;


}
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////
int* vectorize(const char text[100]) {
    char alphabets[27] = "abcdefghijklmnopqrstuvwxyz";
    int* vector = (int*)calloc(26, sizeof(int));

    for (int i = 0; i < strlen(text); i++) {
        for (int j = 0; j < 26; j++) {
            if (text[i] == alphabets[j]) {
                vector[j] = 1;
                break;
            }
        }
    }
    return vector;
}

double matchmaker(const int vector1[26], const int vector2[26]) {
    int dot_product = 0;
    int mag_v1 = 0, mag_v2 = 0;

    for (int i = 0; i < 26; i++) {
        dot_product += vector1[i] * vector2[i];
        mag_v1 += vector1[i] * vector1[i];
        mag_v2 += vector2[i] * vector2[i];
    }

    double magnitude1 = sqrt(mag_v1);
    double magnitude2 = sqrt(mag_v2);

    if (magnitude1 == 0 || magnitude2 == 0) {
        return 0;
    }

    return (double)dot_product / (magnitude1 * magnitude2);
}

void responder(char cleaned_output[], char keywords[][20], int keyword_count) {
    int similarity = 0;
    char best_keyword[20] = "";

    char *token = strtok(cleaned_output, " ");
    while (token != NULL) {
        int* token_vector = vectorize(token); // Vector for the current token
        for (int i = 0; i < keyword_count; i++) {
            int* keyword_vector = vectorize(keywords[i]); // Vector for the keyword
            int current_similarity = matchmaker(token_vector, keyword_vector);

            if (current_similarity > similarity) {
                similarity = current_similarity;
                strcpy(best_keyword, keywords[i]); // Copy the best keyword
            }

            free(keyword_vector); // Free memory for keyword vector
        }
        printf("Token: %s Keyword: %s Similarity: %d\n", token, best_keyword, similarity);

        free(token_vector); // Free memory for token vector
        token = strtok(NULL, " ");
    }
}









