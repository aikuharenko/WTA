WTA
======
implementation of WTA hash function from papers
"Thomas Dean Mark A. Ruzon Mark Segal Jonathon Shlens Sudheendra Vijayanarasimhan Jay YagnikFast,
 Accurate Detection of 100,000 Object Classes on a Single Machine" and
"Jay Yagnik, Dennis Strelow, David A. Ross, Ruei-sung Lin The Power of Comparative Reasoning"

Here are two main functions: compute_hash() and compare_hashes().
Hash of input array consists of N spans, encoded together. 
Each span is K random values of input array.
We encode this span with the position of max element in span.
To encode one span we need bits_per_span = ceil(log_2(K)) bits.
So in one unsigned char we can group W = (8 / bits_per_span) span codes. Together they form a band.
There will be M = ceil(N / W) bands, and they together will compose a hash.

To compare two hashes we compute a Hamming distance between them.
consider two unsigned chars: x and y. Hamming distance between them can be computed as
sum_of_bits(x xor y) and we could compute sum of bits with one operation with lookuptable.

Install instructions:
====================
3. download package: git clone https://github.com/aikuharenko/WTA.git
4. install package: sudo luarocks make wta-1.0-0.rockspec
6. run example: torch example.lua
