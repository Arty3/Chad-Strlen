# :fire: The Chad Strlen :fire:

###### _Some guys were competing to make the fastest strlen, so I took it personally_

## Features
- Supports virtually every architecture
- Supports both big endian and little endian
- Supports implementation aliasing
- Extensive compiler optimizations
- Unreadable (skill issue)
- Very, very fast

## Limitations
- Requires GNUC features (also found within clang)

## Usage
I strongly recommend you use
[GCC](https://gcc.gnu.org/) or
[CLANG](https://clang.llvm.org/)
to compile it,
<br>
Then, simply include this implementation in your project.
<br><br>
Using `GCC` or `CLANG`, the function will automatically alias to `strlen`,
<br>
so you can just use it like you would with a regular `strlen`:
```C
// Change this to whichever header it's defined in
#include "strlen_implementation.h"

void foo(void)
{
	size_t length = strlen("Hello, world!");
}

```

Very basic header example:

```h
#ifndef  __STRLEN_IMPL
# define __STRLEN_IMPL

#ifdef __cplusplus
extern "C" {
#endif

// Use alias instead of implementation
size_t	strlen(const char *__restrict__ s);

#ifdef __cplusplus
}
#endif

#endif // __STRLEN_IMPL
```

<br>
You can also just let the compiler overwrite the strlen function:

```sh
$(which cc) -Wl,--wrap=strlen __strlen_impl.c $src -o MyProgram
```

###### _Replace `$src` with whichever source files you want to compile_
<br>
Then just use it as you normally would:

```c
#include <string.h>

void foo(void)
{
	size_t length = strlen("Hello, world!");
}
```

## Benchmarks

- Compiled sources using `GCC`
- No compiler optimizations were used (`-O0`).
- Benchmarked over **100000000** iterations.
- Used a 4 page buffer per iteration (**16384** bytes).
- Used my own `AMD Ryzen 9 7950x CPU`

| Strlen    | Time Taken |
| ------    | ---------- |
| Glibc     | 584.043ms  |
| _**Mine**_| 594.236ms  |
| Musl      | 2792.659ms |
| Basic     | 11216.379ms|

###### _Each benchmark counted almost 2 trillion bytes (yes, really!)_

This means that it took _**362.69 femtoseconds**_ to count 1 byte:
```math
\frac{362.69}{10^{15}} seconds \rightarrow \frac{362.69}{1,000,000,000,000,000} seconds
```
\
Which means, that in this time, light travelled only _**108.73 micrometers**_:

```math
0.00010873m \rightarrow \frac{108.73}{1,000,000} m
```

## How it works

The function uses a technique which is very widely used
in solved functions like strlen, memcpy and so on.
<br>
It processes the buffer in "words" (aka CPU words).
These are typically the size of a pointer.
<br>
<br>
This allows the function to process multiple bytes at a time
via a byte mask which checks for the null-terminator,
<br>
depending on the system's byte order, which adjusts the mask accordingly.
<br>
<br>
The bytemasking process is incredibly fast since its just bitwise
operations, if the null-terminator is found, the function
<br>
calculates the exact offset to it, and returns a diffence
between the starting pointer and the null-terminator pointer,
(Pointer arithmetics are always faster).
<br>
<br>
For very small buffers, the function doesn't even enter
a loop, making small buffers also blazingly fast.
<br>
Of course each compilation target is compiled based on
architecture, byte order and so on.
<br>
So a lot less runtime efficiency is wasted there.

## License
This project uses the `GNU GENERAL PUBLIC LICENSE v3.0` license
<br>
For more info, please find the `LICENSE` file here: [License](LICENSE)