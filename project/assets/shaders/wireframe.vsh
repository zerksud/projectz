uniform mat4 u_MVPMatrix;

attribute vec4 a_Position;

void main(void) {
    gl_Position =  u_MVPMatrix * a_Position;
}
