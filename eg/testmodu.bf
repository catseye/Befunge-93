v The original implementation of Befunge-93 was in ANSI C (a.k.a C89).
v The description of Befunge-93 did not describe how modulo should be
v implemented for a negative modulus -- it relied on ANSI C's semantics.
v
v Unfortunately, ANSI C did not define negative modulus either.
v
v So this program tests what your Befunge-93 implementation does for
v modulo by negative numbers.  If it outputs:
v
v  1 -1 : result has same sign as the dividend (like C99)
v -2  2 : result has same sign as the divisor  (like Python)
v
v Of course, since it is undefined, other results are possible.
v
>703-%.07-3%.@
