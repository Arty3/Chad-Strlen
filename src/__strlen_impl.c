#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <endian.h>

#if defined(__GNUC__) || defined (__clang__)
typedef unsigned long int __attribute__ ((__may_alias__)) word_t;
typedef unsigned long int __attribute__ ((__may_alias__)) bytemask_t;
#if !defined(__weak_alias)
#define __weak_alias(old, new)	\
	extern __typeof__(old) new	\
	__attribute__((__weak__, __alias__(#old)))
#endif
#else
typedef unsigned long int word_t;
typedef unsigned long int bytemask_t;
#endif

size_t	__strlen_impl(const char *__restrict__ s)
{
#if defined(__GNUC__) || defined (__clang__)
	register const uintptr_t	s0 = (uintptr_t)s;
	register const word_t		*w = (const word_t *)(((uintptr_t)s) & \
									-((uintptr_t)(sizeof(word_t))));

	register word_t	wi = *w;

	register word_t	m = ((word_t)-1 / 0xff) * 0x7f;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	bytemask_t	register mask = ~(((wi & m) + m) | wi | m) >> (CHAR_BIT * (s0 % sizeof (word_t)));
#else
	bytemask_t	register mask = ~(((wi & m) + m) | wi | m) << (CHAR_BIT * (s0 % sizeof (word_t)));
#endif

	if (mask)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
# if __SIZEOF_POINTER__ == 8
		return (__builtin_ctzl(mask) / CHAR_BIT);
# else
		return (__builtin_ctzll(mask) / CHAR_BIT);
# endif
#else
# if __SIZEOF_POINTER__ == 8
		return (__builtin_clzl(mask) / CHAR_BIT);
# else
		return (__builtin_clzll(mask) / CHAR_BIT);
# endif
#endif
	}

	do
	{
		wi = *++w;
	} while (((wi - (((word_t)-1 / 0xff) * 0x01)) & ~wi & (((word_t)-1 / 0xff) * 0x80)) == 0);

#if __BYTE_ORDER == __LITTLE_ENDIAN
	wi = (wi - ((word_t)-1 / 0xff) * 0x01) & ~wi & ((word_t)-1 / 0xff) * 0x80;
#else
	word_t rb = ((word_t)-1 / 0xff) * 0x7f;
	wi = ~(((wi & rb) + rb) | wi | rb);
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# if __SIZEOF_POINTER__ == 8
		wi = (__builtin_ctzl(wi) / CHAR_BIT);
# else
		wi = (__builtin_ctzll(wi) / CHAR_BIT);
# endif
#else
# if __SIZEOF_POINTER__ == 8
		wi = (__builtin_clzl(wi) / CHAR_BIT);
# else
		wi = (__builtin_clzll(wi) / CHAR_BIT);
# endif
#endif

	return ((const char *)w) + wi - s;
#endif
}

#if defined(__GNUC__) || defined (__clang__)
__weak_alias(__strlen_impl, strlen);
#endif
