require 'torch'
local ffi = require 'ffi'
require 'torchffi'
require 'image'

ffi.cdef[[

	unsigned char* compute_hash(float* a, int* spans, int N, int K, int L);
	void quit(int* perms, unsigned char*);
	int *init_permutations(int N, int K, int L);
	unsigned char* create_lookup_table();
	void print_lookup_table(unsigned char* table);
	int compare_hashes(unsigned char* h1, unsigned char* h2, unsigned char* lookup_table, int M);

]]

wta = {}
local cwta = ffi.load("wta")
wta.c = cwta

function getM(N, K)

	local i = 1
	local bits_per_span = 0;
	while (i < K) do
		i = i * 2;
		bits_per_span = bits_per_span + 1
	end

	W = math.floor(8 / bits_per_span); 
	M = math.ceil(N / W);

	return M

end

function wta.init(N, K, L)
-- N - number of spans
-- K - number of elements in each span
-- L - length of encoded array

	wta.K = K
	wta.N = N
	wta.L = L
	wta.M = getM(N, K);
	
	--precompute positions of elements in spans
	wta.perms = wta.c.init_permutations(N, K, L)
	
	--precompute lookup table
	wta.lookup_table = wta.c.create_lookup_table()

end

function wta.compute_hash(patch)
--compute hash of patch
--length of patch should be L.

	local patch_cont = patch:type('torch.FloatTensor'):contiguous()
	local patch_c = torch.data(patch_cont)
	local hash = wta.c.compute_hash(patch_c, wta.perms, wta.N, wta.K, wta.L)
	return hash

end

function wta.compare_hashes(h1, h2)

	local score = wta.c.compare_hashes(h1, h2, wta.lookup_table, wta.M)
--	print(score)
	local lua_score = torch.Tensor(score)
	return score

end

function wta.quit()
--free memory
	wta.c.quit(wta.c.perms, wta.c.lookup_table)

end

