# BigBear Editor 2.0 - 3D Learning-Oriented Engine
BigBear Editor is a free 3D learning-oriented engine project built by me, Bear X (Chinese ♀). Originally, I started to build it in 2018 during my master's degree of computer science. It was intended as a project in which I can organize my knowledge of C++ and OpenGL. After that, I suspended this project because of my busy work and exams. About three months ago, I passed the exam successfully, and I started working on this project again. I discovered how terrible the project I wrote before. So I studied some related knowledge and resources, including rendering, engine architecture, etc., and reconstructed this project. So I named it 2.0. Whenever I learn a piece of knowledge, I will reproduce it in this project. I believe that this is a good way to learn, accumulate and consolidate knowledge! In addition, this project has implemented some basic functions. When I need to reproduce a new technology, I can use many implemented or integrated functions, which greatly improves my learning efficiency! Compared to learning complex engines, such as the source code of Unreal Engine 4, (that's too complicated). It is a better choice to continously build our own micro-engine during the learning process. Anyway, I want to share my source code with other friends or developers who might be going down the same path as mine and need a reference or just something to look up.
## Disclaimer
Although this version 2.0 of the project has indeed undergone a reconstruction, due to time constraints, the code is still somewhat **ugly**.
* Some buttons or menu items on the interface are just interfaces, and I do not implement their functions.
* Some operations is not considered with various situations, nor does it do various troubleshooting work for possible misoperations.
* Some memory is not released properly.
* There are many bugs, and it is easy to crash.

The problems I have listed are the ones that can be easily corrected and realized through simple techniques, but this could take a lot of time. As I said before, this project is just a learning-oriented project. Its purpose is considered to organize and accumulate knowledge, not to provide users with comfortable use. So when these problems did not hinder my study and research, I did not have enough time to correct them for the time being.
In addition, I referred to some code, learning resources, UI designs, and art resources. I'm pretty thankful!
## Environment
* Windows 10. Although I built version 1.0 in Mac, version 2.0 is built on Windows. I do not consider Mac compatibility for the time being.
* QT 5.7.2
* Qt Creator 4.11.1
* MinGW 5.3.0 32bit for C++
* OpenGL. The higher the version, the more effects you can use.
* Python 3.7.9 32bit. Please copy .../Python/Python37-32/Lib into Code/BBearEditor/Engine/Python!
## Features
Here lists the features that already exists in my engine. (Some contents need to be improved) o(╥﹏╥)o
* Fully featured viewer and editor application
  - Object create/delete/set-parent
  - Scene hierarchy viewing
  - Coordinate system and object transformation
  - Movable camera
  - GUI object/material inspector for viewing/editing
  - Drag & Drop operation
* Rendering System
  - Pipeline
    + Forward Rendering
    + Deferred Rendering
  - 2D & 3D
    + Sprite, Canvas, Full screen quad
    + Mesh, Terrain, TBN, Sky box
  - Lighting & Shadows
    + Multiple types of lighting: Directional light, Point light, Spotlight, Arealight
    + Soft Shadow: PCF, PCSS, VSM
    + Spherical Harmonic Lighting: Zonal Harmonics
    + Ray Tracing: Efficient GPU Screen-Space Ray Tracing
    + Photon Mapping
    + GI: SSAO, SSDO, FLC
    + Anisotropic Lighting: KajiyaKay Hair
    + Volume rendering
  - PBR with IBL support
  - Common Shader
    + Water
    + Heat Distort
    + Normal Mapping
    + Translucency
    + Cartoon Rendering
    + Fog
* High Performance Rendering
  - Tiled & Clustered LightCulling
  - Balanced K-D Tree
  - Compute Shader
* Physics
  - SPH
* Math & Geometry
  - Ray
  - Bounding Box
  - Frustum
* 3D FBX Skeletal Animation (version 1.0, It hasn't been reconstructed yet ~ T.T)
* Asset Manager
  - Asset Import & Management
    + Texture
    + OBJ
    + FBX (version 1.0)
    + Python Script
  - Serialization & Deserialization
    + Scene
    + Material
* Memory Pool
* Python Virtual Machine
## TODO
Due to limited time, the functions I listed above may not be as cool as the words seem. Of course, I still have many plans. I have a lot of knowledge I want to learn, and I will continue to improve my project!
- [ ] Continuous reproduction of papers
- [ ] Improve Photon Mapping
- [ ] Bézier curve
- [ ] Perlin noise
- [ ] Improve Volume Rendering
- [ ] Physics Simulation
- [ ] Improve PBR
- [ ] FBX Animation System
- [ ] TFO Particle System
- [ ] Extension of Python
- [ ] Font Rendering
- [ ] LOD Scene
## Contacts and Links
I really look forward to making friends with related interests, and we will share experience and learn together! I'm Big Bear (●￣(ｴ)￣●)
* E-Mail: 184346971@qq.com
## Screen Shot
* PBR Material  
![131350577-20c40653-8f73-4351-a082-0c459e8a5923_副本_副本](https://user-images.githubusercontent.com/31690363/134453521-e4701db7-79a1-4ae1-8546-c00c7b349c00.png)
![38FCC5FFFE520709A82F9B12BDA1662F](https://user-images.githubusercontent.com/31690363/134385568-62c18b04-5e77-48e1-8297-7c663af0c839.png)
* Shadows  
![image](https://user-images.githubusercontent.com/31690363/133650927-56a3da4f-412c-4879-9657-6432c703eb13.png)
![134385270-89fba7aa-2299-4f6f-9a4b-b18cae218e0a_副本_副本](https://user-images.githubusercontent.com/31690363/134453742-a3d9ea43-d961-4c5c-bbbf-0b9066fb9499.png)
* SH_lighting  
![Y(T@48LWH9 2TGQ %GZ3(SE](https://user-images.githubusercontent.com/31690363/133653521-1fb1ec66-cfa4-43c7-9b75-a7674adc0bb2.png)
* Photon Mapping  
A temporary progress, there are still some problems, which will be corrected later~ T.T  
The left figure shows the position of photons. Red photons are the nearest K photons around a point tested.
![{84A H%5_SB` SBFED03BN](https://user-images.githubusercontent.com/31690363/134267061-5f81fd14-94b4-416f-91e0-a082cf47314b.png)
* Particle System  
![134385763-37b9774c-8fae-4f09-8f80-ee26a8634d41_副本](https://user-images.githubusercontent.com/31690363/134453838-3553efd0-7d4b-40a2-b1dc-a49a1d05f5de.png)
* Cartoon Rendering  
Make do with it. Please forgive me for not having art resources...  
![B$)FZ(V$)QNN$ %`26%OXJ5](https://user-images.githubusercontent.com/31690363/134540491-b60f3772-25a2-4447-a67b-a267120256de.png)
* Main interface  
![V%BRYKJB E9I0JK(YGZ7ZMM](https://user-images.githubusercontent.com/31690363/130344475-acf28e7e-d0a0-4c86-860f-ce25d081b559.png)
