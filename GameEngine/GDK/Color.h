#pragma once

struct Color {
	float r, g, b, a;
};

namespace color {
	static Color red = { 1,0,0,1 };
	static Color green = { 0,1,0,1 };
	static Color blue = { 0,0,1,1 };
	static Color yellow = { 1,1,0,1 };
	static Color purple = { 1,0,1,1 };
	static Color cyan = { 0,1,1,1 };
	static Color white = { 1,1,1,1 };
	static Color black = { 0,0,0,1 };
};

