__kernel void write_one(__global int *outBuffer) {
    if(get_local_id(0)){
        int value = atomic_dec(outBuffer);
        if(value == 0 ) {
            outBuffer[1] = 1337;
        }
    }
}