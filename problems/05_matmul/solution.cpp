#include "solution.h"

// TODO: implement the declarations from solution.h.
// C = A x B (row-major). A is M x K, B is K x N, C is M x N.
// TODO: implement. Validate arguments and guard dimension arithmetic
// against overflow.
bool matmul(const float* a, const float* b, float* c, std::size_t m, std::size_t k, std::size_t n){

    if(a == nullptr || b == nullptr || c == nullptr)
    {
        return false;
    }

    if(k!= 0 && m  > std::numeric_limits<std::size_t>::max() / k)
    {
        return false;
    }

    if(k!= 0 && n > std::numeric_limits<std::size_t>::max() / k)
    {
        return false;
    }

    if(m != 0 && n > std::numeric_limits<std::size_t>::max() / m)
    {
        return false;
    }

    for(std::size_t i = 0; i < m; i++){
        for(std::size_t j = 0; j < n; j++){
            c[i * n + j] = 0.0f;
            float sum = 0;
            for(std::size_t l = 0; l < k; l++){
                sum += a[i * k + l] * b[l * n + j];
            }
            c[i * n + j] = sum;
        }
    }

    return true;
}

