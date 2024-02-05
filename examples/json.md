# json 场景描述

尝试用 json 来描述场景，通过读取 json 文件来配置和生成场景。

## 配置

> 注：类型 point = color = vec3d = [double, double, double]

|key|value|description|
|--|--|--|
|inherit|[file,...]|继承自别的 json 文件|
|scene_id|int|旧的通用场景|
|image_name|string|生成的图片名|
|image_width|int|图像宽度|
|image_height|int|图像长度|
|aspect_ratio|double|宽长比|
|lookfrom|[double, double, double]|摄像机位置|
|lookat|[double, double, double]|摄像机看向方向|
|vup|[double, double, double]|摄像机向上方向|
|vfov|double|摄像机视角|
|aperture|double|摄像机光圈大小|
|background|[double, double, double]|摄像机光圈大小|
|max_depth|int|光线最大弹射次数|
|pps|int|单像素采样次数|
|threads|int|启用多线程数|
|objects|{}|场景描述|

### texture

|name|type|description|
|--|--|--|
|Color|color:vec3d|单色|
|Checker|tex1:texture, tex2:texture|两种纹理组成棋盘|
|Image|filename:string|图片采样纹理|
|Noise|scale:double|噪声|

```json
{
  "texture":{
    "t1" : {
      "type": "Color",
      "color": [1.0, 1.0, 1.0]
    },
    "t2":{
      "type": "Color",
      "color": [0.0, 0.0, 0.0]
    },
    "c1":{
      "type": "Checker",
      "tex1": "t1",
      "tex2": "t2"
    },
    "i1":{
      "type": "Image",
      "filename": "XXX.png"
    },
    "n1":{
      "type": "Noise",
      "scale": 1.0
    }
  }
}
```

### material 

|name|type|description|
|--|--|--|
|Lambertian / LB |tex:texture||
|Dielectric / DI |ir:double||
|Diffuse_light / DL |tex:texture||
|Isotropic / IS |tex:texture||
|Metal / MT |color:vec3d, fuzz:double||


```json
{
  "material":{
    "l1":{
      "type": "Lambertian",
      "tex": "t1"
    },
    "d1":{
      "type": "Dielectric",
      "ir": 1.5
    },
    "m1":{
      "type": "Metal",
      "color": [1.0, 0.5, 0.2],
      "fuzz": 0.1
    }
  }
}
```

### shape

|name|type|description|
|--|--|--|
|Sph / sphere|center:vec3d, radius:double, material|球体|
|Tri / triangle|v0:vec3d, v1:vec3d, v2:vec3d, material|三角面片|
|Quad / quad|Q: vec3d, u: vec3d, v: vec3d, material|四边形（可以是三角形）|
|List / list|list:[]|数组，内部可以是 object|
|BVH / bvh|bvh:[]|数组，内部object 会构建成 bvh 树|
|Mesh / mesh|filename : "", scale:double, material:|三角形网格，支持 obj 文件|
|Box / box|p_min : vec3d, p_max: vec3d, material|立方体|
|Trans / transtion|offset : vec3d, OBJ|平移|
|Scale / scale|offset : vec3d, OBJ|缩放|
|R_x / rotate_x|angle:double, OBJ|绕 x 旋转|
|R_y / rotate_y|angle:double, OBJ|绕 y 旋转|
|R_z / rotate_z|angle:double, OBJ|绕 z 旋转|

```json
{
  "objects":{
    "a" : {
      "type" : "Quad",
      "Q" :[555, 0, 0],
      "u" :[0, 0, 555],
      "v" :[0,555,0],
      "material" : "green"
    },
}
```

## 样例

```json
{
  "image_name":"cornell.bmp",
  "width": 600,
  "height": 600,
  "lookfrom": [278, 278, -800],
  "lookat": [278, 278, 0],
  "vup": [0.0, 1.0, 0.0],
  "aperture":0.1,
  "vfov": 40.0,
  "max_depth": 5,
  "pps": 100,
  "threads": 256,
  "background": [0.0, 0.0, 0.0],
  "texture":{
    "red" : {
      "type": "Color",
      "color": [0.65, 0.05, 0.05]
    },
    "white":{
      "type": "Color",
      "color": [0.73, 0.73, 0.73]
    },
    "green" : {
      "type": "Color",
      "color": [0.12, 0.45, 0.15]
    },
    "wlight" : {
      "type": "Color",
      "color": [15, 15, 15]
    }
  },
  "material": {
    "red":{
      "type": "Lambertian",
      "tex": "red"
    },
    "white":{
      "type": "Lambertian",
      "tex": "white"
    },
    "green":{
      "type": "Lambertian",
      "tex": "green"
    },
    "light":{
      "type": "Diffuse_light",
      "tex": "wlight"
    }
  },
  "objects":{
    "a" : {
      "type" : "Quad",
      "Q" :[555, 0, 0],
      "u" :[0, 0, 555],
      "v" :[0,555,0],
      "material" : "green"
    },
    "b" : {
      "type" : "Quad",
      "Q" :[0, 0, 555],
      "u" :[0, 0, -555],
      "v" :[0,555,0],
      "material" : "red"
    },
    "c" : {
      "type" : "Quad",
      "Q" :[0, 555, 0],
      "u" :[555, 0, 0],
      "v" :[0,0,555],
      "material" : "white"
    },
    "d" : {
      "type" : "Quad",
      "Q" :[0, 0, 555],
      "u" :[555, 0, 0],
      "v" :[0,0,-555],
      "material" : "white"
    },
    "e" : {
      "type" : "Quad",
      "Q" :[555, 0, 555],
      "u" :[-555, 0, 0],
      "v" :[0,555,0],
      "material" : "white"
    },
    "light": {
      "is_light" : true,
      "type" : "Quad",
      "Q" :[213, 554, 227],
      "u" :[130, 0, 0],
      "v" :[0,0,105],
      "material" : "light"
    }
  }
}
```

```json
{
    "image_name": "img.bmp",
    "max_depth":5,
    "pps": 100,
    "inherit": "examples/cornell.json",
    "material":{
        "glass":{
            "type": "Dielectric",
            "ir": 1.5
        }
    },
    "objects":{
        "box1":{
            "type": "Trans",
            "offset":[265,0,295],
            "obj" : {
                "type": "R_y",
                "angle":15,
                "obj":{
                    "type": "Box",
                    "p_min":[0.0, 0.0, 0.0],
                    "p_max":[165.0, 330.0, 156.0],
                    "material":"white"
                }
            }
        },
        "sphere1":{
            "is_light":true,
            "type":"Sph",
            "center":[190,90,190],
            "radius": 90.0,
            "material":"glass"
        }
    }
}
```
