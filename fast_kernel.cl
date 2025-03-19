__kernel void bitonic_sort_local(__global int* data, __local int* local_data, int stages_number, int direction) {
    int local_id = get_local_id(0);
    int work_group_size = get_local_size(0);
    int offset = get_group_id(0) * work_group_size;
    local_data[local_id] = data[offset << 1 + local_id];
    local_data[local_id + work_group_size] = data[offset << 1 + local_id + work_group_size];

    barrier(CLK_LOCAL_MEM_FENCE);

    for(int stage = 0; stage < stages_number; ++stage) {
        for(int stage_pass = 0; stage_pass < stage + 1; ++stage_pass) {
            int dir = direction;
            int dist = 1 << (stage - stage_pass);
            int left_id = (local_id % dist) + (local_id / dist) * 2 * dist;
            int right_id = left_id + dist;
            int left_elem = local_data[left_id];
            int right_elem = local_data[right_id];
            if((local_id / (1 << stage)) % 2 == 1)
                dir = 1 - dir;
            int greater = (left_elem > right_elem) ? left_elem : right_elem;
            int lesser  = (left_elem > right_elem) ? right_elem : left_elem;
            local_data[left_id]  = dir ? lesser : greater;
            local_data[right_id] = dir ? greater : lesser;

            barrier(CLK_LOCAL_MEM_FENCE);
        }
    }

    data[offset << 1 + local_id] = local_data[local_id];
    data[offset << 1 + local_id + work_group_size] = local_data[local_id + work_group_size];
}
