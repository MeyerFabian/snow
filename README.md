Visuals
====
Click the video folder for a video! :)
or watch on [youtube!](https://www.youtube.com/watch?v=JlNf7CUW4UY)

<img src="https://github.com/MeyerFabian/snow/blob/master/video_and_thesis/young.jpg" width="350"><img src="https://github.com/MeyerFabian/snow/blob/master/video_and_thesis/coll.jpg" width="350">

Project
====
Ready for Windows: mingw-w64, VS (2013 and 2017 tested), NMake(compile_commands activated)
Theoretically portable to unix-systems (no dependency restrictions)

Dependencies
====
[GLEW](http://glew.sourceforge.net/) (Tested 2.1.0, build from source)

[GLFW](http://www.glfw.org/) (Tested 3.2.1, build from source)

[ASSIMP](http://www.assimp.org/index.php/downloads) (Tested 4.1.0, build from source)

[GLM](https://glm.g-truc.net/0.9.9/index.html) (Tested GLM 0.9.9.0, Header only)

Compute Shader ready GFX introduced with OpenGL 4.3

Included Dependencies
====
[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)

[voxelizer](https://github.com/takagi/cl-voxelize/) (A precomputed voxelization of the Stanford-Bunny is already included in resources/model/)

Usage
====
Manipulate the physical constants in defines.h to alter the simulation. (Changes might not be numerically stable) 

Abstract
=====

Physic simulations allow the creation of dynamic scenes on the computer. Computer generated images become lively and find use in movies, games and engineering applications. GPGPU techniques make use of the graphics card to simulate physics. 

The simulation of dynamic snow is still little researched. The Material Point Method is the first technique which is capable of showing the dynamics and characteristics of snow.

The hybrid use of Lagrangian particles and a regular cartesian grid enables solving of partial differential equations. Therefore particles are transformed to the grid.

The grid velocities can be updated with the calculation of gradients in an FEM-manner (finite element method). Finally grid node velocities are weight back to the particles to move them across the scene. This method is coupled with a constitutive model to cover the dynamic nature of snow. This includes collisions and breaking.

This bachelor thesis connects the recent developments in GPGPU techniques of OpenGL with the Material Point Method to efficiently simulate visually compelling, dynamic snow scenes.


