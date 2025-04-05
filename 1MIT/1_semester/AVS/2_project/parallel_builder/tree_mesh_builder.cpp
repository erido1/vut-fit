/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Erika Do <xdoeri00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    06 December 2024
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

bool TreeMeshBuilder::shouldPruneBlock(size_t size, const Vec3_t<float> &offset, const ParametricScalarField &field) {
    Vec3_t<float> center((offset.x + size / 2.0f) * mGridResolution, 
                         (offset.y + size / 2.0f) * mGridResolution, 
                         (offset.z + size / 2.0f) * mGridResolution);

    float fieldValue = evaluateFieldAt(center, field);
    return fieldValue > field.getIsoLevel() + (std::sqrt(3.0f) / 2.0f) * (size * mGridResolution);
}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field) {
    unsigned mTotalTriangles = 0;

    #pragma omp parallel 
    {
        #pragma omp master
        mTotalTriangles = marchCubesRecursive(mGridSize, Vec3_t<float>(0, 0, 0), field);
    }

    return mTotalTriangles;
}

unsigned TreeMeshBuilder::marchCubesRecursive(size_t size, Vec3_t<float> position, const ParametricScalarField &field)
{
    if (size <= 1) {
        return buildCube(position, field);
    }
    
    unsigned halfSize = size / 2;
    unsigned localTriangles = 0;

    for (int i = 0; i < 8; i++) {
        Vec3_t<float> childPos(
            position.x + (i & 1) * halfSize,
            position.y + ((i & 2) >> 1) * halfSize,
            position.z + ((i & 4) >> 2) * halfSize);

        if (shouldPruneBlock(halfSize, childPos, field)) {
            continue;
        }

        #pragma omp task shared(localTriangles) firstprivate(childPos, halfSize)
        {
            #pragma omp atomic
            localTriangles += marchCubesRecursive(halfSize, childPos, field);
        }
    }

    #pragma omp taskwait
    return localTriangles;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());
    float value = std::numeric_limits<float>::max();

    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        value = std::min(value, distanceSquared);
    }

    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    #pragma omp critical
    mTriangles.push_back(triangle);
}
