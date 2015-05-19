snow
====

Abstract
==

Physic simulations allow the creation of dynamic scenes on the computer. Computer generated images become lively and find use in movies, games and engineering applications. GPGPU techniques make use of the graphics card to simulate physics. The simulation of dynamic snow is still little researched. The Material Point Method is the first technique which is capable of showing the dynamics and characteristics of snow.
The hybrid use of Lagrangian particles and a regular cartesian grid enables solving of partial differential equations. Therefore particles are transformed to the grid.
The grid velocities can be updated with the calculation of gradients in an FEM-manner (finite element method). Finally grid node velocities are weight back to the particles to move them across the scene. This method is coupled with a constitutive model to cover the dynamic nature of snow. This includes collisions and breaking.
This bachelor thesis connects the recent developments in GPGPU techniques of OpenGL with the Material Point Method to efficiently simulate visually compelling, dynamic snow scenes.