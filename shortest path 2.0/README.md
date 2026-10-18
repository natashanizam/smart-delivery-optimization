# Smart Delivery Optimization

A small C++ project that combines three classic algorithms into one
delivery scenario:

| Part | Algorithm | Job in the story |
|------|-----------|------------------|
| Fragile / indivisible cargo | 0/1 Knapsack (DP) | Pick the most valuable items that fit in the vehicle |
| Divisible cargo (rice, oil...) | Fractional Knapsack (greedy) | Fill the leftover space, partial loads allowed |
| Road network | Prim's MST | Cheapest set of roads connecting every city |
| Delivery routes | Dijkstra | Shortest driving route from the warehouse to each city |

The 0/1 step runs first. Whatever capacity is left over goes to the
fractional step, because divisible goods can fill any gap exactly.

## Build and run

Works with old MinGW too (GCC 6): the code only needs C++11.

```
g++ -std=c++11 -Wall -Wextra main.cpp Knapsack.cpp MST.cpp Dijkstra.cpp -o delivery
delivery                 # built-in sample data
delivery data.txt        # data from a file

g++ -std=c++11 -Wall -Wextra tests.cpp Knapsack.cpp MST.cpp Dijkstra.cpp -o tests
tests
```

`main.cpp` and `tests.cpp` both define `main()`, so they are always
built as two separate programs.

## Input file format

```
capacity 10
items 5
Laptop 3 1000 0        name weight value divisible(0/1)
...
cities 5
Warehouse              first city is the warehouse (route source)
City A
...
edges 7
0 1 2                  city index, city index, distance (km)
...
```

Invalid input (negative weights, bad indices, missing sections) gives a
clear error message instead of a crash.

## Complexity

| Function | Time | Space |
|----------|------|-------|
| `knapsack01` | O(n * W) | O(n * W) |
| `fractionalKnapsack` | O(n log n) | O(n) |
| `primMST` | O(E log V) | O(V + E) |
| `dijkstra` | O((V + E) log V) | O(V + E) |

n = number of items, W = capacity, V = cities, E = roads.

## Design notes and edge cases

- Items with weight <= 0 or value < 0 are ignored (no divide by zero).
- Capacity 0, empty lists and nothing-fits all return an empty result.
- The fractional sort compares ratios by cross-multiplication, so there
  is no rounding error.
- `addEdge` rejects negative weights and out-of-range cities, which
  both Prim's and Dijkstra assume.
- On a disconnected network `primMST` reports `connected = false` and
  `dijkstra` marks unreachable cities with `INF`.

## Files

`Item.h` item struct, `Graph.h` graph type and `addEdge`,
`Knapsack.*` both knapsacks, `MST.*` Prim's, `Dijkstra.*` shortest
paths, `main.cpp` the program, `tests.cpp` unit tests, `data.txt`
sample input.

## Run with this
g++ -std=c++11 main.cpp Knapsack.cpp MST.cpp Dijkstra.cpp -o delivery.exe
.\delivery.exe 
