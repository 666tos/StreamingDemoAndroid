uniform sampler2D _uPlaneY;
uniform sampler2D _uPlaneU;
uniform sampler2D _uPlaneV;

uniform highp float  _uScreenAspectRatio;
uniform highp float  _uTextureAspectRatio;

varying highp vec3  _vPosition;
varying highp vec3  _vViewPos;
varying highp vec3  _vNormal;
varying highp vec2  _vTexCoord;

highp vec2 getTexCoord() {
    // texture is centered within a surface
    highp float base_y = 0.5;
    highp float offset = _vTexCoord.y - base_y;
    // texture occupies entire width, height is adjsuted to preserve aspect ratio
    highp float y = base_y + offset * (_uTextureAspectRatio / _uScreenAspectRatio);

    return vec2(_vTexCoord.x, y);
}

void main() {
    highp vec2 texCoord = getTexCoord();
    if (texCoord.y < 0.0 || texCoord.y > 1.0) {
        gl_FragColor = vec4(0, 0, 0, 0);

        return;
    }

    highp float y = texture2D(_uPlaneY, texCoord).a;
    highp float u = texture2D(_uPlaneU, texCoord).a;
    highp float v = texture2D(_uPlaneV, texCoord).a;
    highp vec3 yuv = vec3(y, u, v);

    highp vec3 colorOffset = vec3(0, -0.5, -0.5);

    highp mat3 colorMatrix = mat3(1,     1,          1,
                                  0,     -0.344,     1.770,
                                  1.403, -0.714,     0);

    highp vec3 rgb = colorMatrix * (yuv + colorOffset);

    gl_FragColor = vec4(rgb, 1);
}