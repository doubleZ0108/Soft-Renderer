#pragma once

#include <stdio.h>
#include <malloc.h>

#define BM 19778		// 位图的标志

#define PATH "texture.bmp"  //打开的文件路径 

//判断是否是位图,在0-1字节 
int IsBitMap (FILE *fp) {
	unsigned short s;
	fseek (fp, 0, SEEK_SET);
	fread (&s, 1, 2, fp);
	if (s == BM)
		return 1;
	else
		return 0;
}

//获得图片的宽度,在18-21字节 
long getWidth (FILE *fp) {
	long width;
	fseek (fp, 18L, SEEK_SET);
	fread (&width, 1, 4, fp);
	return width;
}

//获得图片的高度 ，在22-25字节 
long getHeight (FILE *fp) {
	long height;
	fseek (fp, 22L, SEEK_SET);
	fread (&height, 1, 4, fp);
	return height;
}

//获得每个像素的位数,在28-29字节 
unsigned short getBit (FILE *fp) {
	unsigned short bit;
	fseek (fp, 28, SEEK_SET);
	fread (&bit, 1, 2, fp);
	return bit;
}

//获得数据的起始位置
unsigned int getOffSet (FILE *fp) {
	unsigned int OffSet;
	fseek (fp, 10L, SEEK_SET);
	fread (&OffSet, 1, 4, fp);
	return OffSet;
}

//获得图像的数据
void getData (FILE* fp, unsigned char *r, unsigned char *g, unsigned char *b) {
	int i, j = 0;
	int stride;
	unsigned char* pix = NULL;
	long height, width;
	height = getHeight (fp);
	width = getWidth (fp);

	fseek (fp, getOffSet (fp), SEEK_SET);	//找到位图的数据区 

	stride = (24 * width) / 8;	//对齐,计算一个width有多少个8位 
	stride = stride / 4 * 4;		//取四的倍数 r,g,b,alph 
									//写入数组 
	pix = (unsigned char *)malloc (stride);
	for (j = 0; j<height; j++) {
		fread (pix, 1, stride, fp);
		for (i = 0; i<width; i++) {
			*(r + (height - 1 - j) * width + i) = pix[i * 3 + 2];
			*(g + (height - 1 - j) * width + i) = pix[i * 3 + 1];
			*(b + (height - 1 - j) * width + i) = pix[i * 3];
		}
	}

	/*for (j = 0; j<height; j++) {
		for (i = 0; i<width; i++) {
			printf ("%4d-", *(r + i + j));
			printf ("%4d-", *(g + i + j));
			printf ("%4d", *(b + i + j));
			printf ("\t");
		}
	}*/
}