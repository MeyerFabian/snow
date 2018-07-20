#include "../snow/rendering/myRenderingEngine.hpp"
#include "../snow/simulation/myPhysicEngine.hpp"
#include <iostream>
#include <memory>
#include "../snow/object/mesh.hpp"
#include <vector>
#include "../snow/object/grid.hpp"
#include "../snow/object/particlesystem.hpp"
#include "../snow/object/collisionObjects.hpp"
#include "../snow/simulation/timeUpdate.hpp"
#include "../snow/defines/defines.hpp"
#include "GLFW/glfw3.h"
#include <time.h>
#include "math.h"
using namespace  std;


double t = 0.0;
void scene0(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    int y=0;
    int z=0;
    float xpos=0.9625f,ypos=0.72f,zpos=0.9625;
    while(y<32){
        while(z<32){
            while(x<32){
                pPs->particles->push_back(Particle(Vector3f(
                                               xpos + ((float)x)*0.025f,
                                               ypos + ((float)y)*0.025f,
                                               zpos + ((float)z)*0.025f),Vector3i(0,0,0)));

                x+=1;
            }
            x=0;
            z+=1;

        }
        z=0;

        y+=1;
    }

}

void scene1(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    /*
     *
#define YOUNG_MODULUS 1.4e5
#define POISSON 0.41
#define HARDENING 10.0
#define CRIT_COMPRESSION 2.5e-2
#define CRIT_STRETCH 7.5e-3
*/
    int x = 0;
    float xpos=5.0f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*32){
        float width = 1.0;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        float rand4=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){

            float mass = (rand4>0.5)? (((rand4-0.5)*10.0f)+1.0f)*1.0e-4f:5e-3f;
            pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(0,0,0),mass));
        x+=1;
        }
    }
}

void scene2(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=1.0f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;

        float rand4=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
            //float mass = (rand4>0.5)? rand4*6.0f*5.0e-4f:10e-3f;
            float mass = (rand4>0.5)? (((rand4-0.5)*10.0f)+1.0f)*8.0e-4f:10e-3f;
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(10*1e8,0*1e8,1*1e8),mass));
        x+=1;
        }
    }

    x = 0;
    xpos=4.5f,ypos=1.7125f,zpos=5.5f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;

        float rand4=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
            float mass = (rand4>0.5)? (((rand4-0.5)*10.0f)+1.0f)*8.0e-4f:10e-3f;
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(-12*1e8,1*1e8,-5*1e8),mass));
        x+=1;
        }
    }
}
void scene3(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=3.5f,ypos=3.025f,zpos=5.0f;
    while(x<32*32*32){
        float width = 1.1;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(0*1e6,0*1e6,0*1e6),2.25e-3f));
        x+=1;
        }
    }

}
void scene4(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
 /*
  * #define DT 1e-4
    #define RENDER_DT 0.033333
    #define YOUNG_MODULUS 1.4e5
    #define POISSON 0.2
    #define HARDENING 10.0
    #define CRIT_COMPRESSION 2.5e-2
    #define CRIT_STRETCH 7.5e-3
    */

    int x = 0;
    float xpos=GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            ypos=GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            zpos=GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING;

    while(x<NUMOFPARTICLES){
        float height = 0.5;
        float width = ((GRID_DIM_X-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING);
        //float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*height;
        float rand3=(float(rand())/32727.0f)*width;
       // if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3)));
        x+=1;
      // }
    }

    shared_ptr<Mesh> sphere2=make_shared<Mesh>() ;
    sphere2->LoadMesh("model/sphere.obj");
    sphere2->setPosition(0.4125f,1.0125f,2.8f);
    sphere2->setScale(0.2f,0.2f,0.2f);
    sphere2->setRotation(0,0,0);

    meshes->push_back(sphere2);
    pCO->colliders->push_back(Collider(sphere2,0.2f,1,Vector3f(10.0f,0.0f,0.0f),Vector3f(0.0f,0.0f,0.0f)));

    shared_ptr<Mesh> sphere=make_shared<Mesh>() ;
    sphere->LoadMesh("model/sphere.obj");
    sphere->setPosition(4.0125f,1.0125f,1.2f);
    sphere->setScale(0.2f,0.2f,0.2f);
    sphere->setRotation(0,0,0);

    meshes->push_back(sphere);
    pCO->colliders->push_back(Collider(sphere,0.2f,1,Vector3f(-10.0f,0.0f,0.0f),Vector3f(0.0f,0.0f,0.0f)));

    shared_ptr<Mesh> sphere3=make_shared<Mesh>() ;
    sphere3->LoadMesh("model/sphere.obj");
    sphere3->setPosition(1.2f,1.0125f,0.4125f);
    sphere3->setScale(0.2f,0.2f,0.2f);
    sphere3->setRotation(0,0,0);

    meshes->push_back(sphere3);
    pCO->colliders->push_back(Collider(sphere3,0.2f,1,Vector3f(0.0f,0.0f,10.0f),Vector3f(0.0f,0.0f,0.0f)));

}

void scene5(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=0.8125,
            ypos=GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            zpos=2.4;
    while(x<32*32*64){
        float height = 0.5;
        float width = 2.0;
        float length = 0.8;
        //float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*height;
        float rand3=(float(rand())/32727.0f)*length;
       // if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3)));
        x+=1;
      // }
    }

    shared_ptr<Mesh> sphere=make_shared<Mesh>() ;
    sphere->LoadMesh("model/sphere.obj");
    sphere->setPosition(0.2125f,1.1125f,2.8f);
    sphere->setScale(0.3f,0.3f,0.3f);
    sphere->setRotation(0,0,0);

    meshes->push_back(sphere);
    pCO->colliders->push_back(Collider(sphere,0.3f,1,Vector3f(5.0f,0.0f,0.0f),Vector3f(0.0f,0.0f,0.0f)));


}
void scene6(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    Vector3f pos(5.0f,0.0,5.0f);
    Vector3f scale(12.5f*1.259f,12.5f*1.259f,12.5f*1.259f);

    std::string filename = "model/bunny.voxel";
    pPs->initParticlesFromFile(filename, pos, scale);

    //1. Test 91897 Particles 400kg/m Density 6.25Mass
    //2. Test 190000 Particles 900kg/m Density 6.25 Mass SAME RESULT
    //3. Test 91897 Particles 600kg/m Density 9.5Mass SAME RESULT
}

void scene7(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=2.0f,ypos=1.525f,zpos=5.0f;
    while(x<32*32*32){
        float width = 1.0;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(-10*1e8,0,0),6.25e-3f));
        x+=1;
        }
    }

}

