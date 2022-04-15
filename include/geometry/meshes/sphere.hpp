#pragma once

#include <geometry/meshCommon.hpp>
#include <utils/intX.hpp>
#include <geometry/meshes/icosahedron.hpp>

#include <geometry/meshCommon.hpp>
#include <glm/gtx/norm.hpp>

namespace meshes {
shapeMesh createSphere(const shapeMesh& oldSphere, u32 extraDepth);

static shapeMesh sphere1X = createSphere(meshes::icosahedron, 2);
static shapeMesh sphere2X = createSphere(sphere1X, 2);
static shapeMesh sphere3X = createSphere(sphere2X, 2);

}
