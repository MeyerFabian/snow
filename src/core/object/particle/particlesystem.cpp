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
                                           const glm::vec3& pos,
                                           const glm::vec3& scale) {
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
          Particle(glm::vec3(scale.x * x + pos.x, scale.y * y + pos.y,
                             scale.z * z + pos.z),
                   glm::vec3(0.0, 0.0, 0.0), 6.25e-3f));
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
  auto p = (glm::vec4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                         sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "xp: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
  auto a = (glm::vec4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                         sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "vp: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
  auto vb = (glm::vec4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                          sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "vpn: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV0);
  auto dvb0 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "dvpn0: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV1);
  auto dvb1 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "dvpn1: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV2);
  auto dvb2 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4), GL_MAP_READ_BIT));
  std::cout << "dvpn2: ";
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
  std::cout << "FPpB" << std::endl;
  auto f = (glm::mat4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                         sizeof(glm::mat4), GL_MAP_READ_BIT));
  // p[15*15*15].print();
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
  std::cout << "FEpB" << std::endl;
  auto m = (glm::mat4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                         sizeof(glm::mat4), GL_MAP_READ_BIT));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ParticleSystem::initSSBO() {
  glGenBuffers(1, &posB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pPositions = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    glm::vec4 data(particles.at(i).position, particles.at(i).mass);
    pPositions[i] = data;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_POS_BUFFER, posB);
  std::cout << "Number of particles: " << particles.size() << std::endl;
  std::cout << "ParticlePositionBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &velB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pVelocities = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    glm::vec4 data(particles.at(i).velocity, 0.0f);
    pVelocities[i] = data;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_BUFFER, velB);
  std::cout << "ParticleVelocityBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &velBn);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pVelocitiesn = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pVelocitiesn[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_VEL_N_BUFFER, velBn);
  std::cout << "ParticleVelocityBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &deltaV0);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV0);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pDeltaV0 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV0[i] = glm::vec4(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_0,
                   deltaV0);
  std::cout << "ParticleDelta0VelocityBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &deltaV1);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV1);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pDeltaV1 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV1[i] = glm::vec4(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_1,
                   deltaV1);
  std::cout << "ParticleDelta1VelocityBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &deltaV2);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, deltaV2);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pDeltaV2 = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pDeltaV2[i] = glm::vec4(0, 0, 0, 0);
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_DELTA_VEL_BUFFER_2,
                   deltaV2);
  std::cout << "ParticleDelta2VelocityBufferSize: "
            << sizeof(glm::vec4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &FEpB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FEpB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pForcesE = (glm::mat4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pForcesE[i] = particles.at(i).forceElastic;
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FE_BUFFER, FEpB);
  std::cout << "ParticleForceElasticBufferSize: "
            << sizeof(glm::mat4) * particles.size() / 1024 << " KB"
            << std::endl;

  glGenBuffers(1, &FPpB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, FPpB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * particles.size(),
               NULL, GL_STATIC_DRAW);
  auto pForcesP = (glm::mat4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4) * particles.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < particles.size(); i++) {
    pForcesP[i] = particles.at(i).forcePlastic;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PARTICLE_FP_BUFFER, FPpB);
  std::cout << "ParticleForceElasticBufferSize: "
            << sizeof(glm::mat4) * particles.size() / 1024 << " KB"
            << std::endl;
}
void ParticleSystem::updateSSBOBuffer() {}

