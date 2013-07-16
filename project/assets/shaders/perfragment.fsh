precision mediump float;

uniform vec3 u_LightPos;
uniform sampler2D u_Texture;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoordinate;
varying float v_LightDistance;

void main() {
    vec3 lightVector = normalize(u_LightPos - v_Position);

    float diffuse = dot(v_Normal, lightVector);
    diffuse = diffuse * (1.0 / (1.0 + (0.1 * v_LightDistance * v_LightDistance)));
    diffuse = diffuse + 0.2;

    gl_FragColor = diffuse * texture2D(u_Texture, v_TexCoordinate);
}
