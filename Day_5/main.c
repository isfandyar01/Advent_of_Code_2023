#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"
#define MAX_SEEDS 4

int seed[MAX_SEEDS] = {0};
int seed_count = 0;

char *mapNames[] = {"seed-to-soil map:\n",
                    "soil-to-fertilizer map:\n",
                    "fertilizer-to-water map:\n",
                    "water-to-light map:\n",
                    "light-to-temperature map:\n",
                    "temperature-to-humidity map:\n",
                    "humidity-to-location map:\n"};

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


void read_seeds(char *file_contents)
{
    char *saveptr = NULL;
    char *seed_start = strchr(file_contents, ':');
    if (seed_start == NULL)
    {
        return;
    }
    seed_start++; //// Move past the ':'
    char *seed_string = strtok_s(seed_start, "\n", &saveptr);

    char *extract_seeds = strtok_s(seed_string, " ", &saveptr);
    while (extract_seeds != NULL)
    {
        seed[seed_count++] = strtoll(extract_seeds, NULL, 10);
        extract_seeds = strtok_s(NULL, " ", &saveptr);
    }
}

int main()
{


    char *contents = read_from_file();
    printf("%s", contents);


    read_seeds(contents);
    // printf("%s", contents);

    for (size_t i = 0; i < MAX_SEEDS; i++)
    {
        printf(" %d ", seed[i]);
    }


    return 0;
}
