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
        // List of stop words
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

    cleaned_output[strlen(cleaned_output) - 1] = '\0';

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

    // Calculate the similarity
    double similarity_value = (magnitude1 == 0 || magnitude2 == 0) ? 0.0 : (double)dot_product / (magnitude1 * magnitude2);

    // Return similarity only if it's 0.5 or more, otherwise return 0
    return (similarity_value >= 0.6) ? similarity_value : 0.0;
}

struct chadbot {
    char* keyword_string;
    int txt_reference;
};

int find_keyword(char cleaned_output[], struct chadbot services[], int keywords_count) {
    double max_similarity = 0.0;
    int best_index = -1; // Initialize to -1 to indicate no match found
    char cleaned_copy[100];
    strcpy(cleaned_copy, cleaned_output);

    char* token = strtok(cleaned_copy, " ");
    while (token != NULL) {
        int* vector1 = vectorize(token);
        for (int i = 0; i < keywords_count; i++) {
            int* vector2 = vectorize(services[i].keyword_string);
            double sim = similarity(vector1, vector2);
            if (sim > max_similarity) {
                max_similarity = sim;
                best_index = i; // Store the index of the best match
            }
            free(vector2);
        }
        free(vector1);
        token = strtok(NULL, " ");
    }

    return best_index; // Return the index of the best match, or -1 if no match found
}

int update(struct chadbot services[], int services_count) {
    const char* path = "C:\\Users\\amrut\\OneDrive\\Documents\\GitHub\\Flight-Reservation-System\\Responses.txt";
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[1000];
    int i = 0;
    int line_number = 0;

    // Read keywords into services
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;

        // Check if the line number is a multiple of 5
        if (line_number % 5 == 0 && i < services_count) {
            line[strcspn(line, "\n")] = '\0';
            services[i].keyword_string = strdup(line);
            services[i].txt_reference = line_number;
            i++;
        }
    }
    fclose(file);
    return i;
}


char* replier(int line_number) {
    const char* path = "C:\\Users\\amrut\\OneDrive\\Documents\\GitHub\\Flight-Reservation-System\\Responses.txt";
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;  // Return NULL on failure to open the file
    }

    char line[256];  // Buffer to hold each line
    int current_line = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        current_line++;

        // Check if the current line matches the requested line number
        if (current_line == line_number) {
            line[strcspn(line, "\n")] = '\0';  // Remove newline character
            fclose(file);  // Close the file before returning
            return strdup(line);  // Return a dynamically allocated copy of the line
        }
    }

    fclose(file);
    return strdup("Can you rephrase?");  // Return a default reply if not found
}

int main() {
    char text[100] = "hahahahahha";
    char* cleaned_value = cleaner(text);
    int services_count = 10;

    struct chadbot services[10];
    int loaded_services = update(services, services_count);
    if (loaded_services == -1) {
        return 1;
    }

    for (int i = 0; i < loaded_services; i++) {
        printf("Service %d Keywords: %s\n", i + 1, services[i].keyword_string);
    }

    int keyword_index = find_keyword(cleaned_value, services, loaded_services);
    printf("For the user input: %s\n", text);
    printf("Best matching keyword: %d\n", keyword_index);


    int line_number = services[keyword_index].txt_reference + 1;
    char* reply = replier(line_number);
    printf("Reply from chatbot: %s\n", reply);

    free(keyword_index);

    return 0;
}
