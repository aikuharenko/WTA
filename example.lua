require 'wta'
patch1 = torch.rand(3,3)
patch2 = torch.rand(3,3)
print(patch)

N = 10 --number of spans
K = 4 --length of span
L = 9 --length of the patch

wta.init(N, K, L)
h1 = wta.compute_hash(patch1)
h2 = wta.compute_hash(patch2)
score = wta.compare_hashes(h1, h2)
print(score)
--wta.c.print_lookup_table(wta.lookup_table)
