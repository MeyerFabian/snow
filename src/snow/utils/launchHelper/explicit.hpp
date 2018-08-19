#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "../../rendering/GLFWWindow.hpp"

#include <time.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
#include "../../object/mesh.hpp"
#include "../../object/particlesystem.hpp"
#include "../../rendering/particleRenderer.hpp"
#include "../../scene/Scene.hpp"
#include "../../simulation/MPMPhysicEngine.hpp"
#include "../../simulation/timeUpdate.hpp"
#include "../benchmarker.hpp"
#include "../defines.hpp"

int launchSnow(Scene& scene);
#endif

