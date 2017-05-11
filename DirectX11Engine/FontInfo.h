#pragma once

//header file containing info about a font texture

struct Character {
	int codePoint, x, y, width, height, originX, originY;
};

struct BufferInfo {
	int width, height, offsetX, offsetY;
};

const static int ARIAL_HEIGHT = 125;
const static int ARIAL_WIDTH  = 350;


const static int INDEX_OFFSET = 32;

static Character characters_Arial[] = {
	{ ' ', 272, 106, 3, 3, 1, 1 },
	{ '!', 137, 81, 7, 25, -1, 24 },
	{ '"', 178, 106, 12, 10, 0, 24 },
	{ '#', 312, 31, 20, 25, 1, 24 },
	{ '$', 117, 0, 19, 29, 1, 25 },
	{ '%', 194, 0, 28, 25, 0, 24 },
	{ '&', 185, 31, 22, 25, 0, 24 },
	{ '\'', 197, 106, 6, 10, 0, 24 },
	{ '(', 59, 0, 11, 31, 0, 24 },
	{ ')', 70, 0, 11, 31, 0, 24 },
	{ '*', 164, 106, 14, 12, 1, 24 },
	{ '+', 109, 106, 19, 17, 0, 20 },
	{ ',', 190, 106, 7, 10, -1, 4 },
	{ '-', 231, 106, 12, 5, 1, 11 },
	{ '.', 243, 106, 7, 5, -1, 4 },
	{ '/', 68, 81, 13, 25, 2, 24 },
	{ '0', 278, 56, 18, 25, 0, 24 },
	{ '1', 107, 81, 12, 25, -2, 24 },
	{ '2', 40, 56, 19, 25, 1, 24 },
	{ '3', 206, 56, 18, 25, 0, 24 },
	{ '4', 97, 56, 19, 25, 1, 24 },
	{ '5', 116, 56, 18, 25, 0, 24 },
	{ '6', 59, 56, 19, 25, 1, 24 },
	{ '7', 188, 56, 18, 25, 0, 24 },
	{ '8', 224, 56, 18, 25, 0, 24 },
	{ '9', 242, 56, 18, 25, 0, 24 },
	{ ':', 64, 106, 7, 19, -1, 18 },
	{ ';', 158, 81, 7, 24, -1, 18 },
	{ '<', 90, 106, 19, 18, 0, 20 },
	{ '=', 145, 106, 19, 12, 0, 17 },
	{ '>', 71, 106, 19, 18, 0, 20 },
	{ '?', 170, 56, 18, 25, 0, 24 },
	{ '@', 0, 0, 33, 31, 0, 24 },
	{ 'A', 0, 31, 25, 25, 2, 24 },
	{ 'B', 20, 56, 20, 25, -1, 24 },
	{ 'C', 50, 31, 24, 25, 0, 24 },
	{ 'D', 141, 31, 22, 25, -1, 24 },
	{ 'E', 0, 56, 20, 25, -1, 24 },
	{ 'F', 78, 56, 19, 25, -1, 24 },
	{ 'G', 322, 0, 25, 25, 0, 24 },
	{ 'H', 249, 31, 21, 25, -1, 24 },
	{ 'I', 151, 81, 7, 25, -1, 24 },
	{ 'J', 52, 81, 16, 25, 1, 24 },
	{ 'K', 97, 31, 22, 25, -1, 24 },
	{ 'L', 35, 81, 17, 25, -1, 24 },
	{ 'M', 222, 0, 25, 25, -1, 24 },
	{ 'N', 228, 31, 21, 25, -1, 24 },
	{ 'O', 297, 0, 25, 25, 0, 24 },
	{ 'P', 207, 31, 21, 25, -1, 24 },
	{ 'Q', 136, 0, 25, 26, 0, 24 },
	{ 'R', 74, 31, 23, 25, -1, 24 },
	{ 'S', 270, 31, 21, 25, 0, 24 },
	{ 'T', 119, 31, 22, 25, 1, 24 },
	{ 'U', 291, 31, 21, 25, -1, 24 },
	{ 'V', 272, 0, 25, 25, 2, 24 },
	{ 'W', 161, 0, 33, 25, 1, 24 },
	{ 'X', 247, 0, 25, 25, 2, 24 },
	{ 'Y', 25, 31, 25, 25, 2, 24 },
	{ 'Z', 163, 31, 22, 25, 1, 24 },
	{ '[', 101, 0, 10, 31, 0, 24 },
	{ '\\', 94, 81, 13, 25, 2, 24 },
	{ ']', 81, 0, 10, 31, 1, 24 },
	{ '^', 128, 106, 17, 15, 1, 24 },
	{ '_', 250, 106, 22, 4, 2, -3 },
	{ '`', 222, 106, 9, 6, 0, 24 },
	{ 'a', 237, 81, 19, 19, 1, 18 },
	{ 'b', 152, 56, 18, 25, 0, 24 },
	{ 'c', 17, 106, 17, 19, 0, 18 },
	{ 'd', 134, 56, 18, 25, 1, 24 },
	{ 'e', 218, 81, 19, 19, 1, 18 },
	{ 'f', 81, 81, 13, 25, 1, 24 },
	{ 'g', 0, 81, 18, 25, 1, 18 },
	{ 'h', 18, 81, 17, 25, 0, 24 },
	{ 'i', 144, 81, 7, 25, 0, 24 },
	{ 'j', 91, 0, 10, 31, 3, 24 },
	{ 'k', 314, 56, 18, 25, 0, 24 },
	{ 'l', 130, 81, 7, 25, 0, 24 },
	{ 'm', 192, 81, 26, 19, 0, 18 },
	{ 'n', 0, 106, 17, 19, 0, 18 },
	{ 'o', 256, 81, 19, 19, 1, 18 },
	{ 'p', 296, 56, 18, 25, 0, 18 },
	{ 'q', 260, 56, 18, 25, 1, 18 },
	{ 'r', 51, 106, 13, 19, 0, 18 },
	{ 's', 275, 81, 18, 19, 1, 18 },
	{ 't', 119, 81, 11, 25, 1, 24 },
	{ 'u', 34, 106, 17, 19, 0, 18 },
	{ 'v', 329, 81, 18, 19, 1, 18 },
	{ 'w', 165, 81, 27, 19, 2, 18 },
	{ 'x', 293, 81, 18, 19, 1, 18 },
	{ 'y', 332, 56, 18, 25, 1, 18 },
	{ 'z', 311, 81, 18, 19, 1, 18 },
	{ '{', 46, 0, 13, 31, 1, 24 },
	{ '|', 111, 0, 6, 31, -1, 24 },
	{ '}', 33, 0, 13, 31, 1, 24 },
	{ '~', 203, 106, 19, 7, 0, 15 },
};
