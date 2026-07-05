What is a order book?

An order book is a list of buy and sell order for a market, they are organized by price level and the time the order was placed

The hierarchy of an order book is:
1. Price
2. Time

What is price-time priority?

Price-time priority is a rule that states the best buy price (highest bid) and sell price (lowest ask) will be prioritized. Orders at the same price level should be executed in the order they were received.

What is the difference between a limit order and market order?

A limit order is an order in which someone orders at their worst price, either their max price they are willing to pay or lowest price they are willing to sell. A market order does not care about price and immediately executes on the best available price.

What is a partial fill and when does it happen?

A partial fill is when an order cannot be fully completed due to a lack of liquidity either side of the trade. For a limit order, the order is filled to the extent the market can accomodate. The partial quantity is filled with the rest remains in liquidity until the quantity is available.

What is the spread and what does it tell you about the market?

The spread is typically the bid-ask spread, which is the difference between what the highest bidder is willing to pay and the lowest price the seller is asking for. This can affect markets because it shows the difference between buyer and seller perception and the hidden cost related to immediate transactions, in which the person making the transaction has to settle. A higher spread is a sign of market uncertainty and lower liquidity.

What is the bid and what is the ask?

The bid is the highest price a buyer is willing to pay and the ask is the lowest price the seller would accept. The difference is the spread.

What data structure gives you the best bid (max) and best ask (min) efficiently?

We can use two balanced BSTs to hold the individual limit prices. To get the best bid and and ask, you keep track of pointers of the best bid and ask for an O(1) operation

How do you look up an order by ID in O(1)?

You would use hashmap to store the order ID as the key and the order object as the value. This gives O(1) add, delete, and lookup.

Why is a sorted structure better than an unsorted one for the order book?

A sorted structure is better than an unsorted one because it can easily acces the best bid and ask price. If the best price is exhausted, it can also easily determine the next best price. In an unsorted, there would a O(n) operation everytime to determine the best prices.

What are the tradeoffs between a std::map, a heap, and a sorted array for this use case?

A heap provides optimal lookup and access to the bet bid/ask price by keeping that at the root of the tree and maintaining proper parent/child relationships. It is also highly memory efficient. However, it doesn't maintain a larger state of the order book, and struggles with other operations with other arbitrary price levels outside of the min/max. For prices outside the root, it can have O(n) operation times, which is nad.

The std::map is a balance BST which best manages the entire order book by keeping prices in a sorted order. It easily tracks the best bid/ask price and can also insert a new price level, delete, and lookup arbitrary price values. This maintains a full scope of the entire order book. One downside of this struct is that it has higher memory overhead and cache locality issues.

The sorted array keeps the price level sorted, which allows for efficient lookup for the best price. However, operations such as insertion and deletion are not efficienet since it must shift the array down, which slows the time complexity by being O(n).

How do you maintain time priority within the same price level?

At each price level, we maintain a FIFO doubly linked list which is sorted by the time of the order. New orders placed at the existing price level will be put at the end of the doubly linked list and and as the engine matches orders at the price level, the head will move forward.

What is heap allocation and why is it slow?

Heap allocation is the dyanmic process of allocation memory in a program's global memory pool (the heap) at runtime. It is slow because it relies on metadat strucutres like freelist that search for and keep track of free space blocks, may require a system call for more memory, and can face memory fragmentation. The heap tends to face poorer cache locality due to memory fragmentation. 

Memory Fragmentation: When free memory is broken down into smaller chunks with "gaps" because data that is stored can't be fit into the gaps or is dynamically added/removed.

Cache Locality: The tendency of a program to access memory locations that are close together in space or time, making cache hits more likely and improving perfomance. 

What is a memory pool and how does it help?

A memory pool is a predetermined large block of memory that is broken into smaller chunks. Instead of requesting for memory from the system every time, it sets aside a larger block and allocates to smaller data as needed. By allocating one larger chunk, it prevents frequent reallocation and system calls for more memory, reduces memory fragmentation and as a result improves cache locality.

What is stack vs heap memory?

Stack memory is automatically managed memory used for function calls and local variables. New data is appeneded to the end of the stack and is also removed at the end as well. This prevents memory fragmentation since its placed sequentially and improves cache locality. However the stack is limited in size and is not suited for large or dynamically allocated objects

Heap memeory is a region of memeory used for dynamic allocation of memory at runtime. Metadata structures like freelists keep track of the free space in the heap, and memory is allocated to data based on the free gaps that are large enough (determined by the freelist). However, since the memory in the heap can be dynamically modified (add, remove, extend), this can lead to memory fragmentation and reduce cache locality.

What does RAII mean in C++ and why does it matter?

RAII stands for Resource Acquisition is Initialization. This is a C++ technique in which an objects creation and destruction is tied to a resource being created or destroyed. When you acquire a resource in the contstructor, you automatically release the resource in the deconstructor. This matters because it gurantees that objects in the stack and heap are destroyed when they are out of scope and their respective resources as well. This prevents the underlying resource from leaking even if there is an error or an exception is thrown. 

What is a smart pointer (unique_ptr, shared_ptr) and when do you use each?

A smart pointer is a class that is a wrapper for traditional pointers that automatically managees the lifecycle of dynamically allocated memory. It follows RAII and deallocates memeory as the pointer goes out of scope. This prevents resource leaks and dangling pointers. You use a unqiue ptr when you want one pointer to hold ownership of an object and use a shared pointer when you want multiple pointers to share an object.












