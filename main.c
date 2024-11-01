#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

char* cleaner(char text[]) {
    int j = 0;
    char lowercase_output[100];
    char cleaned_output[100] = "";

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i]) || text[i] == ' ') {
            lowercase_output[j++] = tolower(text[i]);
        }
    }
    lowercase_output[j] = '\0';

    const char* stop_words[] = {
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

    char* token = strtok(lowercase_output, " ");
    while (token != NULL) {
        bool is_stop_word = false;

        for (int i = 0; i < stopword_count; i++) {
            if (strcmp(token, stop_words[i]) == 0) {
                is_stop_word = true;
                break;
            }
        }

        if (!is_stop_word) {
            strcat(cleaned_output, token);
            strcat(cleaned_output, " ");
        }

        token = strtok(NULL, " ");
    }

    cleaned_output[strlen(cleaned_output) - 1] = '\0';  // Remove trailing space

    static char final_output[100];
    strcpy(final_output, cleaned_output);
    return final_output;
}
int* vectorize(char text[]) {
    int* vector = (int*)calloc(26, sizeof(int));
    for (int j = 0; text[j] != '\0'; j++) {
        if (text[j] >= 'a' && text[j] <= 'z') {
            vector[text[j] - 'a']++;
        }
    }
    return vector;
}

double similarity(const int vector1[26], const int vector2[26]) {
    int dot_product = 0, mag_v1 = 0, mag_v2 = 0;

    for (int i = 0; i < 26; i++) {
        dot_product += vector1[i] * vector2[i];
        mag_v1 += vector1[i] * vector1[i];
        mag_v2 += vector2[i] * vector2[i];
    }

    double magnitude1 = sqrt(mag_v1);
    double magnitude2 = sqrt(mag_v2);
    return (magnitude1 == 0 || magnitude2 == 0) ? 0.0 : dot_product / (magnitude1 * magnitude2);
}

char* find_keyword(char cleaned_output[], char keywords_list[][100], int keywords_count) {
    double max_similarity = 0.0;
    char* keyword = (char*)calloc(100, sizeof(char));
    char cleaned_copy[100];
    strcpy(cleaned_copy, cleaned_output);

    char* token = strtok(cleaned_copy, " ");
    while (token != NULL) {
        int* vector1 = vectorize(token);
        for (int i = 0; i < keywords_count; i++) {
            int* vector2 = vectorize(keywords_list[i]);
            double sim = similarity(vector1, vector2);
            if (sim > max_similarity) {
                max_similarity = sim;
                strncpy(keyword, keywords_list[i], 100);
            }
            free(vector2);
        }
        free(vector1);
        token = strtok(NULL, " ");
    }
    return keyword;
}



int main() {
    char text[100] = "Can i talk to the customer support?";
    char* cleaned_value = cleaner(text);
    char keywords_list[][100] = {
        "cancel cancellation abort", "status confirmation", "baggage allowance weight carry luggage",
        "refund reimbursement", "payment billing charge receipt", "feedback complaint review issue",
        "customer contact support help assistance", "hello hi greetings hey", "book booking reserve",
        "status flight arrival departure delay"
    };
    int keywords_count = sizeof(keywords_list) / sizeof(keywords_list[0]);
    char* matched_keyword = find_keyword(cleaned_value, keywords_list, keywords_count);
    printf("For the user input: %s\n", text);
    printf("Best matching keyword: %s\n", matched_keyword);
    free(matched_keyword);

    return 0;
}
