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

int valid_number[15] = {0};
int number_index = 0;
char number_storage[10];

size_t width = 0;
size_t height = 0;

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

bool is_symbol_around(int row_index, int column_index, char matrix_array[height][width])
{
    if (row_index < 0 || row_index >= height)
    {
        return false;
    }
    if (column_index < 0 || column_index >= width)
    {
        return false;
    }
    // search top left from index and check for symbol

    if (isdigit(matrix_array[row_index][column_index]))
    {
        return false;
    }

    if (matrix_array[row_index][column_index] == '.')
    {
        return false;
    }

    return true;
}

int main()
{

    char *contents = read_from_file();

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
    int start;
    bool symbol_found = false;
    for (size_t i = 0; i < height; i++)
    {
        // start = 0;
        size_t number_storage_index = 0;
        for (size_t j = 0; j < width; j++)
        {
            symbol_found = false;

            if (isdigit(matrix[i][j])) // first check if the number at index i and j is a digit
            {
                start = j;

                while (j < width && isdigit(matrix[i][j])) // if it is a digit then keep on looping until condition fails
                {
                    number_storage[number_storage_index] = matrix[i][j]; // store each char which is digit into the number storage buffer
                    j++;                                                 // keep moving the column forward
                    number_storage_index++;                              // and number storage buffer
                }
                number_storage[number_storage_index] = '\0'; // now the index j has a non digit char so at we are
                                                             // out of loop and we null terminate the number storage
                number_storage_index = 0;                    // we reset index so that we can check for other numbers in a row/line
                // we convert the numbers to integers and store them in array

                if (is_symbol_around(i, start - 1, matrix) ||     // Left
                    is_symbol_around(i, j - 1, matrix) ||         // Right
                    is_symbol_around(i - 1, start, matrix) ||     // Up
                    is_symbol_around(i + 1, start, matrix) ||     // Down
                    is_symbol_around(i - 1, start - 1, matrix) || // Top-left
                    is_symbol_around(i - 1, j - 1, matrix) ||     // Top-right
                    is_symbol_around(i + 1, start - 1, matrix) || // Bottom-left
                    is_symbol_around(i + 1, j - 1, matrix))       // Bottom-right
                {
                    valid_number[number_index++] = atoi(number_storage);
                    printf("%d \n", valid_number[number_index - 1]);
                }
            }
        }
    }
    return 0;
}
