# Pixel mapping

## Giving coordinates

![Add image]()

- Each node has a coordinate wich will be calculated when device discovery is active
- Root node (Controller) starts with [0,0]
- On adding a new node add values to coordinate respecting the table below

| Side         | a        | b          | c          | d        | e          | f          |
|--------------|----------|------------|------------|----------|------------|------------|
| to add value | [+0, +1] | [+1, +0.5] | [+1, -0.5] | [+0, -1] | [-1, -0.5] | [-1, +0.5] |

## Find highest, lowest, most right and most left

16-bit numbers first 16 are x-cor, second are  y-cor
Keep track of highes and lowes x and y value to know how big matrix will be

a: 
