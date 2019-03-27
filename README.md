# B+ Tree & LinearHashing

## B+ Tree

* M = Number of Buffers
* B = Size of Buffer
* Keys = ceil(B + 1)/12 (Max keys that can be present in one node)

### prerequisite

* C++
* How to compile and run code
* `g++ BTree.cpp -o BTree`
* `./BTree.out filename M B`
* It accepts three command line argument
1. InputfileName 
2. Number of Buffers 
3. Size of Buffer
* In above command input filename contains all queries like insert key,find key,count etc..
* It executes all queries that are present in the file and store output of each query in output.txt file.

* For calculating degree of b+tree. Assuming that block size = buffer size. If each block has space for n keys and n+1 pointers then size occupied = 4*(n) + 8*(n+1) if keys are integers. Selecting the maximum n such that 4*n+8*(n+1) <= block size

## Linear Hashing

* Block size = Buffer size(B)
* Let there be n blocks and r records then the occupancy = (r/(n*r1))*100 , where r1 is maximum records in a block (b/4).

### prerequisite

* C++
* How to compile and run code
* `g++ linear_hashing.cpp -o hash`
* `./hash.out filename M B`
* It accepts three command line argument
1. InputfileName 
2. Number of Buffers 
3. Size of Buffer
* It executes all queries that are present in the file and store output of each query in output.txt file.