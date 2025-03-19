__kernel void bitonic_sort_global(__global int* data, int stage, int stage_pass, int direction) {
    int id = get_global_id(0); 
    int dist = 1 << (stage - stage_pass);

    int left_id = (id % dist) + (id / dist) * 2 * dist;
    int right_id = left_id + dist;

    int left_elem = data[left_id];
    int right_elem = data[right_id];

    if ((id / (1 << stage)) % 2 == 1)
        direction = 1 - direction;
        
    int greater = (left_elem > right_elem) ? left_elem : right_elem;
    int lesser = (left_elem > right_elem) ? right_elem : left_elem;

    data[left_id]  = direction ? lesser : greater;
    data[right_id] = direction ? greater : lesser;
}
