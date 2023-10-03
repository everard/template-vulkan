# DESCRIPTION
This repository contains a template for building Vulkan applications using SDL2.

# COMPILATION
Warning! Uses the latest C++ Standard. Currently (October 2023) only compiles
under Clang 16 and later.

To build all modules and executables (from all .ccm files), run:

```
make
```

To build an executable file from a .cc file, run:

```
make build/program.name_of_cc_file
```

For example, to build main.cc into executable, run:

```
make build/program.main
```

# LICENSE
Copyright Nezametdinov E. Ildus 2023.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt)
