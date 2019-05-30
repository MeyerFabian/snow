#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "../../rendering/GLFWWindow.hpp"

#include <time.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../../object/collision/collisionObjects.hpp"
#include "../../object/grid/grid.hpp"
#include "../../object/mesh.hpp"
#include "../../object/particle/particlesystem.hpp"
#include "../../rendering/particleRenderer.hpp"
#include "../../scene/Scene.hpp"
#include "../../simulation/MPMPhysicEngine.hpp"
#include "../../simulation/timeUpdate.hpp"
#include "../benchmarker.hpp"
#include "../defines.hpp"

int launchSnow(Scene& scene);
#endif

