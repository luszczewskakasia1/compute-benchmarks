#ifndef ATOMIC_OP
#error "Need a definition of ATOMIC_OP"
#endif

#ifndef DATATYPE
#error "Need a definition of DATATYPE"
#endif

#ifndef ATOMIC_DATATYPE
#error "Need a definition of ATOMIC_DATATYPE"
#endif

#ifndef OTHER_ARGUMENT_BUFFER_SIZE
#error "Need a definition of OTHER_ARGUMENT_BUFFER_SIZE"
#endif

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 64
#endif

#define ATOMIC_OP_128(address, otherArgument) \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);        \
    ATOMIC_OP(address, otherArgument);

__kernel void initialize(__global ATOMIC_DATATYPE *buffer, const DATATYPE value) {
    atomic_init(buffer + get_global_id(0), value);
}

__kernel void one_atomic(__global ATOMIC_DATATYPE *buffer, const __global DATATYPE *otherArgumentBuffer, const uint iterations) {
    const DATATYPE otherArgument = otherArgumentBuffer[get_global_id(0) % OTHER_ARGUMENT_BUFFER_SIZE];
    for (uint i = 0; i < iterations; i++) {
        ATOMIC_OP_128(buffer, otherArgument)
    }
}
