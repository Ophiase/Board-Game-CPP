uniform vec4 worldSpace;

void main() {
    vec2 uv = gl_FragCoord.xy;
    vec2 normalized = (uv - worldSpace.xy) / worldSpace.zw;

    vec2 color = mod(normalized, 0.5);
    gl_FragColor = vec4(color.x, color.y, 0.0, 0.5);
}