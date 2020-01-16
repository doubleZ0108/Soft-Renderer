#pragma once

#include "device.h"

//=====================================================================
// 渲染实现
//=====================================================================

// 绘制扫描线
void device_draw_scanline (device_t *device, scanline_t *scanline, float diffuse) {
	IUINT32 *framebuffer = device->framebuffer[scanline->y];
	float *zbuffer = device->zbuffer[scanline->y];
	int x = scanline->x;
	int w = scanline->w;
	int width = device->width;
	int render_state = device->render_state;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width) {
			float rhw = scanline->v.rhw;
			if (rhw >= zbuffer[x]) {
				float w = 1.0f / rhw;
				zbuffer[x] = rhw;
				if (render_state & RENDER_STATE_COLOR) {
					float r = scanline->v.color.r * w * (0.3 + diffuse);
					float g = scanline->v.color.g * w * (0.3 + diffuse);
					float b = scanline->v.color.b * w * (0.3 + diffuse);
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = CMID (R, 0, 255);
					G = CMID (G, 0, 255);
					B = CMID (B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
				if (render_state & RENDER_STATE_TEXTURE) {
					float u = scanline->v.tc.u * w;
					float v = scanline->v.tc.v * w;
					IUINT32 cc = device_texture_read (device, u, v, (0.3 + diffuse));
					framebuffer[x] = cc;
				}
			}
		}
		vertex_add (&scanline->v, &scanline->step);
		if (x >= width) break;
	}
}

// 主渲染函数
void device_render_trap (device_t *device, trapezoid_t *trap, float diffuse) {
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < device->height) {
			trapezoid_edge_interp (trap, (float)j + 0.5f);
			trapezoid_init_scan_line (trap, &scanline, j);
			device_draw_scanline (device, &scanline, diffuse);
		}
		// 大于 height 就不用再画了，后边一定都在屏幕外，直接 break 就好
		// 但是小于 0 还要继续向下判断，不能 break
		if (j >= device->height) break;
	}
}



float calculate_diffuse(point_t *p1, point_t *p2, point_t *p3) {
	vector_t a, b, f;
	vector_sub(&a, p1, p2);
	vector_sub(&b, p2, p3);
	vector_crossproduct(&f, &a, &b);
	vector_normalize(&f);
	vector_t light_dir = { 1, 2, 3, 1 };
	vector_normalize(&light_dir);

	float diffuse = vector_dotproduct(&f, &light_dir);
	return diffuse<0 ? -diffuse : 0;
}

// 根据 render_state 绘制原始三角形
void device_draw_primitive (device_t *device, const vertex_t *v1,
	const vertex_t *v2, const vertex_t *v3, int depth) {
	if (depth > 5) { return; }
	point_t p1, p2, p3, c1, c2, c3;
	int render_state = device->render_state;

	float diffuse = calculate_diffuse(&v1->pos, &v2->pos, &v3->pos);
	// 按照 Transform 变化
	transform_apply (&device->transform, &c1, &v1->pos);
	transform_apply (&device->transform, &c2, &v2->pos);
	transform_apply (&device->transform, &c3, &v3->pos);

	// 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
	// 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
	int flag[3];
	if (transform_check_cvv(&c1) != 0) { flag[0] = 0; }
	else { flag[0] = 1; }
	if (transform_check_cvv(&c2) != 0) { flag[1] = 0; }
	else { flag[1] = 1; }
	if (transform_check_cvv(&c3) != 0) { flag[2] = 0; }
	else { flag[2] = 1; }
	if (flag[0] + flag[1] + flag[2] < 3) {
		return;
		vertex_t vv1, vv2, vv3;
		if (depth > 5) {
			if (flag[0]) {
				vertex_interp(&vv2, v1, v2, 0.5);
				vertex_interp(&vv3, v1, v3, 0.5);
				device_draw_primitive(device, v1, &vv2, &vv3, depth + 1);
			}
			if (flag[1]) {
				vertex_interp(&vv1, v1, v2, 0.5);
				vertex_interp(&vv3, v2, v3, 0.5);
				device_draw_primitive(device, &vv1, v2, &vv3, depth + 1);
			}
			if (flag[2]) {
				vertex_interp(&vv1, v1, v3, 0.5);
				vertex_interp(&vv2, v2, v3, 0.5);
				device_draw_primitive(device, &vv1, &vv2, v3, depth + 1);
			}
			vertex_interp(&vv1, v1, v2, 0.5);
			vertex_interp(&vv2, v2, v3, 0.5);
			vertex_interp(&vv3, v3, v1, 0.5);
			device_draw_primitive(device, &vv1, &vv2, &vv3, depth + 1);
		}
		else {
			vertex_interp(&vv2, v1, v2, 0.5);
			vertex_interp(&vv3, v1, v3, 0.5);
			device_draw_primitive(device, v1, &vv2, &vv3, depth + 1);
			vertex_interp(&vv1, v1, v2, 0.5);
			vertex_interp(&vv3, v2, v3, 0.5);
			device_draw_primitive(device, &vv1, v2, &vv3, depth + 1);
			vertex_interp(&vv1, v1, v3, 0.5);
			vertex_interp(&vv2, v2, v3, 0.5);
			device_draw_primitive(device, &vv1, &vv2, v3, depth + 1);
			vertex_interp(&vv1, v1, v2, 0.5);
			vertex_interp(&vv2, v2, v3, 0.5);
			vertex_interp(&vv3, v3, v1, 0.5);
			device_draw_primitive(device, &vv1, &vv2, &vv3, depth + 1);
		}
		return;
	}

	// 归一化
	transform_homogenize (&device->transform, &p1, &c1);
	transform_homogenize (&device->transform, &p2, &c2);
	transform_homogenize (&device->transform, &p3, &c3);

	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid_t traps[2];
		int n;

		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;

		vertex_rhw_init (&t1);	// 初始化 w
		vertex_rhw_init (&t2);	// 初始化 w
		vertex_rhw_init (&t3);	// 初始化 w

								// 拆分三角形为0-2个梯形，并且返回可用梯形数量
		n = trapezoid_init_triangle (traps, &t1, &t2, &t3);

		// 依次绘制梯形
		if (n >= 1) device_render_trap (device, &traps[0], diffuse);
		if (n >= 2) device_render_trap (device, &traps[1], diffuse);
	}

	if (render_state & RENDER_STATE_WIREFRAME) {		// 线框绘制
		device_draw_line (device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->foreground);
		device_draw_line (device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->foreground);
		device_draw_line (device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->foreground);
	}
}