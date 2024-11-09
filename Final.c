#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

char* cleaner(char* input) {
    if (!input) return NULL;

    int len = strlen(input);
    char* temp = (char*)malloc(len + 1);
    char* output = (char*)calloc((len + 1), sizeof(char));

    if (!temp || !output) {
        free(temp);
        free(output);
        return NULL;
    }

    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i]) || input[i] == ' ') {
            temp[j++] = tolower(input[i]);
        }
    }
    temp[j] = '\0';

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

    int stopwords_len = sizeof(stop_words) / sizeof(stop_words[0]);
    char* token = strtok(temp, " ");
    output[0] = '\0';  // Initialize output string

    while (token != NULL) {
        bool found = false;
        for (int i = 0; i < stopwords_len; i++) {
            if (strcmp(token, stop_words[i]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            strcat(output, token);
            strcat(output, " ");
        }
        token = strtok(NULL, " ");
    }

    // Remove trailing space if exists
    int output_len = strlen(output);
    if (output_len > 0 && output[output_len - 1] == ' ') {
        output[output_len - 1] = '\0';
    }

    free(temp);
    return output;
}

int* vectorize(char* input) {
    if (!input) return NULL;

    int* vector = (int*)calloc(26, sizeof(int));
    if (!vector) return NULL;

    for (int j = 0; input[j] != '\0'; j++) {
        if (input[j] >= 'a' && input[j] <= 'z') {
            vector[input[j] - 'a']++;
        }
    }
    return vector;
}

double similarityof(int* vector1, int* vector2) {
    if (!vector1 || !vector2) return 0.0;

    double dot_product = 0, mag_v1 = 0, mag_v2 = 0;

    for (int i = 0; i < 26; i++) {
        dot_product += vector1[i] * vector2[i];
        mag_v1 += vector1[i] * vector1[i];
        mag_v2 += vector2[i] * vector2[i];
    }

    mag_v1 = sqrt(mag_v1);
    mag_v2 = sqrt(mag_v2);
    return (mag_v1 == 0 || mag_v2 == 0) ? 0.0 : dot_product / (mag_v1 * mag_v2);
}

char* responsefor(const char* input) {
    if (!input) return NULL;

    FILE* responses = fopen("C:\\Users\\amrut\\OneDrive\\Documents\\GitHub\\Flight-Reservation-System\\Responses.txt", "r");
    if (!responses) {
        return strdup("Error: Could not open responses file.");
    }

    char line[512];
    char* best_response = NULL;
    int* v_input = vectorize((char*)input);
    double max_sim = 0.0;

    while (fgets(line, sizeof(line), responses)) {
        line[strcspn(line, "\n")] = 0;

        char* line_copy = strdup(line);
        if (!line_copy) continue;

        char* parts[3];
        parts[0] = strtok(line_copy, "\t");
        parts[1] = strtok(NULL, "\t");
        parts[2] = strtok(NULL, "\t");

        if (parts[1] && parts[2]) {
            int* v_keywords = vectorize(parts[1]);
            if (v_keywords) {
                double sim = similarityof(v_keywords, v_input);
                if (sim > max_sim && sim >= 0.60) {
                    max_sim = sim;
                    free(best_response);
                    best_response = strdup(parts[2]);
                }
                free(v_keywords);
            }
        }
        free(line_copy);
    }

    free(v_input);
    fclose(responses);

    return best_response ? best_response : strdup("I'm sorry, I couldn't find a relevant response.");
}

int main() {
    char input[100];
    printf("=== Welcome to Airline Chatbot ===\n");
    printf("\nType 'exit' to quit the program.\n");
    printf(" Chadbot: Welcome to our Airline Services... How can I help you?");

    do {
        printf("\nUser: ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Thank you for using our service. Goodbye!\n");
            break;
        }

        char* cleaned_value = cleaner(input);
        if (cleaned_value) {
            char* response = responsefor(cleaned_value);
            if (response) {
                printf("Chatbot: %s\n", response);
                free(response);
            }
            free(cleaned_value);
        }
    } while (1);

    return 0;
}
