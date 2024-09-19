# Read from a file instead of stdin
with open("input.txt", "r") as file:
    seeds, *blocks = file.read().split("\n\n")  # Split file content by double newline

# Convert seeds to a list of integers
seeds = list(map(int, seeds.split(":")[1].split()))

# Process each block (map)
for block in blocks:
    ranges = []

    # Read and parse the map ranges from the block (ignoring the first line)
    for line in block.splitlines()[1:]:
        ranges.append(list(map(int, line.split())))  # Convert each line into a list of integers

    new = []  # Store updated seed mappings
    print(f"\nProcessing block:\n{block.strip()}\n")
    for x in seeds:
        original_seed = x
        for a, b, c in ranges:
            if b <= x < b + c:  # Check if seed 'x' is within the range 'b' to 'b + c'
                new.append(x - b + a)  # Map seed 'x' to the new value
                print(f"Seed {original_seed} mapped to {x - b + a} (range {b}-{b+c-1})")
                break
        else:
            new.append(x)  # If no match, keep the seed value unchanged
            print(f"Seed {original_seed} remains {x} (no match)")

    seeds = new  # Update seeds to the newly mapped values
    print(f"Updated seeds after block: {seeds}\n")

# Output the smallest mapped seed value
print(f"\nFinal minimum location: {min(seeds)}")
