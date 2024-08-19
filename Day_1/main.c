#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENANME "input.txt"


int total_sum;


typedef struct
{
    /* data */
    bool first_digit_init;
    char digit;

} number;


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


    printf("%s\n", contents);


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
            printf("first digit %c\n\r", first_num.digit);
            printf("last_digit %c\n\r ", last_digit.digit);
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
