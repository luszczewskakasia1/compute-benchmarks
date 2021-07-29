cat devices_base.inl embargo/devices.inl                                | # raw source code
    grep -E "(NAMED)?DEVICE"                                            | # filter only device definitions,        e.g. DEVICE(0x56A8, DG2_CONFIG, GTTYPE_GT4)
    sed -E "s/(NAMEDDEVICE|DEVICE)\(\s*([^,]+), ([^,]+)(,.*)?/\2 \3/g"  | # strip uninmportant parts of the macro, e.g. 0x56A8 DG2_CONFIG
    sed -E "s/_(HW_CONFIG|CONFIG|([0-9x]*))$//g"                        | # strip device type suffix,              e.g. 0x56A8 DG2
    sed -E 's/^([^ ]+) (.*)/\1 \L\u\2/g'                                | # convert device type to capitalized,    e.g. 0x56A8 Dg2
    sed -E "s/([^ ]+) (.*)/INTEL_PRODUCT_ID\(\2, \1\)/g"                  # wrap in ComputeBenchmarks macro,       e.g. INTEL_PRODUCT_ID(Dg2, 0x56A8)
