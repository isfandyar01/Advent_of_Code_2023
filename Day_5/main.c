#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"

#define MAX_SEEDS 20
#define MAX_MAPS 7


uint64_t seed[MAX_SEEDS] = {0};
int seed_count = 0;

uint64_t final_location[MAX_SEEDS];


char *mapNames[] = {"seed-to-soil map:\n",
                    "soil-to-fertilizer map:\n",
                    "fertilizer-to-water map:\n",
                    "water-to-light map:\n",
                    "light-to-temperature map:\n",
                    "temperature-to-humidity map:\n",
                    "humidity-to-location map:\n"};


typedef struct
{
    uint64_t dest;
    uint64_t source;
    uint64_t range;

} map;

typedef struct
{
    map *maps;
    int number_of_entries;
} map_list;

map_list all_maps[MAX_MAPS];

int map_entry_index = 0;

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


void read_seeds()
{
    char *file_contents = read_from_file();
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


void read_maps()
{
    uint64_t temp[3] = {0};
    char *file_contents = read_from_file(); // Assuming this reads your data correctly


    for (int i = 0; i < MAX_MAPS; i++)
    {
        int number_entries = 0;
        char *map_start = strstr(file_contents, mapNames[i]);
        if (map_start == NULL)
        {
            printf("Couldn't find map: %s\n", mapNames[i]);
            continue;
        }

        // Move to the start of the data (next line)
        map_start = strchr(map_start, '\n');
        if (map_start == NULL)
        {
            continue;
        }
        map_start++; // numbers started
        char *line_start = map_start;

        // Read entries for this map until we hit the next map or the end of the file
        char *next_map_start = NULL;
        if (i < MAX_MAPS - 1)
        {
            // If there is a next map, find the start of the next map section
            next_map_start = strstr(map_start, mapNames[i + 1]);
            next_map_start--;
        }
        else
        {
            // If there is no next map (i.e., this is the last map), set it to NULL
            next_map_start = NULL;
        }
        // next_map_start--;

        // Count the number of entries in the current map

        while (line_start < next_map_start || (next_map_start == NULL && *line_start != '\0'))
        {
            sscanf(line_start, "%d %d %d", &temp[0], &temp[1], &temp[2]);

            line_start = strchr(line_start, '\n');
            if (line_start == NULL)
            {
                break; // End of the file or section
            }
            number_entries++;
            line_start++; // Move to the next line
        }

        // Reset the pointer to start reading data again
        all_maps[i].maps = malloc(number_entries * sizeof(map));
        all_maps[i].number_of_entries = number_entries;

        line_start = map_start;
        int entry_index = 0;
        while (line_start < next_map_start || (next_map_start == NULL && *line_start != '\0'))
        {

            sscanf_s(line_start, "%d %d %d", &temp[0], &temp[1], &temp[2]);

            all_maps[i].maps[entry_index].dest = temp[0];
            all_maps[i].maps[entry_index].source = temp[1];
            all_maps[i].maps[entry_index].range = temp[2];
            entry_index++;

            line_start = strchr(line_start, '\n');
            if (line_start == NULL)
            {
                break;
            }


            // maps[map_entry_index].dest = temp[0];
            // maps[map_entry_index].source = temp[1];
            // maps[map_entry_index].range = temp[2];
            // map_entry_index++;

            line_start++;
        }

        file_contents = (next_map_start != NULL) ? next_map_start : (line_start + strlen(line_start));
    }
}


void process_maps()
{
    for (int sed = 0; sed < seed_count; sed++)
    {
        uint64_t current_seed = seed[sed];

        for (int i = 0; i < MAX_MAPS; i++)
        {
            int number_entries = all_maps[i].number_of_entries;

            for (int j = 0; j < number_entries; j++)
            {
                uint64_t dest = all_maps[i].maps[j].dest;
                uint64_t src = all_maps[i].maps[j].source;
                uint64_t rang = all_maps[i].maps[j].range;


                if (src <= current_seed && current_seed < src + rang)
                {
                    // printf("seed in range \n");
                    uint64_t offset = current_seed - src;
                    current_seed = dest + offset;
                    break;
                }
            }
        }
        final_location[sed] = current_seed;
    }
}


//Comparison function
// Comparison function for qsort
int compare(const void *a, const void *b)
{
    if (*(uint64_t *)a < *(uint64_t *)b)
        return -1;
    if (*(uint64_t *)a > *(uint64_t *)b)
        return 1;
    return 0;
}

int main()
{


    read_seeds();
    read_maps(); /* code */
    process_maps();
    qsort(final_location, MAX_SEEDS, sizeof(uint64_t), compare);

    printf("minium location %lld", final_location[0]);


    return 0;
}
