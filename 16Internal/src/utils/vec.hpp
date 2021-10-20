#pragma once

struct vec3 {
	float x, y, z;

	vec3 operator+(vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	vec3 operator-(vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	vec3 operator*(float d) {
		return { x * d, y * d, z * d };
	}

	void normalize() {
		if (x < -89) x = -89;
		else if (x > 89) x = 89;

		if (y < -180) y = 180;
		else if (y > 180) y = -180;
	}
};

struct vec2 {
	float x, y;
};

struct vec4 {
	float x, y, z, w;
};