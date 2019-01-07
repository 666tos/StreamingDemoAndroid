uniform sampler2D _uPlaneY;
uniform sampler2D _uPlaneU;
uniform sampler2D _uPlaneV;

varying highp vec3  _vPosition;
varying highp vec3  _vViewPos;
varying highp vec3  _vNormal;
varying highp vec2  _vTexCoord;

void main()
{
    highp float y = texture2D(_uPlaneY, _vTexCoord).a;
    highp float u = texture2D(_uPlaneU, _vTexCoord).a;
    highp float v = texture2D(_uPlaneV, _vTexCoord).a;
    highp vec3 yuv = vec3(y, u, v);

    highp vec3 colorOffset = vec3(0, -0.5, -0.5);

    highp mat3 colorMatrix = mat3(1,     1,          1,
                                  0,     -0.344,     1.770,
                                  1.403, -0.714,    0);

    highp vec3 rgb = colorMatrix * (yuv + colorOffset);

    gl_FragColor = vec4(rgb, 1);
}

