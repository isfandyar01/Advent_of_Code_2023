#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"

#define number_max_charc 3

typedef enum
{
    Digit,
    Dot,
    Symbol,

} ident;

int valid_number[1000] = {0};
int number_index = 0;
char number_storage[100];

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
    size_t width = 0;
    size_t height = 0;
    for (size_t i = 0; contents[i] != '\0'; i++)
    {
        if (contents[i] == '\n')
        {
            break;
        }

        width++;
    }

    height = strlen(contents) / (width + 1);

    // printf("%zu\n %zu\n\r", width, height);

    char matrix[height][width];

    size_t height_index = 0;
    size_t w_index = 0;

    ident map_enums[height][width];

    // memset(map_enums, 0, height * width * sizeof(map_enums[0][0]));

    for (size_t i = 0; contents[i] != '\0'; i++)
    {

        if (contents[i] == '\n')
        {
            w_index = 0;
            height_index++;
            // printf("\n");
            // printf("%d", map_enums[height_index][w_index]);
        }
        else
        {
            matrix[height_index][w_index] = contents[i];
            w_index++;
            // printf("%d", map_enums[height_index][w_index]);
        }
    }

    for (size_t i = 0; i < height; i++)
    {
        int start = 0;

        size_t j = 0;
        while (j < width)
        {
            start = j;
            size_t number_storage_index = 0;
            if (isdigit(matrix[i][j]))
            {
                /* code */

                while (j < width && isdigit(matrix[i][j]))
                {
                    // printf("%c", matrix[i][j]); // Print the digit
                    number_storage[number_storage_index++] = matrix[i][j];
                    j++;
                    // number_storage_index++;
                }
                number_storage[number_storage_index] = '\0';
                valid_number[number_index++] = atoi(number_storage);
                printf("%d \n", valid_number[number_index - 1]);
            }
            else
            {
                j++;
            }

            // printf("\n");
        }
    }
    return 0;
}
