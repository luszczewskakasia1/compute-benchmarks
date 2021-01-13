__kernel void increment(__global int *outBuffer) {
    size_t id = get_global_id(0);
    outBuffer[id]++;
}
