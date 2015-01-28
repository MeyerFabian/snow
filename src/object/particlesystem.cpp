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
void ParticleSystem::initParticlesFromFile(const std::string& filename){
    std::ifstream file;
    file.open(filename);
    if(file.is_open()){
        std::cout << "Opened File: " << filename  << std::endl;
        std::string line;
        while(std::getline(file,line)){
            float x = std::stof(line.substr(0,line.find(',')));
            line.erase(0,line.find(',')+1);
            float y = std::stof(line.substr(0,line.find(',')));
            line.erase(0,line.find(',')+1);
            float z = std::stof(line);
            //std::cout << x << ", "  << y <<", " << z <<std::endl;
            this->particles->push_back(Particle(Vector3f(12.5f*x+4.1f,12.5f*y+0.3225f,12.5f*z+4.5f)));
        }
        file.close();
    }
    else{
        std::cout << "Error: File "<< filename << " not found." <<std::endl;
    }

}

void ParticleSystem::debug(){

    std::cout << "Particle"<<std::endl;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    Vector4f* p = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (particles)->size(), GL_MAP_READ_BIT));
    std::cout << "xp: ";p[5+32*5].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    Vector4i* a = (Vector4i*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4i)* (particles)->size(), GL_MAP_READ_BIT));
    std::cout << "vp: ";a[5+32*5].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    Vector4i* vb = (Vector4i*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,3* sizeof(Vector4i)* (particles)->size(), GL_MAP_READ_BIT));
    std::cout << "vpn: "; vb[3*(5+32*5)].print();
    std::cout << "dvp1: ";vb[3*(5+32*5)+1].print();
    std::cout << "dvp2: ";vb[3*(5+32*5)+2].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
    std::cout << "FPpB"<<std::endl;
    Matrix4f* f= (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f)* (particles)->size(), GL_MAP_READ_BIT));
    //p[15*15*15].print();
    f[(5+32*5)].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
    std::cout << "FEpB"<<std::endl;
    Matrix4f* m = (Matrix4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Matrix4f)* (particles)->size(), GL_MAP_READ_BIT));
    m[(5+32*5)].print();
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
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof (Vector4i) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pVelocities = (Vector4i*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4i) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<particles->size();i++){
        pVelocities[i]= particles->at(i).velocity;
        pVelocities[i].w =0;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_BUFFER , velB);
    std::cout << "ParticleVelocityBufferSize: "<<sizeof(Vector4f) * (particles)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&velBn);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    glBufferData(GL_SHADER_STORAGE_BUFFER,3* sizeof (Vector4i) * (particles)->size(), NULL, GL_STATIC_DRAW);
    pVelocitiesn = (Vector4i*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,3* sizeof(Vector4i) * (particles)->size(), GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT));
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_BUFFER , velBn);
    std::cout << "ParticleVelocityBufferSize: "<< 3* sizeof(Vector4i) * (particles)->size()/1024 << " KB" <<std::endl;




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
