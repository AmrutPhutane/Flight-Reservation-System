#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAX_INPUT_LENGTH 100
#define PATH "C:\\Users\\amrut\\OneDrive\\Documents\\GitHub\\Flight-Reservation-System\\Responses.txt"


//Structure to store the Keywords
struct chadbot {
    char* keyword_string;
    int txt_reference;
};
//List of Functions Used
char* cleaner(const char text[]);
int* vectorize(char text[]);
double similarity(const int vector1[26], const int vector2[26]);
int find_keyword(char cleaned_output[], struct chadbot services[], int keywords_count);
int update(struct chadbot services[], int services_count);
char* replier(int line_number);
void betabot(int service_type);
void cleanup(struct chadbot services[], int loaded_services);

// Cleaner-> Cleans the input string...
char* cleaner(const char text[]) {

    if (!text) return NULL;

    int j = 0;
    int text_len = strlen(text);
    char* lowercase_output = (char*)malloc((text_len + 1) * sizeof(char));

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
    char* cleaned_output = (char*)calloc(text_len + 1, sizeof(char));
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
    free(lowercase_output);
    return cleaned_output;
}

// Vectorize-> Convert the given input in 26D vector...
int* vectorize(char text[]) {
    if (!text) return NULL;

    int* vector = (int*)calloc(26, sizeof(int));
    if (!vector) return NULL;

    for (int j = 0; text[j] != '\0'; j++) {
        if (text[j] >= 'a' && text[j] <= 'z') {
            vector[text[j] - 'a']++;
        }
    }
    return vector;
}

//Similarity-> Calculates the cosine value of given vectors...
double similarity(const int vector1[26 ], const int vector2[26]) {
    int dot_product = 0, mag_v1 = 0, mag_v2 = 0;

    for (int i = 0; i < 26; i++) {
        dot_product += vector1[i] * vector2[i];
        mag_v1 += vector1[i] * vector1[i];
        mag_v2 += vector2[i] * vector2[i];
    }

    double magnitude1 = sqrt(mag_v1);
    double magnitude2 = sqrt(mag_v2);
    double similarity_value = (magnitude1 == 0 || magnitude2 == 0) ? 0.0 : (double)dot_product / (magnitude1 * magnitude2);
    return similarity_value;
}

//Function that return the index of the best matched keyword...
int find_keyword(char cleaned_output[], struct chadbot services[], int keywords_count) {
    double max_similarity = 0.0;
    int best_index=-1;
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
                best_index = i;
            }
            free(vector2);
        }
        free(vector1);
        token = strtok(NULL, " ");

        if(max_similarity > 0.70)
        {
            return best_index;

        }
    }

    return -1;
}

int update(struct chadbot services[], int services_count) {
    const char* path = PATH;
    FILE* file = fopen(path, "r");
    char line[1000];
    int i = 0;
    int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
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
    const char* path = PATH;
    FILE* file = fopen(path, "r");
    char line[256];
    char* Default = "I am here to help you!!! Can you rephrase your question?";
    int current_line = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        current_line++;
        if (current_line == line_number) {
            line[strcspn(line, "\n")] = '\0';
            fclose(file);
            return strdup(line);
        }
    }
    fclose(file);
    return strdup(Default);
}


//Handling y/n responses...
void betabot(int service_type) {
    char response[MAX_INPUT_LENGTH];

    switch (service_type) {
        case 0://If user agrees to book tiket...
            printf("\nChadbot:Ayush Has Not Defined Booking Function.\n");
            break;

        case 1://If user want to see flights...
            printf("\n=== Ticket Booking Process ===\n");
            printf("Destination: ");
            fgets(response, sizeof(response), stdin);
            printf("Boarding Airport: ");
            fgets(response, sizeof(response), stdin);
            printf("Travel Date (DD/MM/YYYY): ");
            fgets(response, sizeof(response), stdin);
            printf("\nSearching for available flights...\n");
            printf("Sample flights will be displayed here->\n");
            break;

        case 2: // If user want to cancel ticket...
            printf("\n=== Ticket Cancellation ===\n");
            printf("Enter ticket number: ");
            fgets(response, sizeof(response), stdin);
            printf("Processing cancellation request...\n");
            break;
    }
}

//Frees the appended Struct...
void cleanup(struct chadbot services[], int loaded_services) {
    for (int i = 0; i < loaded_services; i++) {
        free(services[i].keyword_string);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    struct chadbot services[10];
    int loaded_services = update(services, 10);
    bool awaiting_response = false;
    int current_service = -1;

    if (loaded_services == -1) {
        printf("Error: Failed to load services.\n");
        return 1;
    }

    printf("=== Welcome to Airline Chatbot ===\n");
    printf("\nType 'exit' to quit the program.\n");

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

        if (awaiting_response) {
            if (strcasecmp(input, "y") == 0 || strcasecmp(input, "yes") == 0) {
                betabot(current_service);
            }
            else if (strcasecmp(input, "n") == 0 || strcasecmp(input, "no") == 0) {
                printf("Chadbot:Is there anything else I can help you with?\n");
            }
            else {
                printf("Chadbot:Please respond with 'yes' or 'no'.\n");
                continue;
            }
            awaiting_response = false;
            current_service = -1;
            continue;
        }

        char* cleaned_value = cleaner(input);
        int keyword_index = find_keyword(cleaned_value, services, loaded_services);

        if (keyword_index >= 0) {
            int line_number = services[keyword_index].txt_reference + 1;
            char* reply = replier(line_number);
            printf("Chadbot: %s\n", reply);
            awaiting_response = true;
            current_service = keyword_index;
            free(reply);
        } else {
            printf("Chadbot: I'm not sure I understand. Could you please rephrase that?\n");
            printf("Sometimes I fail to understand Natural Language. Please try with keywords:\n");
        }
        free(cleaned_value);
    } while (1);

    cleanup(services, loaded_services);
    return 0;
}