#version 430 core


layout (binding = 0, offset = 0) uniform atomic_uint fill_counter;

layout (binding = 1, r32ui) coherent uniform uimage2D head_pointer;

struct list_item 
{
    vec4 color;
    float depth;
    int facing;
    uint next;
};

layout (binding = 0; std430) buffer list_item_blocked
{
    list_item item[];
};

in VS_OUT
{
    vec4 pos;  
    vec4 color;
};

void main(void) 
{
    ivec2 P = ivec2(gl_FragCoord.xy);
    
    uint index = atomicCounterIncrement(fill_counter);
    
    uint old_head = imageAtomicExangle(head_pointer, P, index);
    
    item[index].color = fs_in.color;
    item[index].depth = gl_FragCoord.z;
    item[index].facing = gl_FrontFace ? 1 : 0;
    item[index].next = old_head;
}