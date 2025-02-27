#version 330

uniform vec4 hoverOverlayColor;
uniform vec2 center;
uniform float radius;
uniform float secondMultiplier;
uniform float secondsHeldDown;

in vec4 passedColor;
layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

void main() {
	float opacityMultiplier = min(1, (1 - max(0, length(gl_FragCoord.xy - center) / radius)) * secondsHeldDown * secondMultiplier);
	gl_FragColor = mix(passedColor, mix(passedColor, hoverOverlayColor, hoverOverlayColor.a), opacityMultiplier);
}