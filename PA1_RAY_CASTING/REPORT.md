<font size = "6em" ><bold>**PA1: 光线投射** &nbsp;&nbsp;</bold></font> <small>2021080070 计14 韩佑硕 </small> 

# **1.代码逻辑**
## **1-1. main**
`main`中的主逻辑采用了 README.pdf上的逻辑。
## **1-2. Group (Intersect)**
被用处 ： 算出刚产生的视线，跟`image`中是否有`interesction`。  
由于`Transform`可能改变方向向量的长度，每次求交先进行`normalization`  
```C++
// main.cpp
bool isIntersect = baseGroup->intersect(camRay, hit, 0); // 0 means background
```
关于`t`和`tmin`，用一下代码实现了。
```c++
if (t < tmin || t > hit.getT()) return false;
```
### **1-2-1. Sphere**
采用上课讲的几何方法来实现了。  
发现  
![与球面交点求法——几何求法](./images/2023-04-17-21-38-48.png)
此处 $d^2$ 不用求其算数平方根。  
```c++
float d2 = Vector3f::dot(l, l) - tp*tp;
float t2 = radius*radius - d2;
if ( t2<0 ) return false;
float t = tp - sqrt(t2);
```
节省了一次 sqrt运算。

### **1-2-2. Plane**
采用上课讲的方法来实现了。  
需要考虑平面与方向向量平行的情况。  
```C++
// 特殊情况 ： 平行
float cos = Vector3f::dot(normal, P_dir);
if (fabs(cos) < 1e-6) return false;
```
### **1-2-3. Triangle**
1. 判断光线和三角形所在平面的交点。  
2. 判断光线与平面的交点是否在三角形内部。  
至于第二步，采用了如下方法  
1. 以逆时针放置a, b, c，且设normal vector 为 $(b-a) \times (c-a)$  ![intersect_triangle1](./images/2023-04-17-22-15-23.png)
2. 对于三角形某一条边(比如ab)， 该交点在三角形内部的那一侧等价于$(a-p) \times (b-p)$的方向与normal vector 同向。  ![intersect_triangle2](./images/2023-04-17-22-17-38.png)

对应的核心代码为 inTriangle 函数
```C++
 bool inTriangle(const Vector3f& p) {
     return Vector3f::dot(Vector3f::cross((vertices[0] - p), (vertices[1] - p)), normal) >= -1e-6 &&
            Vector3f::dot(Vector3f::cross((vertices[1] - p), (vertices[2] - p)), normal) >= -1e-6 &&
            Vector3f::dot(Vector3f::cross((vertices[2] - p), (vertices[0] - p)), normal) >= -1e-6;
 }
```
## **1-3. Material (Phong Model)**
实现 Phong 模型计算局部光强，跟intersect同样理由要注意 normalization。  
clamp函数实现如下。
```C++
float clamp(float x) { return std::max((float)0, x); }
```

## **1-4. Camera (generate Ray)**
阅读  
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
以及  
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/standard-coordinate-systems.html  
里的相关知识，引入aspect变量而实现了。  
但PA1里所有的width = height， 所以该值就是1。  
并且本人从Raster Space 到 Camera Space 坐标变换时，直接进行变换（不引入NDC space），
而发现在本次实验中，当Camera Space原点为O(O_x,O_y)时，Raster Space 原点为O_R(O_x - w/2,O_y - h/2) 故此如下实现了代码。
```C++
float Camera_x = (point.x() - width / 2.0f);
float Camera_y = (point.y() - height / 2.0f);
Vector3f rayDirOnCamera = Vector3f(Camera_x, Camera_y, fy).normalized(); // direction of ray in Camera space
```

# **2. 代码参考**
本人自己完成了本次PA。

# **3. 遇到的困难**
1. 一开始没考虑到 transform后物体求交的情况，没进行相应的 normalization，结果导致呈现错误结果。  
解决办法 ： 每次intersection开始时进行`normalization`  
![scene03_sphere.txt 错误显示](./images/2023-04-17-21-33-24.png)
2. 特殊情况 ： 平行。 若当前object为平面或者三角形时，视线方向向量可能跟object 平行，这情况要特殊处理，而一开始没有考虑到这种情况。
# **4. 未解决的困难**
很感谢，没有未解决的困难。
# **5. 建议**
非常感谢各位助教和老师布置这样富有含金量、高质量的作业。这份作业让我深入了解了课程内容，并提高了我对光线跟踪的理解。  
我认为这份作业非常有挑战性，但有一些问题可能需要更具体的要求或更明确的指导，以便我们更好地理解并完成作业。