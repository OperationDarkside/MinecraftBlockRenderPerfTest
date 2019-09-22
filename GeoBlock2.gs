#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

in vec4 fbr[];
in vec4 ftl[];
in vec4 ftr[];

in vec4 rbl[];
in vec4 rbr[];
in vec4 rtl[];
in vec4 rtr[];

out vec3 col;

void main() {
	
	
	// front 1
	col = vec3(1.0f, 0.0f, 0.0f); 
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = ftr[0];
    EmitVertex();
    gl_Position = ftl[0];
    EmitVertex();
    EndPrimitive();

    // front 2
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = fbr[0];
    EmitVertex();
    gl_Position = ftr[0];
    EmitVertex();
    EndPrimitive();
    
    // right 1
    col = vec3(0.0f, 1.0f, 0.0f);
    gl_Position = fbr[0]; 
    EmitVertex();
    gl_Position = rtr[0]; 
    EmitVertex();
    gl_Position = ftr[0];
    EmitVertex();
    EndPrimitive();
    
    // right 2
    gl_Position = fbr[0]; 
    EmitVertex();
    gl_Position = rbr[0]; 
    EmitVertex();
    gl_Position = rtr[0];
    EmitVertex();
    EndPrimitive();
    
    // back 1
    col = vec3(0.0f, 0.0f, 1.0f);
    gl_Position = rbr[0]; 
    EmitVertex();
    gl_Position = rtl[0]; 
    EmitVertex();
    gl_Position = rtr[0];
    EmitVertex();
    EndPrimitive();
    
    // back 2
    gl_Position = rbr[0]; 
    EmitVertex();
    gl_Position = rbl[0]; 
    EmitVertex();
    gl_Position = rtl[0];
    EmitVertex();
    EndPrimitive();
    
    // left 1
    col = vec3(0.0f, 1.0f, 1.0f);
    gl_Position = rbl[0]; 
    EmitVertex();
    gl_Position = ftl[0]; 
    EmitVertex();
    gl_Position = rtl[0];
    EmitVertex();
    EndPrimitive();
    
    // left 2
    gl_Position = rbl[0]; 
    EmitVertex();
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = ftl[0];
    EmitVertex();
    EndPrimitive();
    
    // top 1
    col = vec3(1.0f, 1.0f, 0.0f);
    gl_Position = ftl[0]; 
    EmitVertex();
    gl_Position = rtr[0]; 
    EmitVertex();
    gl_Position = rtl[0];
    EmitVertex();
    EndPrimitive();
    
    // top 2
    gl_Position = ftl[0]; 
    EmitVertex();
    gl_Position = ftr[0]; 
    EmitVertex();
    gl_Position = rtr[0];
    EmitVertex();
    EndPrimitive();

    // bottom 1
    col = vec3(1.0f, 1.0f, 1.0f);
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = rbr[0]; 
    EmitVertex();
    gl_Position = rbl[0];
    EmitVertex();
    EndPrimitive();
    
    // bottom 2
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = fbr[0]; 
    EmitVertex();
    gl_Position = rbr[0];
    EmitVertex();
    EndPrimitive();
} 