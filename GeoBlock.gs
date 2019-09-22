#version 330 core
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 36) out;

in vec4 PosRight[];

out vec3 col;

void main() {
	
	
	// front 1
	col = vec3(1.0f, 0.0f, 0.0f); 
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = PosRight[1];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();

    // front 2
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = PosRight[0];
    EmitVertex();
    gl_Position = PosRight[1];
    EmitVertex();
    EndPrimitive();
    
    // right 1
    col = vec3(0.0f, 1.0f, 0.0f);
    gl_Position = PosRight[0]; 
    EmitVertex();
    gl_Position = PosRight[3]; 
    EmitVertex();
    gl_Position = PosRight[1];
    EmitVertex();
    EndPrimitive();
    
    // right 2
    gl_Position = PosRight[0]; 
    EmitVertex();
    gl_Position = PosRight[2]; 
    EmitVertex();
    gl_Position = PosRight[3];
    EmitVertex();
    EndPrimitive();
    
    // back 1
    col = vec3(0.0f, 0.0f, 1.0f);
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[3]; 
    EmitVertex();
    gl_Position = gl_in[3].gl_Position;
    EmitVertex();
    EndPrimitive();
    
    // back 2
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[2]; 
    EmitVertex();
    gl_Position = PosRight[3];
    EmitVertex();
    EndPrimitive();
    
    // left 1
    col = vec3(0.0f, 1.0f, 1.0f);
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = gl_in[3].gl_Position; 
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();
    
    // left 2
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();
    gl_Position = gl_in[3].gl_Position;
    EmitVertex();
    EndPrimitive();
    
    // top 1
    col = vec3(1.0f, 1.0f, 0.0f);
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[3]; 
    EmitVertex();
    gl_Position = gl_in[3].gl_Position;
    EmitVertex();
    EndPrimitive();
    
    // top 2
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[1]; 
    EmitVertex();
    gl_Position = PosRight[3];
    EmitVertex();
    EndPrimitive();

    // bottom 1
    col = vec3(1.0f, 1.0f, 1.0f);
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[2]; 
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
    
    // bottom 2
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position = PosRight[0]; 
    EmitVertex();
    gl_Position = PosRight[2];
    EmitVertex();
    EndPrimitive();
} 