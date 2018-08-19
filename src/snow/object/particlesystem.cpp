#include "particlesystem.hpp"
#include <iostream>

void ParticleSystem::render() const {
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, posB);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // DrawArraysInstanced?! + DRAW ONLY GRID NODES
  glDrawArrays(GL_POINTS, 0, particles.size());

  glDisableVertexAttribArray(0);
}
void ParticleSystem::initParticlesFromFile(const std::string& filename,
                                           const Vector3f& pos,
                                           const Vector3f& scale) {
  std::ifstream file;
  file.open(filename);
  if (file.is_open()) {
    std::cout << "Opened File: " << filename << std::endl;
    std::string line;
    while (std::getline(file, line)) {
      float x = std::stof(line.substr(0, line.find(',')));
      line.erase(0, line.find(',') + 1);
      float y = std::stof(line.substr(0, line.find(',')));
      line.erase(0, line.find(',') + 1);
      float z = std::stof(line);
      // std::cout << x << ", "  << y <<", " << z <<std::endl;
      particles.push_back(
          Particle(Vector3f(scale.x * x + pos.x, scale.y * y + pos.y,
                            scale.z * z + pos.z),
                   Vector3i(0, 0, 0), 6.25e-3f));
    }
    file.close();
  } else {
    std::cout << "Error: File " << filename << " not found." << std::endl;
  }
}

void ParticleSystem::debug() const {
  glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
  std::cout << "Particle" << std::endl;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
  Vector4f* p = (Vector4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4f), GL_MAP_READ_BIT));
  std::cout << "xp: ";
  p[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
  Vector4i* a = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i), GL_MAP_READ_BIT));
  std::cout << "vp: ";
  a[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
  Vector4i* vb = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i), GL_MAP_READ_BIT));
  std::cout << "vpn: ";
  vb[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV0);
  Vector4i* dvb0 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i), GL_MAP_READ_BIT));
  std::cout << "dvpn0: ";
  dvb0[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV1);
  Vector4i* dvb1 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i), GL_MAP_READ_BIT));
  std::cout << "dvpn1: ";
  dvb1[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV2);
  Vector4i* dvb2 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i), GL_MAP_READ_BIT));
  std::cout << "dvpn2: ";
  dvb2[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
  std::cout << "FPpB" << std::endl;
  Matrix4f* f = (Matrix4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrix4f), GL_MAP_READ_BIT));
  // p[15*15*15].print();
  f[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
  std::cout << "FEpB" << std::endl;
  Matrix4f* m = (Matrix4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrix4f), GL_MAP_READ_BIT));
  m[0].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ParticleSystem::initSSBO() {
  glGenBuffers(1, &posB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4f) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pPositions = (Vector4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4f) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pPositions[i] = particles.at(i).position;
    pPositions[i].w = particles.at(i).mass;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_POS_BUFFER, posB);
  std::cout << "Number of particles: " << particles.size() << std::endl;
  std::cout << "ParticlePositionBufferSize: "
            << sizeof(Vector4f) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &velB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4i) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pVelocities = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pVelocities[i] = particles.at(i).velocity;
    pVelocities[i].w = 0;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_BUFFER, velB);
  std::cout << "ParticleVelocityBufferSize: "
            << sizeof(Vector4f) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &velBn);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4i) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pVelocitiesn = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pVelocitiesn[i] = Vector4i(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_BUFFER, velBn);
  std::cout << "ParticleVelocityBufferSize: "
            << sizeof(Vector4i) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &deltaV0);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV0);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4i) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pDeltaV0 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV0[i] = Vector4i(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_0,
                   deltaV0);
  std::cout << "ParticleDelta0VelocityBufferSize: "
            << sizeof(Vector4i) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &deltaV1);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV1);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4i) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pDeltaV1 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV1[i] = Vector4i(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_1,
                   deltaV1);
  std::cout << "ParticleDelta1VelocityBufferSize: "
            << sizeof(Vector4i) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &deltaV2);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV2);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4i) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pDeltaV2 = (Vector4i*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector4i) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV2[i] = Vector4i(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_2,
                   deltaV2);
  std::cout << "ParticleDelta2VelocityBufferSize: "
            << sizeof(Vector4i) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &FEpB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix4f) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pForcesE = (Matrix4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrix4f) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pForcesE[i] = particles.at(i).forceElastic;
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FE_BUFFER, FEpB);
  std::cout << "ParticleForceElasticBufferSize: "
            << sizeof(Matrix4f) * particles.size() / 1024 << " KB" << std::endl;

  glGenBuffers(1, &FPpB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix4f) * particles.size(),
               NULL, GL_STATIC_DRAW);
  pForcesP = (Matrix4f*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrix4f) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pForcesP[i] = particles.at(i).forcePlastic;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FP_BUFFER, FPpB);
  std::cout << "ParticleForceElasticBufferSize: "
            << sizeof(Matrix4f) * particles.size() / 1024 << " KB" << std::endl;
}
void ParticleSystem::updateSSBOBuffer() {}

