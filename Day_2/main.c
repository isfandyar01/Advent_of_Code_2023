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

    static int total_sum = 0;
    int game_id = 0;


    const char *color_set[3] = {"blue", "red", "green"};

    const int max_color_values[3] = {14, 12, 13}; // brg


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
    game_strs = strtok_s(game_strs, ";", &saveptr1);


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

        game_strs = strtok_s(NULL, ";", &saveptr1);
        // break;
    }

    if (game_possible)
    {
        /* code */ total_sum += game_id;
        // printf("sum of game id %d \n", game_id);
    }
    return total_sum;
}


/*
when you call strtok it makes an internal thread and when in next loop you call
strtok it will fail after one interation only thats when in process_game we use
strtok_r which restarts and previous strtok keeps running

also strncmp wont work if you have whitespaces in data

u can use sscanf_s to get the relevant item from the string which is not a user input
also i forgot to make sure that total sum was a static in process_game so i did that as well

also strtok_s is a safer version of strtok so using that is ok as well

*/

int main()
{
    int total = 0;
    char *contents = read_from_file();
    char *game_part;
    // printf("%s\n", contents);
    char *save_game_part;
    game_part = strtok_s(contents, "\n", &save_game_part);

    while (game_part != NULL)
    {


        total = process_game(game_part);


        game_part = strtok_s(NULL, "\n", &save_game_part);
    }

    printf("total_sum %d\n", total);
    return 0;
}
