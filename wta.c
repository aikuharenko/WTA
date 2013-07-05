#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*
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

*/

#define DEBUG

const char* byte_to_binary(unsigned char x)
{
//for debug purposes
//convert unsigned char to binary form

    static char b[9];
	int j;
    b[0] = '\0';

    for (j=7;j>=0;j--)
    {
        strcat(b, ((x & (1<<j)) == (1<<j)) ? "1" : "0");
    }

    return b;
}

unsigned char* create_lookup_table(){
//create lookuptable beforehand to speedup computing hamming distanse between hashes

	int i, j, t;
	unsigned char* table = NULL;
	table = (unsigned char*) malloc(sizeof(unsigned char) * 256);	

	for (i = 0; i < 256; i++){
		int s = 0;
		for (j = 0; j < 8; j++){

			int mask = 1 << j;
			t = ((i & mask) == mask) ? 1 : 0;
			s += t;
		}
		table[i] = s;
	}
		
	return table;

}

unsigned char* compute_hash(float* a, int *perms, int N, int K, int L){
//a - input array o length L. Function computes its hash.
//N - number of spans
//K - Length of each span
//perms - N*K array of positions of elements in spans.

	int i, s, iband, stride;
	unsigned char* hash = NULL;//here will be stored M bands of encoded spans

	//compute number of bits to encode one span
	i = 1;
	int bits_per_span = 0;
	while (i < K){
		i = i * 2;
		bits_per_span++;
	}

	int W = floor(sizeof(unsigned char) * 8 / bits_per_span); //how many spans could be encoded in one unsigned char
	int M = ceil((float)N / W);//number of bands

	#ifdef DEBUG
		printf("N = %d\nK = %d\nL = %d\nbets_per_span = %d\nM = %d\nW = %d\n", N, K, L, bits_per_span, M, W);
		printf("input array: ");
		for (i = 0; i < L; i++) printf("%f ", a[i]);
		printf("\n");
	#endif	

	hash = (unsigned char*) malloc(M * sizeof(unsigned char));
	for (i = 0; i < M; i++) hash[i] = 0;

	for (s = 0; s < N; s++){
		//for each span

		//compute the position of max element
		float max = 0;
		int argmax = 0;
		for (i = 0; i < K; i++){

			float t = a[perms[i + s * K]];
			if (t > max){
				max = t;
				argmax = i;
			}

		}
		
		iband = floor(s / W); //number of current band
		stride = (s % W) * bits_per_span;
		hash[iband] = hash[iband] + (argmax << stride);

	
		#ifdef DEBUG
			if ((stride == 0) && (iband > 0)) printf("hash=%s\n\n", byte_to_binary(hash[iband-1]));
			printf("permutation positions: ");
			for (i = 0; i < K; i++) printf("%d ", perms[i + s * K]);
			printf("\nspan elements: ");
			for (i = 0; i < K; i++) printf("%f ", a[perms[i + s * K]]);
			printf("\nmax element posiotion: %d\n", argmax);
		#endif
	}

	#ifdef DEBUG
		printf("hash code: ");
		for (i = 0; i<M; i++) printf("%s ", byte_to_binary(hash[i]));
		printf("\n");
	#endif

	return hash;
}

int compare_hashes(unsigned char* h1, unsigned char* h2, unsigned char* lookup_table, int M){

	int score = 0;
	int i;
	for (i = 0; i < M; i++){
		score += lookup_table[h1[i] ^ h2[i]];
	}
	return score;

}

int* init_permutations(int N, int K, int L){

	int* perms = NULL;
	perms = malloc(N * K * sizeof(int));

	int i;
	for (i = 0; i < N * K; i++){
		 perms[i] = rand() % L;
	}
	
	return perms;
}

void quit(int* perms, unsigned char* lookup_table){

	if (perms != NULL){
		free(perms);
		perms = NULL;
	}	

	if (lookup_table != NULL){
		free(lookup_table);
		lookup_table = NULL;
	}	
}

void print_lookup_table(unsigned char* table){
//for debug
	int i;
	for (i = 0; i < 256; i++){
		printf("i=%d binary=%s sum_of_bits=%d\n", i, byte_to_binary(i), table[i]);
	}

}
