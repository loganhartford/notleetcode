# Connected Pixels

A provided Image class contains a 2D grid of monochrome pixel values.

## What you implement
Create a function going through the image and finding the size of the largest group of connected pixels.

Pixels are connected if they share an edge (above/below/left/right).

### Design discussion (worth thinking about, not graded)

- **Recursion.** A popular solution for this algorithm relies on recursion to find the set - what are the pros / cons of this strategy and what are alternatives?
- **Repeated Pixels.** An ideal solution shouldn't traverse each pixel more than once - how can we effectively track pixels that we've already seen?

**Optimal complexity:** O(r*c) - each pixel must be visited once to see if it is on/off to count or not count.
