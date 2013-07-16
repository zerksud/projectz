uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform mediump vec3 u_LightPos;

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoordinate;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoordinate;
varying float v_LightDistance;

void main(void) {
    v_Position = vec3(u_MVMatrix * a_Position);
    v_TexCoordinate = a_TexCoordinate;
    v_Normal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));
    v_LightDistance = length(u_LightPos - v_Position);

    gl_Position =  u_MVPMatrix * a_Position;
}
