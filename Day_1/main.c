#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENANME "input.txt"
#define total_numbers 9
#define MAX_NUMBER_WORD_LENGTH 5


int total_sum;


char *number_to_word_map[total_numbers][2] = {
    {"one", "1"}, {"two", "2"},   {"three", "3"}, {"four", "4"}, {"five", "5"},
    {"six", "6"}, {"seven", "7"}, {"eight", "8"}, {"nine", "9"},
};


typedef struct
{
    /* data */
    bool first_digit_init;
    char digit;

} number;


int search_map_for_number(char *num, int lenght)
{

    num[lenght] = '\0';
    for (size_t i = 0; i < total_numbers; i++)
    {
        if (strcmp(number_to_word_map[i][0], num) == 0)
        {
            // printf("number %d \n\r", atoi(number_to_word_map[i][1]));
            return atoi(number_to_word_map[i][1]);
        }
    }
    return 0;
}


int check_word_is_number(char *buffer, int index)
{

    char *num = malloc(sizeof(char) * MAX_NUMBER_WORD_LENGTH);
    for (size_t i = 0; i < MAX_NUMBER_WORD_LENGTH; i++)
    {
        num[i] = buffer[index];
        index++;

        int value = search_map_for_number(num, i + 1);
        if (0 != value)
        {
            free(num);
            return value;
        }
    }
    free(num);
    return 0;
}


char *read_from_file()
{


    FILE *file = fopen(FILENANME, "r");


    if (NULL == file)
    {
        fprintf(stderr, "input file : %s: %s \n", FILENANME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);            // seek to end of file
    size_t length_of_file = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);            // seek back to beginning


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

int main()
{
    char *contents = read_from_file();

    number first_num;
    first_num.first_digit_init = false;

    number last_digit;


    // printf("%s\n", contents);


    for (size_t i = 0; contents[i] != '\0'; i++)
    {
        if (contents[i] == '\n') // checking for next line change
        {
            char num[3];
            num[0] = first_num.digit;
            num[1] = last_digit.digit;
            num[2] = '\0';
            total_sum += atoi(num);
            first_num.first_digit_init = false;
            // printf("first digit %c\n\r", first_num.digit);
            // printf("last_digit %c\n\r ", last_digit.digit);
        }


        if (isalpha(contents[i]))
        {
            /* code */
            int value = check_word_is_number(contents, i);
            if (value != 0)
            {
                if (first_num.first_digit_init == false)
                {
                    first_num.digit = value + '0';
                    first_num.first_digit_init = true;
                }
                last_digit.digit = value + '0';
            }
        }


        if (isdigit(contents[i]))
        {
            /* code */
            if (first_num.first_digit_init == false) // only setting first digit once
            {
                first_num.digit = contents[i];
                first_num.first_digit_init = true;
            }
            last_digit.digit = contents[i];
        }
    }


    printf("total sum %d\r\n", total_sum);

    free(contents);

    return 0;
}
