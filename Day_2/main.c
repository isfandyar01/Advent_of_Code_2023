#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENANME "input.txt"
#define RED_T 12
#define GREEN_T 13
#define BLUE_T 14


int total_sum = 0;

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

void trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces
        return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator
    *(end + 1) = '\0';
}

int process_game(char *game)
{

    // printf("game string %s\n", game);
    int game_id = 0;


    const char *color_set[3] = {"blue", "red", "green"};

    const int max_color_values[3] = {14, 12, 13}; // brg


    int min_required[3] = {0};

    sscanf_s(game, "Game %d", &game_id);
    printf("%d\n", game_id);
    // get the game sets Get a pointer to the first occurrence of a character in a string:
    char *game_strs = strchr(game, ':');
    if (game_strs != NULL)
    {
        game_strs++; // Move past ':'
    }
    char *saveptr1;
    // printf("%s\n", game_strs);
    game_strs = strtok_r(game_strs, ";", &saveptr1);


    bool game_possible = true;

    while (game_strs != NULL)
    { // gets the substring after delimiter


        char *set_next_set_read_pointer = game_strs;

        while (set_next_set_read_pointer != NULL)
        {
            int color_count = 0;
            char color[10] = {0};

            sscanf_s(set_next_set_read_pointer, " %d %s", &color_count, color);

            trim_whitespace(color);
            for (size_t color_index = 0; color_index < 3; color_index++)
            {

                if (strncmp(color, color_set[color_index], strlen(color_set[color_index])) == 0)
                {
                    // printf("color %s\n", color);
                    // printf("color count %d\n", color_count);
                    if (color_count > max_color_values[color_index])
                    {
                        game_possible = false;
                    }
                }
            }


            set_next_set_read_pointer = strchr(set_next_set_read_pointer, ',');
            if (set_next_set_read_pointer != NULL)
            {
                set_next_set_read_pointer++;
            }
        }

        game_strs = strtok_r(NULL, ";", &saveptr1);
        // break;
    }

    if (game_possible)
    {
        /* code */ total_sum += game_id;
        // printf("sum of game id %d \n", game_id);
    }
    return total_sum;
}

int main()
{
    int total = 0;
    char *contents = read_from_file();
    char *game_part;
    // printf("%s\n", contents);
    trim_whitespace(contents);
    game_part = strtok(contents, "\n");

    while (game_part != NULL)
    {


        total = process_game(game_part);


        game_part = strtok(NULL, "\n");
    }

    printf("total_sum %d\n", total);
    return 0;
}
