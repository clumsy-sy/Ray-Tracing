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

由于整个项目是在 CPU 上运行的，所以在应对像素点很多的情况时，会运行的非常慢，所以采用有多线程，可以基本上使得运行时间 / 核心数。代码详见[这里](https://github.com/clumsy-sy/Ray-Tracing/blob/main/src/renderer/Renderer.hpp#L75), 由于 `shared_ptr` 会导致多线程运行时频繁的加锁，所以新版中改为 `unique_ptr`。

### SIMD!!!

并发编程非常适合这个项目，因为这个项目中大量运用了 `{double,double,double}` 的向量类型，对于两个向量之间的操作，可以通过 simd 一个指令完成，极大提升效率。代码详见[这里](https://github.com/clumsy-sy/Ray-Tracing/blob/main/src/vector/vec3dx4.h#L11C1-L11C1)

*注意： `-march=native` 的编译指令会自动进行一部分 simd 优化*

## 任务安排

- [X] 支持三角形 + OBJ 文件
- [X] 多线程 + SIMD
- [x] 光线旋转的完善
- [x] 由于原课程升级，项目大更新中！！！
- [ ] 多线程调度升级
- [ ] SAH 算法的实现，现在的 BVH 并不够智能与高效，需要更加优秀的划分策略。
- [X] 重要性采样
- [ ] CRTP
- [ ] 曲面细分
- [ ] 场景信息由 Json 表示

## 图片展示

![Alt](images/zooImg.bmp)
**上图信息**
> 
> CPU： i9-13980HX
> 
> 图片大小：600 * 600
> 
> 单像素采样数：40000
>
> 递归深度： 10
>
> 运行时间：25:54

![Alt](images/balls_simple_400.bmp)
![Alt](images/checker_balls_400.bmp)