int launchSnow(){
    shared_ptr<Grid>const  grid = make_shared<Grid >(GRID_DIM_X,GRID_DIM_Y,GRID_DIM_Z, GRID_SPACING, GRID_POS_X,GRID_POS_Y,GRID_POS_Z);


    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();

    shared_ptr<CollisionObjects> const pCO= make_shared<CollisionObjects > ();

     srand (time(NULL));

    shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes = make_shared<std::vector<shared_ptr<Mesh>>>();

    scene6(pPs,pCO,meshes);

    shared_ptr<Mesh> halfplane=make_shared<Mesh>() ;
    halfplane->setPosition(GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING
                           ,GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
                           GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING);

    pCO->colliders->push_back(Collider(halfplane,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(1.0f,0.0f,0.0f)));

    shared_ptr<Mesh> halfplane2=make_shared<Mesh>() ;
    halfplane2->setPosition(GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING
                            ,GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
                            GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING);

    pCO->colliders->push_back(Collider(halfplane2,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(0.0f,1.0f,0.0f)));

    shared_ptr<Mesh> halfplane3=make_shared<Mesh>() ;
    halfplane3->setPosition(GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING
                            ,GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
                            GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING);

    pCO->colliders->push_back(Collider(halfplane3,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(0.0f,0.0f,1.0f)));


    shared_ptr<Mesh> halfplane4=make_shared<Mesh>() ;
    halfplane4->setPosition(GRID_POS_X+(GRID_DIM_X-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
                            );

    pCO->colliders->push_back(Collider(halfplane4,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(-1.0f,0.0f,0.0f)));

    shared_ptr<Mesh> halfplane5=make_shared<Mesh>() ;
    halfplane5->setPosition(GRID_POS_X+(GRID_DIM_X-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
                            );


    pCO->colliders->push_back(Collider(halfplane5,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(0.0f,-1.0f,0.0f)));

    shared_ptr<Mesh> halfplane6=make_shared<Mesh>() ;
    halfplane6->setPosition(GRID_POS_X+(GRID_DIM_X-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
                            );


    pCO->colliders->push_back(Collider(halfplane6,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(0.0,0.0f,-1.0f)));

    /*
            Mesh halfplane;
    halfplane.setPosition(0.7125f,0.7125f,0.7125f);
    pCo->colliders->push_back(Collider(halfplane));
*/

    /*
    Mesh quad;
    quad.LoadMesh("model/box.obj");
    quad.setPosition(5.0f,-3.0f,5.0f);
    quad.setScale(10.0f,1.0f,10.0f);
    quad.setRotation(0,0,0);
    meshes->push_back(std::move(quad));

*/
    shared_ptr<renderingEngine> const rE = make_shared<myRenderingEngine>(meshes,pPs, grid);

    shared_ptr<TimeUpdate> const update = make_shared<ExplicitTimeUpdate> (pCO,pPs,grid);

    shared_ptr<physicEngine> const  pE= make_shared<myPhysicEngine>(update);
    bool re_err = rE->init();

    if(re_err){
        return 1;
	}
	
    pE->init();
	double currentTime = glfwGetTime(); //gafferongames.com
	double accumulator = 0.0;
	std::cout << std::flush;
	while (rE->shouldClose()){
		
        double newTime = glfwGetTime();
        double frameTime = newTime -currentTime;
        currentTime = newTime;
        accumulator += frameTime;
        while (accumulator >= STEP_DT){
			pE->update(PHYSIC_DT);
			accumulator -=STEP_DT;
        }
		
		rE->render();

    }
    rE->stop();
    return 0;
}

int main(){

    if(!launchSnow()){
        return 1;
    }

return 0;
}
