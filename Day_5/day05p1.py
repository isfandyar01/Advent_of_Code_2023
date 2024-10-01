# Read from a file instead of stdin
with open("input.txt", "r") as file:
    inputs, *blocks = file.read().split("\n\n")  # Split file content by double newline


inputs = list(map(int, inputs.split(":")[1].split()))

seeds = []

for i in range(0, len(inputs), 2):
    seeds.append((inputs[i], inputs[i] + inputs[i + 1]))


# print(blocks)

for block in blocks:
    ranges = []
    for line in block.splitlines()[1:]:
        ranges.append(list(map(int, line.split())))
    print(ranges)
    new = []
    while len(seeds) > 0:
        s, e = seeds.pop()
        # print(s)
        # print(e)
        for a, b, c in ranges:
            os = max(s, b)
            oe = min(e, b + c)
            # print("os",os)
            # print("oe",oe)
            if os < oe:
                new.append((os - b + a, oe - b + a))
                if os > s:
                    seeds.append((s, os))
                if e > oe:
                    seeds.append((oe, e))
                break
        else:
            new.append((s, e))
    seeds = new

# print((seeds))
