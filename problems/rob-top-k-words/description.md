# Top K Frequent Words

Given a list of words and an integer `k`, return the `k` most frequent words. Ties in frequency are broken by **lexicographic order** (ascending — earlier letter wins).

```cpp
vector<string> topKFrequent(vector<string> words, int k);
```

The result should be ordered from most frequent to least frequent. For equal-frequency words, the lexicographically smaller word comes first.

## Example

```
topKFrequent(["i","love","leetcode","i","love","coding"], 2) → ["i","love"]
topKFrequent(["the","day","is","sunny","the","the","the","sunny","is","is"], 4)
    → ["the","is","sunny","day"]
```

## Constraints

- `1 <= words.length <= 500`
- `1 <= words[i].length <= 10`
- All characters are lowercase English letters
- `1 <= k <= unique word count`

**Optimal complexity:** O(n log k) — min-heap of size k, or O(n log n) sort.
