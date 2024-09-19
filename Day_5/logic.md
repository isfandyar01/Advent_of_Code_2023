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
