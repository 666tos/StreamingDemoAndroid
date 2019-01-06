attribute highp vec4 _inPosition;
attribute highp vec4 _inNormal;
attribute highp vec2 _inTexCoord;

varying highp vec3 _vPosition;
varying highp vec3 _vNormal;
varying highp vec2 _vTexCoord;

void main()
{
    highp float texY = _inTexCoord.y;

    _vTexCoord = vec2(_inTexCoord.x, 1.0 - texY);
    _vPosition = _inPosition.xyz;
    _vNormal = _inNormal.xyz;

    gl_Position = _inPosition;
}
