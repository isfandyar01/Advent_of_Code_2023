My approach is to first extract the number in a row and then check if the number is surrounded by a symbol
if symbol is present then store the number in a valid_number array

I am checking symbol around each digit of number and then adding them up

So for each digit i have to check 8 directions

1. up 
2. down
3. left
4. right
5. top left
6. top right
7. bottom left
8. bottom right

once symbol is found boolean is set and number is stored in a array
