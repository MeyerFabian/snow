#include "particlesystem.h"
#include <iostream>
void ParticleSystem::initVBO() {


}

void ParticleSystem::render(){

    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, posB);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        //DrawArraysInstanced?! + DRAW ONLY GRID NODES
        glDrawArrays(GL_POINTS,0,(particles)->size());


    glDisableVertexAttribArray(0);
}
void ParticleSystem::updateVBOBuffer(){


}
void ParticleSystem::debug(){


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    std::cout << "pos"<<std::endl;
    Vector4f* p = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    p[16*32*32+16*32+16].print();
    p[32-1].print();
    p[32*32-31-1].print();
    p[32*32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    std::cout << "vel"<<std::endl;
    Vector4f* a = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    a[16*32*32+16*32+16].print();
    a[32-1].print();
    a[32*32-31-1].print();
    a[32*32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    std::cout << "velBn"<<std::endl;
    Vector4f* vb = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    vb[32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
    std::cout << "FPpB"<<std::endl;
    Matrix4f* f= (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f)* (particles)->size(), GL_MAP_READ_BIT));
    //p[15*15*15].print();
    f[32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
    std::cout << "FEpB"<<std::endl;
    Matrix4f* m = (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f)* (particles)->size(), GL_MAP_READ_BIT));
    m[32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);



    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gvelBn0);
    std::cout << "dvp"<<std::endl;
    Vector4f* k = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    k[32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gvelBn1);
    Vector4f* j = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    j[32-1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);





}

void ParticleSystem::initSSBO(){



    glGenBuffers(1,&posB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pPositions =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<particles->size();i++){
        pPositions[i] = particles->at(i).position;
        pPositions[i].w = particles->at(i).mass;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_POS_BUFFER , posB);
    std::cout <<"Number of particles: "<< (particles)->size()<<std::endl;
    std::cout << "ParticlePositionBufferSize: "<< sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;


    glGenBuffers(1,&velB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Vector4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pVelocities = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<particles->size();i++){
        pVelocities[i]= particles->at(i).velocity;
        pVelocities[i].w =0.0f;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_BUFFER , velB);
    std::cout << "ParticleVelocityBufferSize: "<<sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&velBn);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Vector4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pVelocitiesn = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_BUFFER , velBn);
    std::cout << "ParticleVelocityBufferSize: "<<sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&gvelBn0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gvelBn0);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Vector4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pdeltaVelocitiesn0 = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));

    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_DELTA_BUFFER_0 , gvelBn0);
    std::cout << "ParticleVelocityBufferSize: "<<sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&gvelBn1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gvelBn1);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Vector4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pdeltaVelocitiesn1 = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));

    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_DELTA_BUFFER_1 , gvelBn1);
    std::cout << "ParticleVelocityBufferSize: "<<sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;



    glGenBuffers(1,&FEpB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Matrix4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pForcesE = (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<particles->size();i++){
        pForcesE[i] = particles->at(i).forceElastic;
    }

    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FE_BUFFER , FEpB);
    std::cout << "ParticleForceElasticBufferSize: "<<sizeof(Matrix4f) * (particles)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&FPpB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Matrix4f) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pForcesP = (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<particles->size();i++){
        pForcesP[i] = particles->at(i).forcePlastic;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FP_BUFFER , FPpB);
    std::cout << "ParticleForceElasticBufferSize: "<<sizeof(Matrix4f) * (particles)->size()/1024 << " KB" <<std::endl;

}
void ParticleSystem::updateSSBOBuffer(){
}
