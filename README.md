# BigBear Editor 2.0 - 3D Learning-Oriented Engine
BigBear Editor is a free 3D learning-oriented engine project built by me, Bear X (Chinese ♀). Originally, I started to build it in 2018 during my master's degree of computer science. It was intended as a project in which I can organize my knowledge of C++ and OpenGL. After that, I suspended this project because of my busy work and exams. About May, I passed the exam successfully, and I started working on this project again. I discovered how terrible the project I wrote before. So I studied some related knowledge and resources, including rendering, engine architecture, etc., and reconstructed this project. So I named it 2.0. Whenever I learn a piece of knowledge, I will reproduce it in this project. I believe that this is a good way to learn, accumulate and consolidate knowledge! In addition, this project has implemented some basic functions. When I need to reproduce a new technology, I can use many implemented or integrated functions, which greatly improves my learning efficiency! Compared to learning complex engines, such as the source code of Unreal Engine 4, (that's too complicated). It is a better choice to continously build our own micro-engine during the learning process. Anyway, I want to share my source code with other friends or developers who might be going down the same path as mine and need a reference or just something to look up.
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
    + Volume Rendering: Cloud
  - PBR with IBL support
  - Physics
    + Fluid: SSF
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
  - Fluid: SPH, PCISPH
  - Cloth: PBD
