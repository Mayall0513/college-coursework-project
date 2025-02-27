uniform vec2 offset;
varying vec4 passedColor;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + vec4(offset.xy, 0, 0));
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	passedColor = gl_Color;
}