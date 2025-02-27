#version 330
in vec4 passedColor;

void main() {
	gl_FragColor = passedColor;
}