#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"

typedef struct
{
    int num1;
    int num2;
    int count;
} gear;

gear Gears[2000] = {0};
int gear_count = 0;

int valid_number[6000] = {0};

char number_storage[4];

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

void get_num(int row_index, int column_index, char matrix_array[height][width])
{
    int row = row_index;
    int column = column_index;

    // Define directions to check
    int directions[8][2] = {
        {-1, -1}, // Top-left
        {-1, 0},  // Up
        {-1, 1},  // Top-right
        {0, -1},  // Left
        {0, 1},   // Right
        {1, -1},  // Bottom-left
        {1, 0},   // Down
        {1, 1}    // Bottom-right
    };

    int found_numbers[2] = {0}; // Store found numbers
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        int new_column = column + directions[i][1]; // Column index  vector tranpose maths
        int new_row = row + directions[i][0];       // Row index

        // Check if the new coordinates are within bounds
        if (new_column >= 0 && new_column < width && new_row >= 0 && new_row < height && isdigit(matrix_array[new_row][new_column]))
        {
            // Check if there's a digit at the new position

            // Start extracting the number
            // Storage for the number
            int idx = 0;

            // Find the start of the number by moving left
            int start_column = new_column;
            while (start_column > 0 && isdigit(matrix_array[new_row][start_column - 1]))
            {
                start_column--; // Move left to find the start of the number
            }

            // Collect the digits into number_storage
            while (start_column < width && isdigit(matrix_array[new_row][start_column]))
            {
                number_storage[idx++] = matrix_array[new_row][start_column];
                start_column++;
            }
            number_storage[idx] = '\0'; // Null-terminate the string

            // Convert the found number to integer
            int found_number = atoi(number_storage);

            // Check if the number is not already found
            int is_duplicate = 0;
            for (int j = 0; j < count; j++)
            {
                if (found_numbers[j] == found_number)
                {
                    is_duplicate = 1;
                    break;
                }
            }

            // Store the found number if it's not a duplicate
            if (!is_duplicate)
            {
                if (count < 2)
                { // Store only the first two unique numbers
                    found_numbers[count++] = found_number;
                }
            }

            // Stop if two unique numbers are found
            if (count == 2)
            {
                break;
            }
        }
    }

    // Assign found numbers to the output parameters
    if (count == 2)
    {
        Gears[gear_count].num1 = found_numbers[0];
        Gears[gear_count].num2 = found_numbers[1];
        Gears[gear_count].count = count;
        gear_count++;
    }
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

    char matrix[height][width];

    size_t height_index = 0;
    size_t w_index = 0;

    for (size_t i = 0; contents[i] != '\0'; i++)
    {

        if (contents[i] == '\n')
        {
            w_index = 0;
            height_index++;
        }
        else
        {
            matrix[height_index][w_index] = contents[i];
            w_index++;
        }
    }
    int ans;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            if (matrix[i][j] == '*')
            {

                get_num(i, j, matrix);
            }
        }
    }
    long long total_gear_ratio = 0;
    for (int i = 0; i < gear_count; i++)
    {
        total_gear_ratio += (long long)Gears[i].num1 * Gears[i].num2;
    }

    printf("Total gear ratio: %lld\n", total_gear_ratio);
    return 0;
}
