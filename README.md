# 软光栅渲染器 | Soft Renderer

Tongji University Computer Graphics Course Design --- Soft Renderer

* [项目 | Project](#项目--project)
* [实验结果 | Experimental Results](#实验结果--experimental-results)
* [代码 | Code](#代码--code)
* [项目结构 | Project Structure](#项目结构--project-structure)
* [关于作者 | About the Author](#关于作者--about-the-author)

------

## 项目 | Project

1. **概述-渲染流水线**
2. **顶点变换及画线**
3. **光栅化**
4. **纹理**
5. **光照**

<br/>

## 实验结果 | Experimental Results

- **纹理模式**

  <img src="https://upload-images.jianshu.io/upload_images/12014150-74b10239868e43ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="image.png" style="zoom:50%;" />

- **光照效果**

  <img src="https://upload-images.jianshu.io/upload_images/12014150-ad53a504349d5289.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="image.png" style="zoom:50%;" />

- **裁剪改进**

  <img src="https://upload-images.jianshu.io/upload_images/12014150-0f3cab73923e2e75.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="image.png" style="zoom:50%;" />

<br/>

## 代码 | Code

- [bitmap.h 位图图像信息相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/bitmap.h)
- [coordinate.h 坐标变换相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/coordinate.h)
- [device.h 内存分配、渲染设备相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/device.h)
- [geometry.h 几何计算相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/geometry.h)
- [math.h 数学库函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/math.h)
- [render.h 渲染相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/math.h)
- [window.h 窗口设置相关函数](https://github.com/doubleZ0108/Soft-Renderer/blob/master/src/main/cpp/window.h)

<br/>

## 项目结构 | Project Structure

- Release
  - [SoftRasterization.exe](https://github.com/doubleZ0108/Soft-Renderer/blob/master/Release/SoftRasterization.exe)
- document
  - [计算机图形学——软光栅渲染器文档](https://github.com/doubleZ0108/Soft-Renderer/blob/master/doc/%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%9B%BE%E5%BD%A2%E5%AD%A6%E2%80%94%E2%80%94%E8%BD%AF%E5%85%89%E6%A0%85%E6%B8%B2%E6%9F%93%E5%99%A8.pdf)
  - [坐标变换文档](https://github.com/doubleZ0108/Soft-Renderer/blob/master/doc/%E5%9D%90%E6%A0%87%E5%8F%98%E6%8D%A2.pdf)
- presentation
  - [图形学期末答辩](https://github.com/doubleZ0108/Soft-Renderer/blob/master/pre/%E5%9B%BE%E5%BD%A2%E5%AD%A6_%E6%9C%9F%E6%9C%AB%E9%A1%B9%E7%9B%AE%E7%AD%94%E8%BE%A9.pptx)
- src/main
  - cpp
  - python
  - `SoftRasterization.sln`

<br/>

## 关于作者 | About the Author

| 姓名   | 学号    | 工作                                      | 分数比例 |
| ------ | ------- | ----------------------------------------- | -------- |
| 张喆   | 1754060 | 顶点、画线实现，第一、二部分文档，答辩ppt | 35%      |
| 陈开昕 | 1753188 | 光栅化实现，第三、四部分文档              | 35%      |
| 林磊   | 1652699 | 光照实现，第五部分文档，项目整合          | 30%      |

