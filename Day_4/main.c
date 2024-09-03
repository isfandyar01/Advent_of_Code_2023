#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"
#define MAX_NUMBERS 15

int card_numbers[MAX_NUMBERS] = {0};
int my_numbers[MAX_NUMBERS] = {0};

int card_number_count = 0;
int my_number_count = 0;

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

void process_cards(char *token)
{
    char *savepte = NULL;
    char *extract_card = NULL;
    //  Get a pointer to the first occurrence of a character in a string:
    char *cards_numbers = strchr(token, ':');
    char *my_number_set = strchr(token, '|');


    if (my_number_set != NULL)
    {
        my_number_set++; // move past "|"
    }

    if (cards_numbers != NULL)
    {
        cards_numbers++; // Move past ':'
    }
    extract_card = strtok_s(cards_numbers, "|", &savepte);

    while (extract_card != NULL)
    {
        card_numbers[++card_number_count] = strtol(extract_card, NULL, 10);
        extract_card = strtok_s(NULL, "|", &savepte);
    }


    printf("%s\n", extract_card);

    printf("%s\n", my_number_set);
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
