#include <iostream>

#include <xmmintrin.h> // SSE

using std::cout;
using std::endl;

#define PRINT_M128(x)   \
do {                    \
    cout << #x << ": "; \
    print_m128(x);      \
} while (0)

void print_m128(__m128 x) {
    for (int i = 0; i < 4; ++i) {
#ifdef _WIN32
        cout << x.m128_f32[i] << ' ';
#else
        cout << x[i] << ' ';
#endif
    }
    cout << endl;
}

int main()
{
    __m128 x4 = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
    __m128 y4 = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);

    PRINT_M128(x4);
    PRINT_M128(y4);

    __m128 add4 = _mm_add_ps(x4, y4);
    __m128 mul4 = _mm_mul_ps(x4, y4);

    PRINT_M128(add4);
    PRINT_M128(mul4);

    return 0;
}
