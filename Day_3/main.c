#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"
#define MAX_GEARS 2000
#define MAX_NUMBER_LENGTH 10

typedef struct
{
    int num1;
    int num2;
    int count;
} gear;

gear Gears[2000] = {0};
int gear_count = 0;

int valid_number[6000] = {0};
int number_index = 0;
char number_storage[4];

int pot_gear_storage[10];
int pot_gear_arr_index = 0;

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

int get_full_number(char matrix[height][width], int row, int col)
{
    while (col > 0 && isdigit(matrix[row][col - 1]))
    {
        col--;
    }

    char number[MAX_NUMBER_LENGTH] = {0};
    int i = 0;
    while (col < width && isdigit(matrix[row][col]))
    {
        number[i++] = matrix[row][col++];
    }

    return atoi(number);
}

void check_adjacent_numbers(char matrix[height][width], int row, int col)
{
    int numbers[8] = {0};
    int count = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            int r = row + i;
            int c = col + j;

            if (r >= 0 && r < height && c >= 0 && c < width && isdigit(matrix[r][c]))
            {
                int num = get_full_number(matrix, r, c);
                bool duplicate = false;
                for (int k = 0; k < count; k++)
                {
                    if (numbers[k] == num)
                    {
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate)
                {
                    numbers[count++] = num;
                }
            }
        }
    }

    if (count == 2)
    {
        Gears[gear_count].num1 = numbers[0];
        Gears[gear_count].num2 = numbers[1];
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

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            if (matrix[i][j] == '*')
            {
                check_adjacent_numbers(matrix, i, j);
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
