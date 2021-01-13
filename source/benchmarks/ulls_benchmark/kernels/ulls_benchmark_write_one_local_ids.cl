__kernel void write_one(__global int *outBuffer) {
    outBuffer[get_local_id(0)] = 1;
}

