# Search Suggestions System

Given a sorted list of product names and a search word, return up to 3 lexicographically smallest product suggestions after each prefix of the search word.

```cpp
vector<vector<string>> suggestedProducts(vector<string> products, string searchWord);
```

Returns a vector of size `searchWord.length()`. Each entry contains up to 3 products that share the current prefix, in lexicographic order.

## Example

```
products = ["mobile","mouse","moneypot","monitor","mousepad"]
searchWord = "mouse"

After "m"     → ["mobile","moneypot","monitor"]
After "mo"    → ["mobile","moneypot","monitor"]
After "mou"   → ["mouse","mousepad"]
After "mous"  → ["mouse","mousepad"]
After "mouse" → ["mouse","mousepad"]
```

## Constraints

- `1 <= products.length <= 1000`
- `1 <= products[i].length <= 3000`
- All characters are lowercase English letters
- `1 <= searchWord.length <= 1000`

**Optimal complexity:** O(n log n + n*L + Q*L) — sort products, then binary search or two-pointer for each prefix.
