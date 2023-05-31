#version 300 es

precision mediump float;

layout (location = 0) out vec4 FragColor;



uniform vec4 color;
uniform float top_left_radius;
uniform float top_right_radius;
uniform float bottom_left_radius;
uniform float bottom_right_radius;
uniform vec3 pos;
uniform vec2 dimension;

void main ()
{
	if ((gl_FragCoord.x >= pos.x) && (gl_FragCoord.x <= (pos.x + dimension.x))) {
		if ((gl_FragCoord.y >= pos.y) && (gl_FragCoord.y <= (pos.y + dimension.y))) {

			// left down corner
			float rr = bottom_left_radius;
			float rw = pos.x + rr;
			float rh = pos.y + rr;

			float x = gl_FragCoord.x;
			float y = gl_FragCoord.y;

			if ((x >= pos.x) && (x <= (rw))) {
				if ((y >= pos.y) &&(y <= (rh))) {
					float rradius = bottom_left_radius;

					float r = 
						(rw - gl_FragCoord.x) * 
						(rw - gl_FragCoord.x) + 
						(rh - gl_FragCoord.y) * 
						(rh - gl_FragCoord.y);

					if (r <= (rradius * rradius)) {
						FragColor = color;
					} else {
						discard;
					}
				}
			}

			// right up corner
			rr = top_right_radius;
			rw = pos.x + dimension.x - rr;
			rh = pos.y + dimension.y - rr;


			if ((x >= rw) && (x <= (rw + rr))) {
				if ((y >= rh) &&(y <= (rh + rr))) {
					float rradius = top_right_radius;

					float r = 
						(rw - gl_FragCoord.x) * 
						(rw - gl_FragCoord.x) + 
						(rh - gl_FragCoord.y) * 
						(rh - gl_FragCoord.y);

					if (r <= (rradius * rradius)) {
						FragColor = color;
					} else {
						discard;
					}
				}
			}

			// right down corner
			rr = bottom_right_radius;
			rw = pos.x + dimension.x - rr;
			rh = pos.y;


			if ((x >= rw) && (x <= (rw + rr))) {
				if ((y >= rh) &&(y <= (rh + rr))) {
					float rradius = bottom_right_radius;
					float r = 
						(rw - gl_FragCoord.x) * 
						(rw - gl_FragCoord.x) + 
						(rh + rr - gl_FragCoord.y) * 
						(rh + rr - gl_FragCoord.y);

					if (r <= (rradius * rradius)) {
						FragColor = color;
					} else {
						discard;
					}
				}
			}

			// left up corner
			rr = top_left_radius;
			rw = pos.x;
			rh = pos.y + dimension.y - rr;

			if ((x >= rw) && (x <= (rw + rr))) {
				if ((y >= rh) &&(y <= (rh + rr))) {
					float rradius = top_left_radius;
					float r = 
						(rw + rr - gl_FragCoord.x) *
						(rw + rr  - gl_FragCoord.x) +
						(rh - gl_FragCoord.y) *
						(rh - gl_FragCoord.y);

					if (r <= (rradius * rradius)) {
						FragColor = color;
					} else {
						discard;
					}
				}
			}
			FragColor = color;
		}
	}
}
