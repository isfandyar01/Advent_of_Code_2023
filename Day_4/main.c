#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"
#define MAX_NUMBERS 150

int card_numbers[MAX_NUMBERS] = {0};
int my_numbers[MAX_NUMBERS] = {0};


char *read_from_file()
{

    FILE *file = fopen(FILENANME, "r");

    if (NULL == file)
    {
        fprintf(stderr, "input file : %s: %s \n", FILENANME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);         // seek to end of file
    int length_of_file = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);         // seek back to beginning

    char *buffer = malloc(sizeof(char) * (length_of_file + 1)); // +1 for null terminator

    if (NULL == buffer)
    {
        printf("failed to allocate buffer memory\n\r");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(buffer, 1, length_of_file, file);

    buffer[read_size] = '\0';

    fclose(file);

    return buffer;
}

//Comparison function
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void process_cards(char *token)
{

    int card_number_count = 0;
    int my_number_count = 0;
    char *savepte = NULL;
    char *extract_card = NULL;
    //  Get a pointer to the first occurrence of a character in a string:
    char *cards_numbers = strchr(token, ':');

    if (cards_numbers != NULL)
    {
        cards_numbers++; // Move past ':'
    }


    char *my_number_set = strchr(token, '|');


    if (my_number_set != NULL)
    {
        my_number_set++; // move past "|"
    }
    //start of demlimiter logic

    // Start of delimiter logic
    extract_card = strtok_s(cards_numbers, "|", &savepte);

    char *save_num_ptr = NULL;
    char *card_num_str = strtok_s(extract_card, " ", &save_num_ptr);

    while (card_num_str != NULL)
    {
        card_numbers[card_number_count] = strtol(card_num_str, NULL, 10);
        card_number_count++;
        card_num_str = strtok_s(NULL, " ", &save_num_ptr);
    }


    // extracting my number set


    char *my_num_save_ptr = NULL;
    char *my_num_str = strtok_s(my_number_set, " ", &my_num_save_ptr);
    while (my_num_str != NULL)
    {
        my_numbers[my_number_count] = strtol(my_num_str, NULL, 10);
        my_number_count++;
        my_num_str = strtok_s(NULL, " ", &my_num_save_ptr);
    }


    // printf("Extracted card numbers:\n");

    qsort(card_numbers, card_number_count, sizeof(int), compare);
    // for (int i = 0; i < card_number_count; i++)
    // {
    //     printf("%d ", card_numbers[i]);
    // }

    // printf("\n");
    qsort(my_numbers, my_number_count, sizeof(int), compare);

    // printf("Extracted my numbers:\n");
    // for (int i = 0; i < my_number_count; i++)
    // {
    //     printf("%d ", my_numbers[i]);
    // }

    // printf("\n");
    // printf("%s\n", my_number_set);
    int matches = 0;

    for (size_t i = 0; i < my_number_count; i++)
    {
        for (size_t j = 0; j < card_number_count; j++)
        {
            if (my_numbers[i] == card_numbers[j])
            {
                matches++;
                break;
            }
        }
    }

    int score = 0;
    if (matches > 0)
    {
        score = 1 << (matches - 1); // 1 * 2^(matches-1)
    }
    static int sum_score = 0;
    sum_score += score;
    printf("score == %d\n", sum_score);
}


int main()
{

    char *contents = read_from_file();

    // printf("%s", contents);
    char *saveptr = NULL;
    char *token = strtok_s(contents, "\n", &saveptr);
    while (token)
    {
        // printf("%s\n", token);
        process_cards(token);
        token = strtok_s(NULL, "\n", &saveptr);
    }


    return 0;
}
