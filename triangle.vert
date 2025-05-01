#version 400 core

layout (location = 0) in vec4 vPosition;

// subroutine signature
subroutine vec4 colorType ();
 
// option 1
subroutine (colorType ) vec4 redColor() {
 
    return vec4(1.0, 0.0, 0.0, 1.0);
} 
 
// option 2
subroutine (colorType ) vec4 blueColor() {
 
    return vec4(0.0, 0.0, 1.0, 1.0);
}

// subroutine uniform variable
subroutine uniform colorType colorSelection;

out vec4 vColor;

void main()
{
    vColor = colorSelection();
    gl_Position = vPosition;
}
