#include <climits>
#include <type_traits>

template <typename UnsignedType>
UnsignedType next_pow_2(UnsignedType v) {
  static_assert(std::is_unsigned<UnsignedType>::value,
                "Only works for unsigned types");
  if (v == 0) return 1;
  v--;
  for (size_t i = 1; i < sizeof(v) * CHAR_BIT;
       i *= 2)  // Prefer size_t "Warning comparison between signed and unsigned
                // integer"
  {
    v |= v >> i;
  }
  return ++v;
}

template <typename UnsignedType>
bool IsPowerOfTwo(UnsignedType x) {
  return (x != 0) && ((x & (x - 1)) == 0);
}

