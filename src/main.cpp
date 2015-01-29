
#include "rendering/myRenderingEngine.h"
#include "simulation/myPhysicEngine.h"
#include <iostream>
#include <memory>
#include "object/mesh.h"
#include <vector>
#include "object/grid.h"
#include "object/particlesystem.h"
#include "object/collisionObjects.h"
#include "simulation/timeUpdate.h"
#include "defines.h"
#include "GLFW/glfw3.h"
#include <time.h>
#include "math.h"
using namespace  std;

double dt =  RENDER_DT;

double t = 0.0;
void scene0(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    int y=0;
    int z=0;
    float xpos=0.8125f,ypos=0.7625f,zpos=0.8125f;
    while(y<32){
        while(z<32){
            while(x<32){
                pPs->particles->push_back(Particle(Vector3f(
                                               xpos + ((float)x)*0.025f,
                                               ypos + ((float)y)*0.025f,
                                               zpos + ((float)z)*0.025f)));

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
    int x = 0;
    float xpos=0.8f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*32){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3)));
        x+=1;
        }
    }
}
void scene2(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=2.0f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(10*1000000,1*1000000,1*1000000)));
        x+=1;
        }
    }

    x = 0;
    xpos=3.5f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(-30*1000000,15*1000000,-1*1000000)));
        x+=1;
        }
    }
}
void scene3(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=1.5f,ypos=2.525f,zpos=5.0f;
    while(x<32*32*32){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(10*1000000,-5*1000000,1*1000000)));
        x+=1;
        }
    }

}
void scene4(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            ypos=GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            zpos=GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING;
    while(x<NUMOFPARTICLES){
        float height = 0.2;
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
    float xpos=GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            ypos=GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            zpos=GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING;
    while(x<NUMOFPARTICLES){
        float height = 0.2;
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

    shared_ptr<Mesh> sphere=make_shared<Mesh>() ;
    sphere->LoadMesh("model/sphere.obj");
    sphere->setPosition(0.4125f,1.3125f,1.8f);
    sphere->setScale(0.2f,0.2f,0.2f);
    sphere->setRotation(0,0,0);

    meshes->push_back(sphere);
    pCO->colliders->push_back(Collider(sphere,0.2f,1,Vector3f(10.0f,-3.0f,0.0f),Vector3f(0.0f,0.0f,0.0f)));


}
void scene6(shared_ptr<ParticleSystem> const pPs,shared_ptr<CollisionObjects> const pCO,shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes){
    int x = 0;
    float xpos=GRID_POS_X+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            ypos=GRID_POS_Y+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING,
            zpos=GRID_POS_Z+GRID_COLLISION_PLANE_OFFSET*GRID_SPACING;
    shared_ptr<Mesh> bunny=make_shared<Mesh>() ;
    std::string filename = "model/bunny.voxel";
    pPs->initParticlesFromFile(filename);

    //1. Test 91897 Particles 400kg/m Density 6.25Mass
    //2. Test 190000 Particles 900kg/m Density 6.25 Mass SAME RESULT
    //3. Test 91897 Particles 600kg/m Density 9.5Mass SAME RESULT

}
int launchSnow(){
    shared_ptr<Grid>const  grid = make_shared<Grid >(GRID_DIM_X,GRID_DIM_Y,GRID_DIM_Z, GRID_SPACING, GRID_POS_X,GRID_POS_Y,GRID_POS_Z);


    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();

    shared_ptr<CollisionObjects> const pCO= make_shared<CollisionObjects > ();

     srand (time(NULL));
     //scene1(pPs);
     //scene2(pPs);
    // scene3(pPs);
/*
    for(int x = 0; x < 32; x+=1){
        xpos += 0.025f;
        for(int y = 0; y < 32; y+=1){
            ypos += 0.025f;
            for(int z = 0; z < 32; z+=1){
                zpos += 0.025f;
                pPs->particles->push_back(Particle(Vector3f(xpos,ypos,zpos)));
            }
            zpos = 5.0f;
        }
        ypos = 2.5125f;
    }
*/

/*
    for(int x = 0; x<32*32*32; x+=1){
        float rand1=(float(rand())/32727.0f)*0.4;
        float rand2=(float(rand())/32727.0f)*0.4;
        float rand3=(float(rand())/32727.0f)*0.4;
        float randpi=(float(rand())/32727.0f)*M_PI;
        float rand2pi=(float(rand())/32727.0f)*2.0f*M_PI;
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1*sin(randpi)*cos(rand2pi),
                                               ypos + rand2*sin(randpi)*sin(rand2pi),
                                               zpos + rand3*cos(randpi))));
    }
*/
/*

    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
*/
    shared_ptr<std::vector<shared_ptr<Mesh> > > const  meshes = make_shared<std::vector<shared_ptr<Mesh>>>();

    scene0(pPs,pCO,meshes);

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
    halfplane4->setPosition(GRID_POS_X+(GRID_DIM_X-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
                            );

    pCO->colliders->push_back(Collider(halfplane4,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(-1.0f,0.0f,0.0f)));

    shared_ptr<Mesh> halfplane5=make_shared<Mesh>() ;
    halfplane5->setPosition(GRID_POS_X+(GRID_DIM_X-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
                            );


    pCO->colliders->push_back(Collider(halfplane5,0.0f,0,Vector3f(0.0f,0.0f,0.0f),Vector3f(0.0f,-1.0f,0.0f)));

    shared_ptr<Mesh> halfplane6=make_shared<Mesh>() ;
    halfplane6->setPosition(GRID_POS_X+(GRID_DIM_X-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Y+(GRID_DIM_Y-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING,
                            GRID_POS_Z+(GRID_DIM_Z-2.0f*GRID_COLLISION_PLANE_OFFSET)*GRID_SPACING
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
   double t = 0.0;
   double currentTime = glfwGetTime(); //gafferongames.com
   double accumulator = 0.0;
  while (rE->shouldClose()){
        //while(sim_t < static_fps){
        //timeStepEnd = timeStart;
        double newTime = glfwGetTime();
        double frameTime = newTime -currentTime;
        currentTime = newTime;
        accumulator += frameTime;
        while (accumulator >= dt){

        //std::cout << (timeStart - timeStepEnd) * 1000 << " ms for TimeStep."<<std::endl;

        pE->update(DT);
        accumulator -=dt;
        t+=dt;
        }
        //sim_t += dt;
        //}

        //timeEndPhysic = glfwGetTime();
        //std::cout << (timeEndPhysic - timeStart) * 1000 << " ms for Physic-Engine."<<std::endl;
        //timeStartRendering = glfwGetTime();
        rE->render();
        //timeEnd = glfwGetTime();
        //std::cout << (timeEnd - timeStartRendering) * 1000 << " ms for Rendering-Engine.\n"<<std::endl << std::flush;

    }
    rE->stop();

}

int main(){

    if(!launchSnow()){
        return 1;
    }

return 0;
}



