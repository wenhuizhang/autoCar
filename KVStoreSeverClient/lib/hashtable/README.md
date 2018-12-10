# Hashtable

This is a linkedlist-based hashtable. It depends on the accompanying linkedlist

This implementation is based off on the one found in [phil-lopreiato/data-structures](https://github.com/phil-lopreiato/data-structures) and has been released into the license terms of this project

## Building
Running ```make``` in this directory will produce an object called ```hashtable.o``` that you can [link the object file](http://stackoverflow.com/questions/15441877/how-do-i-link-object-files-in-c-problems-with-compilation/15441915#15441915) into your own application at compile time. See the test directory for an example.

## Simplified Interface
*This is not an all-inclusive list, nor is a complete API spec. Check the header file for the most up to date information*

Using the hashtable requires the client to define two functions whose pointers are passed to the table as args. From hashtable.h:
```
/* Function pointer to calculate hash value for a node
 *
 * Parameters: char* that represents the key for the node in the table
 * Returns: an int that represents the hash value for the node,
 * however you want to calculate it
 */
#define HASH_FUNC int(*hash_func)(char* key)

/* Function pointer to a node comparison function
 *
 * Parameters: two ht_node* pointers that point to the nodes being compared
 * Returns an int depending on how the nodes compare to each other.
 * Ensure that two equal nodes produce a return value of 0
 */
#define NODE_EQUAL int(*node_equal)(struct ht_node* node1, struct ht_node* node2)
```

The simple init function (```ht_init_simple```) simply requires you to pass the number of buckets and those two function pointers (and returns a pointer to the created hashtable). Ensure that you call ```ht_free``` when you're finished to avoid memory leaks.

Nodes can be created with ```ht_create_node``` and then inserted into the table with ```ht_insert```. You can retrieve a node from the table using ```ht_lookup``` (which will return NULL if no node is found with the given key).