* Math & Geometry
  - Ray
  - Bounding Box
  - Collision Detection
  - Frustum
  - Marching Cubes
  - Perlin Noise
  - Mesh Subdivision: Catmull-Clark
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
* Memory Pool: TLSF
* Python Virtual Machine
## TODO
Due to limited time, the functions I listed above may not be as cool as the words seem. Of course, I still have many plans. I have a lot of knowledge I want to learn, and I will continue to improve my project!
- [ ] Continuous reproduction of papers
- [ ] Improve Photon Mapping
- [ ] Bézier curve
- [ ] Improve Volume Rendering
- [ ] More constraints for cloth
- [ ] Improve PBR
- [ ] Animation System
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
* Volume Cloud  
![Volume Cloud ](https://user-images.githubusercontent.com/31690363/141683287-65fd0c7e-6889-4580-a7db-2781a40bcaf2.gif)
* SPH Fluid  
![fluid](https://user-images.githubusercontent.com/31690363/135880900-6845f483-917b-4d77-9267-7720caebc0ed.gif)
* Marching Cubes  
![fluid2](https://user-images.githubusercontent.com/31690363/136342552-8d352ded-e7fd-4d11-bbf8-f4db6fe13e16.gif)
* Cartoon Rendering  
Make do with it. Please forgive me for not having art resources...  
![B$)FZ(V$)QNN$ %`26%OXJ5](https://user-images.githubusercontent.com/31690363/134540491-b60f3772-25a2-4447-a67b-a267120256de.png)
* Main interface  
![V%BRYKJB E9I0JK(YGZ7ZMM](https://user-images.githubusercontent.com/31690363/130344475-acf28e7e-d0a0-4c86-860f-ce25d081b559.png)
## References
* Papers  
[1] Fernando R. Percentage-Closer Soft Shadows. 2005.  
[2] Donnelly W, Lauritzen A. Variance shadow maps[C]// 2006:161-165.  
[3] Green R. Spherical harmonic lighting: The gritty details[C]// Game Developers Conference. 2003.  
[4] Ravi Ramamoorthi, Pat Hanrahan. An Efficient Representation for Irradiance Environment Maps. Stanford University.  
[5] Mara M. Efficient GPU Screen-Space Ray Tracing.  
[6] Dachsbacher C, Stamminger M. Reflective shadow maps[C]// the 2005 symposium. 2005.  
[7] Ritschel T, Grosch T, Seidel H P. Approximating Dynamic Global Illumination in Image Space[C]// Symposium on Interactive 3D graphics and games. MPI Informatik;MPI Informatik;MPI Informatik;, 2009.  
[8] Laurent G, Delalandre C, de LA RIVIèRE, Grégoire, et al. Forward Light Cuts: A Scalable Approach to Real-Time Global Illumination[J]. Computer Graphics Forum, 2016, 35(4):79-88.  
[9] Alexander Majercik, Cyril Crassin, Peter Shirley, and Morgan McGuire, A Ray-Box Intersection Algorithm and Efficient Dynamic Voxel Rendering, Journal of Computer Graphics Techniques (JCGT), vol. 7, no. 3, 66-81, 2018  
[10] Colin Barré-Brisebois, Marc Bouchard. Real-Time Approximation of Light Transport in Translucent Homogenous Media, Gpu Pro 2. Vol. 2. CRC Press, 2011.  
[11] Olsson O, Assarsson U. Tiled Shading[J]. Journal of Graphics Gpu & Game Tools, 2011, 15(4):235-251.  
[12] Solenthaler B, Pajarola R. Predictive-Corrective Incompressible SPH[J]. ACM Transactions on Graphics (TOG), 2009, 28(3):187-192.  
[13] Matthias Müller, Heidelberger B, Hennix M, et al. Position Based Dynamics[J]. Journal of Visual Communication & Image Representation, 2007, 18(2):109-118.  
[14] William E. Lorensen and Harvey E. Cline. 1987. Marching cubes: A high resolution 3D surface construction algorithm. SIGGRAPH Comput. Graph. 21, 4 (August 1987), 163-169.  
[15] Perlin K. An image synthesizer[J]. ACM Siggraph Computer Graphics, 1985, 19(3): 287-296.  
[16] E. Catmull, J. Clark.Recursively generated B-spline surfaces on arbitrary topological meshes[J]. Computer-Aided Design,1978,10(6).350-355.  
[17] Masmano M, Ripoll I, Crespo A, et al. TLSF: a new dynamic memory allocator for real-time systems[C]// Proceedings. 16th Euromicro Conference on Real-Time Systems, 2004. ECRTS 2004. IEEE, 2004.  

* Technical blog and Source code  
[18] [JoeyDeVries/ LearnOpenGL](https://learnopengl.com/)  
[19] [OGLdev/ Modern OpenGL Tutorials](https://ogldev.org/)  
[20] [AngelMonica126/ GraphicAlgorithm](https://github.com/AngelMonica126/GraphicAlgorithm)  
[21] [Peter Shirley/ Ray Tracing in a Weekend](http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf)  
[22] [Peter Shirley/ Ray Tracing: The Next Week](http://www.realtimerendering.com/raytracing/Ray%20Tracing_%20The%20Next%20Week.pdf)  
[23] [Peter Shirley/ Ray Tracing: The Rest of Your Life](http://www.realtimerendering.com/raytracing/Ray%20Tracing_%20the%20Rest%20of%20Your%20Life.pdf)  
[24] [Dezeming Family/ PhotonMap](https://dezeming.top/wp-content/uploads/2021/04/PhotonMap.pdf)  
[25] ATI Research/ Hair Rendering and Shading  
[26] [面向搜索引擎编程/ RayMarching实时体积云渲染入门(RayMarching Real-Time Volume Cloud Rendering)](https://zhuanlan.zhihu.com/p/248406797)  
[27] [0小龙虾0/ 流体模拟：SPH (Fluid Simulation: SPH)](https://blog.csdn.net/qq_39300235/category_9582963.html)  
[28] [Lee Winter/ 基于PBD算法的布料和头发的实时模拟（Unity Demo）(Real-Time Simulation of Cloth and Hair based on PBD Algorithm (Unity Demo))](https://zhuanlan.zhihu.com/p/338474692)  

* General documentation and tutorials  
[29] [Lingqi/ GAMES101](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html)  
[30] [Lingqi/ GAMES202](https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html)  
[31] [孙小磊/ 计算机图形学系列笔记(Computer Graphics Series Notes)](https://www.zhihu.com/column/c_1249465121615204352)  
[32] 秦春林/ 全局光照技术:从离线到实时渲染(Global Illumination Technology: From Offline to Real-Time Rendering)  
[33] NVIDIA/ GPU Gems  
[34] NVIDIA/ Screen Space Fluid Rendering for Games  
[35] [长尾科技/ 最美的公式：你也能懂的麦克斯韦方程组(The Most Beautiful Formula: Maxwell's Equations You Can Understand)](https://zhuanlan.zhihu.com/p/71793554)  
[36] [国防科学技术大学/ 大学物理-振动、波动和波动光学(University Physics - Vibration, Wave and Wave Optics)](https://www.icourse163.org/course/NUDT-50002?tid=1465143463)  
[37] 刘利刚/ GAMES102:几何建模与处理(GAMES102: Geometric Modeling & Processing)
  
  todo... (There are some that haven't been listed yet)
