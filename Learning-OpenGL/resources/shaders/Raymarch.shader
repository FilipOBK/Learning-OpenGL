#version 330 core

uniform vec2 u_Resolution;
uniform float u_Time;

struct Ray {
	vec3 origin;
	vec3 direction;
};

float sdSphere(vec3 p, float s) {
	return length(p) - s;
}
float sdBox(vec3 p, vec3 b) {
	vec3 q = abs(p) - b;
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
}

float smin(float a, float b, float k) {
	float h = max(k - abs(a - b), 0.0) / k;
	return min(a, b) - pow(h, 3) * k * (1./6.);
}

float map(vec3 p) {
	float ground = p.y + .75;

	vec3 sphere_position = vec3(sin(u_Time) * 3., 0, 0);
	float sphere = sdSphere(p - sphere_position, 1.);

	float box = sdBox(p, vec3(.75));

	return min(ground, smin(box, sphere, 2.));
}

void main()
{	
	vec2 uv = (gl_FragCoord.xy * 2. - u_Resolution) / u_Resolution.y;

	// Initialization
	Ray	ray = Ray(vec3(0, 0, -3), normalize(vec3(uv, 1)));
	float t = 0.;
	vec3 color = vec3(0);

	// Raymarching
	for (int i = 0; i < 80; i++) {
		vec3 p = ray.origin + ray.direction * t;	// position along ray
		float d = map(p);							// current distance to scene
		t += d;										// march ray
		
		if (d < .001 || t > 100.) break;			// early stop
	}

	color = vec3(t * .2);
	gl_FragColor = vec4(color, 1);
}
