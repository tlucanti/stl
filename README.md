# STL
An implementation of c++ Standard Template Library containers with test and benchmark suite


## containers
### vector
Dynamically allocated storage with contiguously stored elements.
 - access: `O(1)`
 - insertion or removal from the end: `O(1)`
 - insertion or removal not from end: `O(n)`

### stack
Adapter over any container with LIFO interface.
 - top access: `O(1)`
 - insertion or removal from the end: `O(1)`

### set
Ordered set of elements over red-black tree
 - access: `O(log(n))`
 - insertion or removal: `O(log(n))`

### map
Ordered key-value storage over red-black tree
 - access: `O(log(n))`
 - insertion or removal: `O(log(n))`

### pair
Wrapper for storing any typed data pairs stored in `first` and `second` fields

### ctree
c-version structure of red-black tree, same as c++ set version
 - access: `O(log(n))`
 - insertion or removal: `O(log(n))`

## test suite
all containers has unittests for every method or functions

## benchmark suiote
all containers has benchmars for compairng performance with std c++ containers
