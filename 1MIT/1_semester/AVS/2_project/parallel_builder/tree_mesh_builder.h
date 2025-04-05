/**
 * @file    tree_mesh_builder.h
 *
 * @author  Erika Do <xdoeri00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    06 December 2024
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "base_mesh_builder.h"

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    bool shouldPruneBlock(size_t size, const Vec3_t<float> &blockCenter, const ParametricScalarField &field);
    unsigned marchCubesRecursive(size_t size, Vec3_t<float> position, const ParametricScalarField &field);
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }

    std::vector<Triangle_t> mTriangles;
};

#endif // TREE_MESH_BUILDER_H
