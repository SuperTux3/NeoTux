$input a_position, a_color0
$output v_color0, v_texcoord0

// hack
#include "../../build/_deps/bgfx-src/bgfx/src/bgfx_shader.sh"
//#include <bgfx_shader.sh>

void main()
{
	gl_Position = vec4(1.0);
	//v_color0 = a_color0;
}