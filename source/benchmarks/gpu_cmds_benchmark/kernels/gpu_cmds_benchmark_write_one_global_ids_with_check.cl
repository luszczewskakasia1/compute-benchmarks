__kernel void write_one(__global int *outBuffer) {
    int value = outBuffer[get_global_id(0)];
    if(value == 1)
        outBuffer[get_global_id(0)] = 1;
}

