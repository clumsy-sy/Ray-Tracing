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

|name|type|description|
|--|--|--|
|Color|color:vec3d|单色|
|Checker|tex1:texture, tex2:texture|两种纹理组成棋盘|
|Image|filename:string|图片采样纹理|
|Noise|scale:double|噪声|

### material 

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

|name|type|description|
|--|--|--|
|Lambertian / LB |tex:texture||
|Dielectric / DI |ir:double||
|Diffuse_light / DL |tex:texture||
|Isotropic / IS |tex:texture||
|Metal / MT |color:vec3d, fuzz:double||


### shape

|name|type|description|
|--|--|--|
|Sph|center:vec3d, radius:double, material||
|Tri|v0:vec3d, v1:vec3d, v2:vec3d, material||
|Quad|Q: vec3d, u: vec3d, v: vec3d, material||
|List|list:[]||
|BVH|bvh:[]||
|Mesh|filename : "", scale:double, material:||
|Box|p_min : vec3d, p_max: vec3d, material||
|Trans|offset : vec3d, OBJ||
|Scale|offset : vec3d, OBJ||
|R_x|angle:double, OBJ||
|R_y|angle:double, OBJ||
|R_z|angle:double, OBJ||