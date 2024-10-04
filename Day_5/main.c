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
#define INITIAL_NUM_RANGES 20
#define MAX_MAPS 7


#define MIN_UINT64(a, b) ((a) < (b) ? (a) : (b))
#define MAX_UINT64(a, b) ((a) > (b) ? (a) : (b))

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
    uint64_t start_range;
    uint64_t end_range;

} Seed_Ranges;


typedef struct
{
    map *maps;
    int number_of_entries;
} map_list;

map_list all_maps[MAX_MAPS];


Seed_Ranges final_ranges[MAX_SEEDS * MAX_MAPS]; // Array to store final ranges


int new_seed_list_index = 0;
int final_ranges_count = 0;
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

void process_ranges()
{
    int current_count = 0;
    int new_count = 0;
    Seed_Ranges ranges[2][MAX_SEEDS * MAX_MAPS];
    int current = 0; // Index to track which array is current

    // Initialize first array with seed ranges
    for (int i = 0; i < MAX_SEEDS; i += 2)
    {
        ranges[0][current_count].start_range = seed[i];
        ranges[0][current_count].end_range = seed[i] + seed[i + 1] - 1;
        current_count++;
    }

    // Process each map
    for (int map_index = 0; map_index < MAX_MAPS; map_index++)
    {
        int next = 1 - current; // Index of the array to fill
        new_count = 0;

        // Process each range
        for (int i = 0; i < current_count; i++)
        {
            Seed_Ranges r = ranges[current][i];
            bool mapped = false;

            // Check against each mapping in the current map
            for (int j = 0; j < all_maps[map_index].number_of_entries; j++)
            {
                uint64_t dest = all_maps[map_index].maps[j].dest;
                uint64_t src = all_maps[map_index].maps[j].source;
                uint64_t range_length = all_maps[map_index].maps[j].range;

                uint64_t overlap_start = MAX_UINT64(r.start_range, src);
                uint64_t overlap_end = MIN_UINT64(r.end_range, src + range_length - 1);

                if (overlap_start <= overlap_end)
                {
                    // Add the mapped range to new_ranges
                    ranges[next][new_count].start_range = overlap_start - src + dest;
                    ranges[next][new_count].end_range = overlap_end - src + dest;
                    new_count++;

                    // If there's a part before the overlap, add it back to be processed
                    if (r.start_range < overlap_start)
                    {
                        ranges[current][current_count].start_range = r.start_range;
                        ranges[current][current_count].end_range = overlap_start - 1;
                        current_count++;
                    }

                    // If there's a part after the overlap, add it back to be processed
                    if (r.end_range > overlap_end)
                    {
                        ranges[current][current_count].start_range = overlap_end + 1;
                        ranges[current][current_count].end_range = r.end_range;
                        current_count++;
                    }

                    mapped = true;
                    break;
                }
            }

            // If the range wasn't mapped, add it to new_ranges unchanged
            if (!mapped)
            {
                ranges[next][new_count++] = r;
            }
        }

        // Switch current array
        current = next;
        current_count = new_count;
    }

    // Find the minimum start value
    uint64_t min_location = UINT64_MAX;
    for (int i = 0; i < current_count; i++)
    {
        if (ranges[current][i].start_range < min_location)
        {
            min_location = ranges[current][i].start_range;
        }
    }

    printf("Minimum location: %llu\n", min_location);
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
    // process_maps();
    process_ranges();
    qsort(final_location, MAX_SEEDS, sizeof(uint64_t), compare);
    printf("minium location %lld", final_location[0]);


    return 0;
}
