First i break the file contents into line by line
then i get pointer to ":" in each line and move past it once
then i retrive all the numbers 

now i have to make set of numbers first before "|" and then after "|" and then i can perform
intersection on sets and see which matches and proceed further


```
41 48 83 86 17 | 83 86  6 31 17  9 48 53
 13 32 20 16 61 | 61 30 68 82 17 32 24 19
  1 21 53 59 44 | 69 82 63 72 16 21 14  1
 41 92 73 84 69 | 59 84 76 51 58  5 54 83
 87 83 26 28 32 | 88 30 70 12 93 22 82 36
 31 18 13 56 72 | 74 77 10 23 35 67 36 11
```

i have following set of number 

i want to make array of numbers before "|" and after the "|"

what i am thinking logicaly is to use strtok to break the string at | 
and then use strol to get the numbers in string and store them in array
and again use original string and use strchr to get pointer to | and 
then simply store the numbers in array 
