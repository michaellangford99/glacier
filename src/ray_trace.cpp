//#include "ray_trace.h"

// Soo.
// we need to allocate a buffer for the raw data to go ti
// create a function that generates a GL image from that.
// and then takes as input the inverse view projection matrices to transform from projection space rays to world space
//at least I think
//that is a good solution.
//then I can trace rays through some sort of object tree
//

/* things I need
 object tree
 some sort of registration system for properties that aren't just a 'view' of the object tree
 a way for the system to automatically register new properties of the object tree
 .... properties of shaders. so automatically extract all types and names of uniforms

 a way to load .obj files in a sane way
 */

