#pragma once

#include "math.h"

//=====================================================================
// 坐标变换
//=====================================================================
typedef struct {
	matrix_t world;         // 世界坐标变换
	matrix_t view;          // 摄影机坐标变换
	matrix_t projection;    // 投影变换
	matrix_t transform;     // transform = world * view * projection
	float w, h;             // 屏幕大小
}	transform_t;


// 矩阵更新，计算 transform = world * view * projection
void transform_update (transform_t *ts) {
	matrix_t m;
	matrix_mul (&m, &ts->world, &ts->view);
	matrix_mul (&ts->transform, &m, &ts->projection);
}

// 初始化，设置屏幕长宽
void transform_init (transform_t *ts, int width, int height) {
	float aspect = (float)width / ((float)height);
	matrix_set_identity (&ts->world);
	matrix_set_identity (&ts->view);
	matrix_set_perspective (&ts->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	ts->w = (float)width;
	ts->h = (float)height;
	transform_update (ts);
}

// 将矢量 x 进行 project 
void transform_apply (const transform_t *ts, vector_t *y, const vector_t *x) {
	matrix_apply (y, x, &ts->transform);
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int transform_check_cvv (const vector_t *v) {
	float w = v->w;
	int check = 0;
	if (v->z < 0.0f) check |= 1;
	if (v->z >  w) check |= 2;
	if (v->x < -w) check |= 4;
	if (v->x >  w) check |= 8;
	if (v->y < -w) check |= 16;
	if (v->y >  w) check |= 32;
	return check;
}

// 归一化，得到屏幕坐标
void transform_homogenize (const transform_t *ts, vector_t *y, const vector_t *x) {
	float rhw = 1.0f / x->w;
	y->x = (x->x * rhw + 1.0f) * ts->w * 0.5f;
	y->y = (1.0f - x->y * rhw) * ts->h * 0.5f;
	y->z = x->z * rhw;
	y->w = 1.0f;
}