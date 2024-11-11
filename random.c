#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char* word;
    float weight;
} WeightedWord;

const char* STOP_WORDS[] = {
    "a", "an", "and", "are", "as", "at", "be", "by", "for",
    "from", "has", "he", "in", "is", "it", "its", "of", "on",
    "that", "the", "to", "was", "were", "will", "with"
};

// Replace the existing WEIGHTED_WORDS with NEW_WEIGHTED_WORDS
const WeightedWord WEIGHTED_WORDS[] = {
    {"assist", 3.0f},
    {"help", 3.0f},
    {"book", 2.5f},
    {"ticket", 2.5f},
    {"flight", 2.5f},
    {"cancel", 2.5f},
    {"refund", 2.5f},
    {"baggage", 2.0f},
    {"status", 2.0f},
    {"payment", 2.0f},
    {"support", 2.0f},
    {"availability", 2.0f},
    {"process", 1.5f},
    {"information", 1.5f},
    {"policy", 1.5f},
    {"charges", 1.5f},
    {"online", 1.5f},
    {"confirm", 1.5f},
    {"contact", 1.5f},
    {"issue", 1.5f},
    {"experience", 1.0f},
    {"guide", 1.0f},
    {"bulk", 1.0f},
    {"delayed", 1.0f},
    {"oversized", 1.0f},
    {"fragile", 1.0f},
    {"account", 1.0f},
    {"return", 1.0f},
    {"track", 1.0f},
    {"assist", 1.0f},
    {"ways", 1.0f},
    {"steps", 1.0f},
    {"hours", 1.0f},};

// Convert string to lowercase and remove punctuation
char* clean_string(const char* input) {
    if (!input) return NULL;

    char* cleaned = strdup(input);
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

// Check if word is a stop word
int is_stop_word(const char* word) {
    int stop_words_count = sizeof(STOP_WORDS) / sizeof(STOP_WORDS[0]);
    for (int i = 0; i < stop_words_count; i++) {
        if (strcmp(word, STOP_WORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Get weight for a word
float get_word_weight(const char* word) {
    int weighted_words_count = sizeof(WEIGHTED_WORDS) / sizeof(WEIGHTED_WORDS[0]);
    for (int i = 0; i < weighted_words_count; i++) {
        if (strcmp(word, WEIGHTED_WORDS[i].word) == 0) {
            return WEIGHTED_WORDS[i].weight;
        }
    }
    return 1.0f;  // Default weight
}

// Calculate weighted similarity score between input and question
float calculate_similarity(const char* input, const char* question) {
    char* clean_input = clean_string(input);
    char* clean_question = clean_string(question);

    if (!clean_input || !clean_question) {
        free(clean_input);
        free(clean_question);
        return 0.0f;
    }

    float score = 0.0f;
    float total_weight = 0.0f;

    // Tokenize input
    char* input_copy = strdup(clean_input);
    char* input_token = strtok(input_copy, " ");

    while (input_token) {
        // Skip stop words
        if (!is_stop_word(input_token)) {
            float word_weight = get_word_weight(input_token);
            total_weight += word_weight;

            // Look for this word in question
            char* question_copy = strdup(clean_question);
            char* question_token = strtok(question_copy, " ");

            while (question_token) {
                if (!is_stop_word(question_token)) {
                    if (strcmp(input_token, question_token) == 0) {
                        score += word_weight;
                        break;
                    }
                }
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

// Find best matching question
typedef struct {
    int index;
    float score;
} MatchResult;

MatchResult find_best_match(const char* input, char** questions, int num_questions) {
    MatchResult result = {-1, 0.0f};

    for (int i = 0; i < num_questions; i++) {
        float score = calculate_similarity(input, questions[i]);
        if (score > result.score) {
            result.index = i;
            result.score = score;
        }
    }
    return result;
}

// Main function to demonstrate the chatbot
int main() {
    char* questions[] = {
        "How can I book a flight?",
        "What is the process for making a reservation?",
        "Can I buy a ticket online?",
        "I need help with my booking.",
        "What are the options for purchasing tickets?"
    };
    int num_questions = sizeof(questions) / sizeof(questions[0]);

    char input[256];
    printf("Please enter your query: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character

    MatchResult best_match = find_best_match(input, questions, num_questions);
    if (best_match.index != -1) {
        printf("Best match: %s\n", questions[best_match.index]);
    } else {
        printf("No matching question found.\n");
    }

    return 0;
}