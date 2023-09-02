# Ray-Tracing

一个光线追踪渲染器，内容主要来自[Ray Tracing in One Weekend—The Book Series](https://raytracing.github.io/)

## 编译运行

本项目需要支持 `-std=c++20` 的编译器，由于项目后期出现编译时间过长的情况，所以编译过程被拆分为预编译和编译两部分

**预编译 && 编译**
```sh
# 预编译支持多线程编译 -j[线程数]
make pre-build -j8 && make build 
```

**运行**
```sh
make run [图片名]
```

推荐预编译后，编译运行并且记录运行时间（有些场景需要运行很久）
```sh
make build && time make run
```

## 优化

### 多线程

由于整个项目是在 CPU 上运行的，所以在应对像素点很多的情况时，会运行的非常慢，所以采用有多线程，可以基本上使得运行时间 / 核心数。代码详见[这里](https://github.com/clumsy-sy/Ray-Tracing/blob/main/src/renderer/Renderer.hpp#L75), 由于 `shared_ptr` 会导致多线程运行时频繁的加锁，所以新版中直接改为指针（注意析构）。

### SIMD!!!

并发编程非常适合这个项目，因为这个项目中大量运用了 `{double,double,double}` 的向量类型，对于两个向量之间的操作，可以通过 simd 一个指令完成，极大提升效率。代码详见[这里](https://github.com/clumsy-sy/Ray-Tracing/blob/main/src/vector/vec3dx4.h#L11C1-L11C1)

*注意： `-march=native` 的编译指令会自动进行一部分 simd 优化*

## 待完成

- [x] 光线旋转的完善
- [x] 由于原课程升级，项目大更新中！！！
- [ ] SAH 算法的实现，现在的 BVH 并不够智能与高效，需要更加优秀的划分策略。
- [ ] 重要性采样(暂时只支持部分 PDF)
- [ ] CRTP

## 图片展示

![Alt](images/zooTex.bmp)
**上图信息**
> 
> CPU： i7-9750H （笔记本 6 核 12 逻辑处理器）
> 
> 图片大小：600 * 600
> 
> 单像素采样数：16000
>
> 递归深度： 5 
>
> 运行时间：51:11.18


![Alt](images/zoo16384.bmp)
![Alt](images/balls_simple_400.bmp)
![Alt](images/checker_balls_400.bmp)

**上图信息**
> 
> CPU： i7-9750H （笔记本 6 核 12 逻辑处理器）
> 
> 图片大小：1200 * 800
> 
> 单像素采样数：1000
>
> 递归深度： 5 
>
> 运行时间：4:22.13

![Alt](images/all_20000.bmp)
