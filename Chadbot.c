#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_QUESTIONS 1000
#define MAX_QUESTION_LENGTH 256
#define MAX_WORD_LENGTH 50
#define MAX_STOP_WORDS 100
#define MAX_WEIGHTED_WORDS 100

// Improved error handling with logging
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Weighted Word Structure
typedef struct {
    char word[MAX_WORD_LENGTH];
    float weight;
} WeightedWord;

// Configuration Structure
typedef struct {
    char** stop_words;
    int stop_words_count;
    WeightedWord* weighted_words;
    int weighted_words_count;
} ChatbotConfig;

// Match Result Structure
typedef struct {
    int index;
    float score;
} MatchResult;

// Predefined Stop Words
const char* STOP_WORDS[] = {
    "a", "an", "and", "are", "as", "at", "be", "by", "for",
    "from", "has", "he", "in", "is", "it", "its", "of", "on",
    "that", "the", "to", "was", "were", "will", "with", "i",
    "can", "could", "would", "should", "do", "does", "did"
};
const int STOP_WORDS_COUNT = sizeof(STOP_WORDS) / sizeof(STOP_WORDS[0]);

// Predefined Weighted Words
const WeightedWord WEIGHTED_WORDS[] = {
    {"book", 3.0f},
    {"flight", 3.0f},
    {"ticket", 2.5f},
    {"reservation", 2.5f},
    {"cancel", 2.5f},
    {"refund", 2.5f},
    {"help", 2.5f},
    {"assist", 2.5f},
    {"baggage", 2.0f},
    {"luggage", 2.0f},
    {"status", 2.0f},
    {"payment", 2.0f},
    {"online", 2.0f},
    {"purchase", 2.0f},
    {"booking", 2.0f},
    {"seat", 1.5f},
    {"class", 1.5f},
    {"price", 1.5f},
    {"cost", 1.5f},
    {"change", 1.5f},
    {"modify", 1.5f},
    {"schedule", 1.5f},
    {"time", 1.0f},
    {"date", 1.0f},
    {"destination", 1.0f},
    {"route", 1.0f}
};
const int WEIGHTED_WORDS_COUNT = sizeof(WEIGHTED_WORDS) / sizeof(WEIGHTED_WORDS[0]);

// Logging Function
void log_message(LogLevel level, const char* message) {
    FILE* log_file = fopen("chatbot.log", "a");
    if (!log_file) return;

    const char* level_str[] = {"INFO", "WARNING", "ERROR"};
    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';  // Remove newline

    fprintf(log_file, "[%s] %s: %s\n", level_str[level], date, message);
    fclose(log_file);
}

// Memory-safe string duplication
char* safe_strdup(const char* str) {
    if (!str) return NULL;
    char* dup = strdup(str);
    if (!dup) {
        log_message(LOG_ERROR, "Memory allocation failed in safe_strdup");
        exit(EXIT_FAILURE);
    }
    return dup;
}

// Improved string cleaning function
char* clean_string(const char* input) {
    if (!input) return NULL;

    char* cleaned = calloc(strlen(input) + 1, sizeof(char));
    if (!cleaned) {
        log_message(LOG_ERROR, "Memory allocation failed in clean_string");
        return NULL;
    }

    int j = 0;
    for (int i = 0; input[i]; i++) {
        if (isalnum(input[i]) || input[i] == ' ') {
            cleaned[j++] = tolower(input[i]);
        } else if (ispunct(input[i])) {
            cleaned[j++] = ' ';
        }
    }
    cleaned[j] = '\0';
    return cleaned;
}

// Load configuration from predefined arrays
ChatbotConfig load_config(const char* stop_words_file, const char* weighted_words_file) {
    ChatbotConfig config = {0};

    // Use predefined stop words
    config.stop_words = malloc(STOP_WORDS_COUNT * sizeof(char*));
    for (int i = 0; i < STOP_WORDS_COUNT; i++) {
        config.stop_words[i] = safe_strdup(STOP_WORDS[i]);
    }
    config.stop_words_count = STOP_WORDS_COUNT;

    // Use predefined weighted words
    config.weighted_words = malloc(WEIGHTED_WORDS_COUNT * sizeof(WeightedWord));
    memcpy(config.weighted_words, WEIGHTED_WORDS,
           WEIGHTED_WORDS_COUNT * sizeof(WeightedWord));
    config.weighted_words_count = WEIGHTED_WORDS_COUNT;

    return config;
}

// Free configuration resources
void free_config(ChatbotConfig* config) {
    for (int i = 0; i < config->stop_words_count; i++) {
        free(config->stop_words[i]);
    }
    free(config->stop_words);
    free(config->weighted_words);
}

// Check if word is a stop word
bool is_stop_word(const char* word, ChatbotConfig* config) {
    for (int i = 0; i < config->stop_words_count; i++) {
        if (strcmp(word, config->stop_words[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Get weight for a word
float get_word_weight(const char* word, ChatbotConfig* config) {
    for (int i = 0; i < config->weighted_words_count; i++) {
        if (strcmp(word, config->weighted_words[i].word) == 0) {
            return config->weighted_words[i].weight;
        }
    }
    return 1.0f;  // Default weight
}

// Calculate weighted similarity score
float calculate_similarity(const char* input, const char* question, ChatbotConfig* config) {
    char* clean_input = clean_string(input);
    char* clean_question = clean_string(question);

    if (!clean_input || !clean_question) {
        free(clean_input);
        free(clean_question);
        return 0.0f;
    }

    float score = 0.0f;
    float total_weight = 0.0f;

    char* input_copy = safe_strdup(clean_input);
    char* input_token = strtok(input_copy, " ");

    while (input_token) {
        if (!is_stop_word(input_token, config)) {
            float word_weight = get_word_weight(input_token, config);
            total_weight += word_weight;

            char* question_copy = safe_strdup(clean_question);
            char* question_token = strtok(question_copy, " ");

            while (question_token) {
                if (!is_stop_word(question_token, config)) {
                    if (strcmp(input_token, question_token) == 0) {
                        score += word_weight;
                        break;
                    } }
                question_token = strtok(NULL, " ");
            }
            free(question_copy);
        }
        input_token = strtok(NULL, " ");
    }

    free(input_copy);
    free(clean_input);
    free(clean_question);

    return total_weight > 0 ? score / total_weight : 0.0f;
}

// Main function
int main() {
    // Remove file-based configuration loading
    // Use the predefined arrays directly
    ChatbotConfig config = load_config(NULL, NULL);

    // Example usage
    const char* user_input = "I want to book a flight";
    const char* question = "How can I book a flight?";
    float similarity_score = calculate_similarity(user_input, question, &config);

    printf("Similarity Score: %.2f\n", similarity_score);

    // Free resources
    free_config(&config);
    return 0;
}