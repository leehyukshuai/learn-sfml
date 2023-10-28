uniform sampler2D texture;
uniform float offset;

void main() {
    float r = texture2D(texture, vec2(gl_TexCoord[0].x + offset, gl_TexCoord[0].y + offset)).r;
    float g = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y)).g;
    float b = texture2D(texture, vec2(gl_TexCoord[0].x - offset, gl_TexCoord[0].y - offset)).b;
    gl_FragColor = vec4(r, g, b, 1.0);
}