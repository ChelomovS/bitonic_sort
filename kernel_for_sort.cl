__kernel void bitonic_sort(__global int* data, int stage, int phase_of_stage) {
    int thread_id = get_global_id(0);

    bool direction = ((thread_id >> stage) & 1) == 0;

    int pair_distance = 1 << (stage - phase_of_stage);
    int partner_id = thread_id ^ pair_distance;

    if (partner_id > thread_id) { 
        int left_element = data[thread_id];
        int right_element = data[partner_id];

        if (direction) { 
            if (left_element > right_element) {
                data[thread_id] = right_element;
                data[partner_id] = left_element;
            }
        } else { 
            if (left_element < right_element) {
                data[thread_id]  = right_element;
                data[partner_id] = left_element;
            }
        }
    }
}