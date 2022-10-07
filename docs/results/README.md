# Results

This directory contains some examples of results using this software module.

## Circuit 1 - circuit without nodes

- [Results](./circuit-1/README.md)

## Circuit 2 - circuit with a single node

- [Results](./circuit-2/README.md)

## Circuit 3 - circuit with two nodes

- [Results](./circuit-3/README.md)

## Circuit 4 - complex circuit with multiple nodes

- [Results](./circuit-4/README.md)

## Limitations

1. Image dimensions affects the segmentation results, due to the constant values for kernel sizes used in morphological transformations
    - [Example 1](./limitations/example-1/) shows an example using the circuit 2 with larger image
1. Labels very close to the components can lead to these labels being considered as belonging to the components
    - [Example 2](./limitations/example-2/) shows an example of this limitation
1. Labels close to nodes may not be associated with nodes but with close connections, due to the way nodes are detected
    - [Example 2](./limitations/example-2/) shows an example of this limitation
1. Components that are very close to other components can be considered as being the same component
