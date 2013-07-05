package = "wta"
version = "1.0-0"

source = {
   url = "https://github.com/aikuharenko/WTA.git"
}

description = {
   summary = "wta hash function for torch",
   detailed = [[
   ]],
   homepage = "https://github.com/aikuharenko/wta",
   license = "BSD"
}

dependencies = {
   "torch >= 7.0",
}

build = {
   type = "cmake",
   variables = {
      LUAROCKS_PREFIX = "$(PREFIX)"
   }
}
