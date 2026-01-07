#pragma once
#pragma once
#include <vector>
#include <array>
#include <string>
#include <Python.h>
#include<fstream>
#include "Space.h"

struct Edge {
    int32_t a, b;
};


class MeshData {
private:
    static void convertData();
    static MeshData SingleMeshLoad(const std::filesystem::path& filePath);

public:
    std::vector<Vec4> vertices;
    std::vector<Edge> edges;

    static std::vector<MeshData> load();
    MeshData();
};