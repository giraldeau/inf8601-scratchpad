# registers RDI, RSI, RDX, RCX, R8, and R9,

.global array_addss_iter, array_addss_vect, movss_ex1, movups_ex1

.text

# void array_add_scalar_iter(float *vector, float scalar, long length);
# rdi = vector
# xmm0 = scalar
# rsi  = length
array_addss_iter:
    ret

# void array_add_scalar_iter(float *vector, float scalar, long length);
# rdi = vector
# xmm0 = scalar
# rsi  = length
array_addss_vect:
    ret

movss_ex1:
    ret

movups_ex1:
    ret
