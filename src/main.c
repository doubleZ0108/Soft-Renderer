//=====================================================================
// 
//
// history:
//   2007.7.01  skywind  create this file as a tutorial
//   ...
//   2018.7.7   JYK & LRJ & CHJ modification on light, culling and texture
// 
//=====================================================================
#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>

#include "math.h"
#include "geometry.h"
#include "coordinate.h"
#include "device.h"
#include "render.h"
#include "window.h"
#include "bitmap.h"

#define MAXRGBBYTE 260*260

//=====================================================================
// 主程序
//=====================================================================
vertex_t mesh[8] = {
	{ { 1, -1,  1, 1 },{ 0, 0 },{ 1.0f, 0.2f, 0.2f }, 1 },
{ { -1, -1,  1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 0.2f }, 1 },
{ { -1,  1,  1, 1 },{ 1, 1 },{ 0.2f, 0.2f, 1.0f }, 1 },
{ { 1,  1,  1, 1 },{ 1, 0 },{ 1.0f, 0.2f, 1.0f }, 1 },
{ { 1, -1, -1, 1 },{ 0, 0 },{ 1.0f, 1.0f, 0.2f }, 1 },
{ { -1, -1, -1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 1.0f }, 1 },
{ { -1,  1, -1, 1 },{ 1, 1 },{ 1.0f, 0.3f, 0.3f }, 1 },
{ { 1,  1, -1, 1 },{ 1, 0 },{ 0.2f, 1.0f, 0.3f }, 1 },
};

void draw_plane(device_t *device, int a, int b, int c, int d) {
	vertex_t p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	device_draw_primitive(device, &p1, &p2, &p3, 0);
	device_draw_primitive(device, &p3, &p4, &p1, 0);
}

void draw_box(device_t *device, float theta) {
	matrix_t m;
	matrix_set_rotate(&m, -1, -0.5, 1, theta); // 绕着轴 (-1，-0.5，1) 旋转 theta
	device->transform.world = m;
	transform_update(&device->transform);
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 4, 5, 6, 7);
	draw_plane(device, 0, 4, 5, 1);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 3, 7, 4, 0);
}

void draw_box_2(device_t *device, float theta) {
	matrix_t m;
	matrix_set_translate(&m, -1, 0, 0);
	device->transform.world = m;
	transform_update(&device->transform);
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 4, 5, 6, 7);
	draw_plane(device, 0, 4, 5, 1);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 3, 7, 4, 0);
}

void camera_at_zero(device_t *device, float x, float y, float z) {
	point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	matrix_set_lookat(&device->transform.view, &eye, &at, &up);
	transform_update(&device->transform);
}

void init_texture(device_t *device) {
	static IUINT32 texture[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 64, y = j / 64;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}
	device_set_texture(device, texture, 256 * 4, 256, 256);
}

void init_texture_by_file(device_t *device, FILE *fp) {
	static IUINT32 texture[256][256];
	long width = getWidth(fp);
	long height = getHeight(fp);
	unsigned char *r, *g, *b;
	r = (unsigned char *)malloc(MAXRGBBYTE);
	b = (unsigned char *)malloc(MAXRGBBYTE);
	g = (unsigned char *)malloc(MAXRGBBYTE);
	getData(fp, r, g, b);

	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			texture[i][j] = ((int)*(r + i * width + j) << 16) | ((int)*(g + i * width + j) << 8) | ((int)*(b + i * width + j));
		}
	}
	device_set_texture(device, texture, 256 * 4, width, height);
}

int main(void) {
	device_t device;
	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int indicator = 1;
	int kbhit = 0;
	int animated = 0;
	float alpha = 0;
	float pos = 3.5;

	TCHAR *title = _T("SoftRasterization");

	if (screen_init(800, 600, title))
		return -1;

	device_init(&device, 800, 600, screen_fb);
	camera_at_zero(&device, 3, 0, 0);

	long width, height;
	FILE *fp = fopen("texture.bmp", "r");

	if (fp == NULL) /* `fp`, not `ch` */
	{
		printf("The file didn't opened\n");
		return -1;
	}
	init_texture_by_file(&device, fp);
	//init_texture (&device);
	device.render_state = RENDER_STATE_COLOR;

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		screen_dispatch();
		device_clear(&device, 1);
		camera_at_zero(&device, pos, 0, 0);

		if (screen_keys[VK_UP]) pos -= 0.02f;
		if (screen_keys[VK_DOWN]) pos += 0.02f;
		if (screen_keys[VK_LEFT]) alpha += 0.02f;
		if (screen_keys[VK_RIGHT]) alpha -= 0.02f;

		if (screen_keys[VK_SPACE]) {
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				device.render_state = states[indicator];
			}
		}
		else {
			kbhit = 0;
		}

		if (screen_keys[VK_RETURN]) {
			animated = animated == 0 ? 1 : 0;
		}

		if (animated) {
			alpha += 0.03f;
		}

		draw_box(&device, alpha);
		//	draw_box_2 (&device, alpha);
		screen_update();
		Sleep(1);
	}

	system("pause");
	return 1;
}