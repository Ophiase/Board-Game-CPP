uniform vec2 origin;
uniform vec2 size;

void main() {
    vec2 uv = gl_FragCoord.xy;
    vec2 normalized = (uv - origin) / size;

    if (normalized.x < 0.0 || 1.0 < normalized.x ||
        normalized.y < 0.0 || 1.0 < normalized.y)
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}