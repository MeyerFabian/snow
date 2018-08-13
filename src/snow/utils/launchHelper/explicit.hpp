#ifndef LAUNCHER_H
#define LAUNCHER_H
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
#include "../../simulation/myPhysicEngine.hpp"
#include "../../simulation/timeUpdate.hpp"
#include "../defines.hpp"
#include "GLFW/glfw3.h"

int launchSnow(Scene& scene);
#endif

