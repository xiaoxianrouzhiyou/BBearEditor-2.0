# BigBear Editor 2.0 - 3D Learning-Oriented Engine
BigBear Editor is a free 3D learning-oriented engine project built by me, Bear X (Chinese). Originally, I started to build in 2018 during my master's degree of computer science. It was intended as a project in which I can organize my knowledge of C++ and OpenGL. After that, I suspended this project because of my busy work and taking exams. About three months ago, I passed the exam successfully, and I started working on this project again. I discovered how terrible the project I wrote before. So I studied some related knowledge and resources, including rendering, engine architecture, etc., and reconstructed this project. So I named it 2.0. Whenever I learn a piece of knowledge, I will reproduce it in this project. I believe that this is a good way to learn, accumulate and consolidate knowledge! In addition, this project has implemented some basic functions. When I need to reproduce a new technology, I can use many implemented or integrated functions, which greatly improves my learning efficiency! Compared to learning complex engines, such as the source code of Unreal Engine 4, (that's too complicated). It is a better choice to continously build our own micro-engine during the learning process. Anyway, I want to share my source code with other friends or developers who might be going down the same path as mine and need a reference or just something to look up.
## Disclaimer
Although this version 2.0 of the project has indeed undergone a reconstruction, due to time constraints, the code is still somewhat **ugly**.
* Some buttons or menu items on the interface are just interfaces, and I do not implement their functions.
* Some operations do not be considered with various situations, nor does it do various troubleshooting work for possible misoperations.
* Some memory was not released properly.
* There are many bugs, it is easy to crash.

The problems I have listed are problems that can be easily corrected and realized through simple techniques, but they take a lot of time. As I said before, this project is just a learning-oriented project. Its purpose is considered to organize and accumulate knowledge, not to provide users with comfortable use. So when these problems did not hinder my study and research, I did not have enough time to correct them for the time being.
In addition, I referred to some code, learning resources, UI designs, and art resources. I'm pretty thankful!
## Environment
* Windows 10. Although I built version 1.0 in Mac, version 2.0 is built on Windows. I do not consider Mac compatibility for the time being.
* QT 5.7.2
* Qt Creator 4.11.1
* MinGW 5.3.0 32bit for C++
* OpenGL. The higher the version, the more effects you can use.
* Python 3.7.9 32bit. Please copy .../Python/Python37-32/Lib into Code/BBearEditor/Engine/Python!
## Features
* Encapsulation of rendering system, Forward rendering & Deferred rendering
* Editor: create models, move objects, asset management, adjust parameters ...
* 2D & 3D
* Reproduce some classic algorithms: GI, Spherical Harmonic Lighting, Ray Tracing, ...
* Common shader: water, cartoon fire, heat distort, normal map, PBR, translucent transmission ...
* Particle System: SSBO & compute shader
* Python virtual machine
* Serialization and Deserialization: scene & material
## TODO
Due to limited time, the functions I listed above may not be as cool as the words seem. Basically, for each effect, I select an article to read and reproduce. Of course, I still have many plans. I have a lot of knowledge I want to learn, and I will continue to improve my project!
- [ ] Support multiple materials in a scene
- [ ] Soft shadow : "Variance Shadow Maps"
- [ ] Cartoon rendering
- [ ] Volume rendering
- [ ] Physics simulation
- [x] Improve PBR
- [ ] FBX animation system
- [ ] TFO particle system
- [ ] Continuous reproduction of papers
- [ ] Extension of Python
- [ ] Font
- [ ] LOD
- [ ] Improve storage of scene & material 
## Contacts and Links
I really look forward to making friends with related interests, and we will share experience and learn together!
* E-Mail: 184346971@qq.com
## Screen Shot
PBR

![`B)OC_281{{(7(K FV5CJN6](https://user-images.githubusercontent.com/31690363/131146674-08715915-90ef-4b62-aead-2cd973261e00.png)
![CD}KPOUNK9D2(S_{TIFNTE3](https://user-images.githubusercontent.com/31690363/131149330-4a244459-9d51-4af8-8a71-f83309ad4f43.png)
![JBT58EORSICKGY$~J14YK)L](https://user-images.githubusercontent.com/31690363/131350577-20c40653-8f73-4351-a082-0c459e8a5923.png)

SH_lighting
![image](https://user-images.githubusercontent.com/31690363/128983508-344a9bbb-99dc-4ebf-af64-c9daea820d98.png)
![V%BRYKJB E9I0JK(YGZ7ZMM](https://user-images.githubusercontent.com/31690363/130344475-acf28e7e-d0a0-4c86-860f-ce25d081b559.png)
