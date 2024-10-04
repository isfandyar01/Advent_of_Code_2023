this was the hardest problem so far

i have multiple issue 

first i stored the seed into array

then i process maps into structs

but issue is that for each map there are number of entries e.g
seed-to-soil map:
50 98 2
52 50 48

has two entries 

so what i did was make a 2d array of struct 

row represent it each map and column represent entries of map


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

there can max 7 maps and for each map there can be mulitple entries so coulmns are not defined in the 2d array


row represents the maps soil fertilizer water etc and they are total 7 in numbers

while column of struct array represent map data 


so basically am creating a list like this
   col1      col 2
{[50 98 2],[52 50 48]} map row 1


number of entries tells me how many columns i need to process 


## PART TWO

k so i think i can solve this problem like this

I have made a struct in C which contains 

seed_start
seed_range

i can use them to calculate a range 

seed_start=seed_start;
seed_end = seed_start+seed_range-1;

then i can use map to find out whether seed start and end intersect with map 

for that i need to have maximum of seed_start and map_start
stored in overlap_start
and minimum of seed_end and map_start+map_range
stored in overlap end

if overlap start is less than overlap end then it means that range is over lapping
and i can transform that range according to mapping algo

and if the range is not overlapping i.e overlap_start is greater than seed start and overlap_end is less than seed_end than that range is stored as it is in final_map and once done i can simple pick first index of final_map after sorting it out using qsort for minimum


I was not able to solve it myself claude ai solved it before i Can 


I understand your confusion. Let's break down the code and explain these concepts step by step:

Switching of current array:

The code uses two arrays to process the ranges efficiently. Let's explain why and how:
Seed_Ranges ranges[2][MAX_SEEDS * MAX_MAPS];
int current = 0; // Index to track which array is current

We have two arrays (ranges[0] and ranges[1]) to avoid copying data back and forth.
current keeps track of which array we're currently reading from.

for (int map_index = 0; map_index < MAX_MAPS; map_index++)
{
    int next = 1 - current; // Index of the array to fill
    // ... processing ...
    current = next;
}

For each map, we read from ranges[current] and write to ranges[next].
After processing all ranges for a map, we switch current to next.
This swapping technique allows us to efficiently move between input and output without copying entire arrays.


The mapped variable:

bool mapped = false;
// ... in the mapping loop ...
if (overlap_start <= overlap_end)
{
    // ... process overlap ...
    mapped = true;
    break;
}

mapped keeps track of whether a range has been transformed by any mapping in the current map.
If a range overlaps with a mapping, we set mapped = true and break the inner loop.
After the inner loop:

if (!mapped)
{
    ranges[next][new_count++] = r;
}

If mapped is still false, it means the range wasn't transformed by any mapping in this map.
In this case, we copy the range unchanged to the next array.

This approach ensures that:

Ranges that overlap with mappings are transformed.
Ranges that don't overlap with any mapping are passed through unchanged.
Ranges are split when they partially overlap with a mapping.

The switching of arrays and the mapped flag work together to efficiently process all ranges through all maps, handling transformations and splits as needed.
Does this explanation help clarify these aspects of the code? Let me know if you'd like me to elaborate on any part further.
