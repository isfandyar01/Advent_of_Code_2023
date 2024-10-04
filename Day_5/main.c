#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENANME "input.txt"

#define MAX_SEEDS 2
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
    uint64_t range_start;
    uint64_t range_end;
} final_location_ranges;

final_location_ranges final_ranges[MAX_SEEDS * MAX_MAPS]; // Array to store final ranges

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

} seeds;


typedef struct
{
    map *maps;
    int number_of_entries;
} map_list;

map_list all_maps[MAX_MAPS];

seeds seed_list[MAX_SEEDS / 2];

seeds new_list[20];
int new_seed_list_index = 0;
int final_ranges_count = 0;
int map_entry_index = 0;


// Structure to represent a dynamic array LIFO (stack)
typedef struct
{
    seeds *data;  // Array to hold the structs
    int capacity; // Maximum number of elements the array can hold
    int top;      // Index of the top element (-1 if empty)
} LIFO;


LIFO *init_lifo(int initial_capacity)
{
    LIFO *lifo = (LIFO *)malloc(sizeof(LIFO));
    lifo->data = (seeds *)malloc(initial_capacity * sizeof(seeds));
    lifo->capacity = initial_capacity;
    lifo->top = -1; // Initially empty
    return lifo;
}


void push(LIFO *lifo, seeds value)
{
    if (lifo->top == lifo->capacity - 1)
    {
        // Resize the array if it is full
        lifo->capacity *= 2;
        lifo->data = (seeds *)realloc(lifo->data, lifo->capacity * sizeof(seeds));
    }
    lifo->data[++lifo->top] = value; // Increment top and add value
}


// Function to pop a final_location_ranges struct from the LIFO stack
seeds pop(LIFO *lifo)
{
    if (lifo->top == -1)
    {
        printf("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    return lifo->data[lifo->top--]; // Return value and decrement top
}


// Function to check if the LIFO stack is empty
int is_empty(LIFO *lifo)
{
    return lifo->top == -1;
}

// Free the memory used by the LIFO stack
void free_lifo(LIFO *lifo)
{
    free(lifo->data);
    free(lifo);
}


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

    for (int i = 0; i < MAX_SEEDS; i += 2)
    {
        seed_list[i / 2].start_range = seed[i];
        seed_list[i / 2].end_range = seed[i + 1] + seed_list[i / 2].start_range;
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
    LIFO *lifo = init_lifo(250);
    uint64_t rag_start = 0;
    uint64_t rag_end = 0;

    for (int z = 0; z < MAX_SEEDS / 2; z++)
    {
        push(lifo, seed_list[z]);
    }


    while (!is_empty(lifo))
    {

        uint64_t current_start = seed_list[i].start_range;
        uint64_t current_end = current_start + seed_list[i].end_range;
        rag_start = current_start;
        rag_end = current_end;

        for (int j = 0; j < MAX_MAPS; j++)
        {
            int number_entries = all_maps[j].number_of_entries;
            for (int k = 0; k < number_entries; k++)
            {
                uint64_t dest = all_maps[j].maps[k].dest;
                uint64_t src_start = all_maps[j].maps[k].source;
                uint64_t rang = all_maps[j].maps[k].range;

                uint64_t overlap_start = MAX_UINT64(current_start, src_start);
                uint64_t overlap_end = MIN_UINT64(current_end, (src_start + rang));
                if (overlap_start < overlap_end)
                { // Overlap exists
                    // Apply the mapping for the overlapping range
                    current_start = overlap_start - src_start + dest;
                    current_end = overlap_end - src_start + dest;

                    final_ranges[final_ranges_count].range_start = current_start;
                    final_ranges[final_ranges_count].range_end = current_end;
                    final_ranges_count++;
                    // Handle non-overlapping parts before the overlap
                    if (overlap_start > current_start)
                    {
                        new_list[new_seed_list_index].start_range = rag_start;
                        new_list[new_seed_list_index].end_range = overlap_start;
                        new_seed_list_index++;
                    }

                    // Handle non-overlapping parts after the overlap
                    if (current_end > overlap_end)
                    {
                        new_list[new_seed_list_index].start_range = overlap_end;
                        new_list[new_seed_list_index].end_range = rag_end;
                        new_seed_list_index++;
                    }

                    break;
                }
            }
        }
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
    // process_maps();
    process_ranges();
    qsort(final_location, MAX_SEEDS, sizeof(uint64_t), compare);
    printf("minium location %lld", final_location[0]);


    return 0;
}
