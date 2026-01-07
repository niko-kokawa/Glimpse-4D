#include "MeshData.h"
#include <filesystem>
#include <iostream>
#include<iterator>
#include<fstream>
#include <cstdint>
#include <vector>

#include <Python.h>

MeshData::MeshData() {

}

std::vector<MeshData> MeshData::load()
{
    std::filesystem::path directory =
        std::filesystem::current_path()
        / ".." / ".." / ".." / ".."
        / "Glimpse 4D" / "4D Objects";
    
    directory = std::filesystem::weakly_canonical(directory);
    convertData();

    std::vector<MeshData> meshes;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != ".bin")
            continue;

        meshes.push_back(SingleMeshLoad(entry.path()));
    }

    return meshes;

}

MeshData MeshData::SingleMeshLoad(const std::filesystem::path& filePath) {
    MeshData mesh;

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open " + filePath.string());
    }

    uint32_t vertexCount;
    file.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));

    mesh.vertices.resize(vertexCount);
    file.read(reinterpret_cast<char*>(mesh.vertices.data()),
        vertexCount * sizeof(Vec4));

    uint32_t edgeCount;
    file.read(reinterpret_cast<char*>(&edgeCount), sizeof(edgeCount));

    mesh.edges.resize(edgeCount);
    file.read(reinterpret_cast<char*>(mesh.edges.data()),
        edgeCount * sizeof(Edge));

    return mesh;
}

void MeshData::convertData()
{
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // Set Python home
    config.home = Py_DecodeLocale(
        "C:\\Users\\14374\\.pyenv\\pyenv-win\\versions\\3.13.2",
        nullptr
    );

    // Add search paths manually
    PyWideStringList_Append(
        &config.module_search_paths,
        L"C:\\Users\\14374\\.pyenv\\pyenv-win\\versions\\3.13.2\\python313.zip"
    );
    PyWideStringList_Append(
        &config.module_search_paths,
        L"C:\\Users\\14374\\.pyenv\\pyenv-win\\versions\\3.13.2\\DLLs"
    );
    PyWideStringList_Append(
        &config.module_search_paths,
        L"C:\\Users\\14374\\.pyenv\\pyenv-win\\versions\\3.13.2\\Lib"
    );
    PyWideStringList_Append(
        &config.module_search_paths,
        L"C:\\Users\\14374\\.pyenv\\pyenv-win\\versions\\3.13.2\\Lib\\site-packages"
    );

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }

    std::filesystem::path filePath =
        std::filesystem::current_path()
        / ".." / ".." / ".." / ".."
        / "Glimpse 4D"
        / "data conversion.py";

    filePath = std::filesystem::weakly_canonical(filePath);

    std::ifstream scriptFile(filePath);
    if (!scriptFile.is_open()) {
        std::cerr << "Failed to open script: " << filePath << "\n";
        return;
    }

    std::string script(
        (std::istreambuf_iterator<char>(scriptFile)),
        std::istreambuf_iterator<char>()
    );

    PyRun_SimpleString(script.c_str());
    Py_Finalize();
    PyConfig_Clear(&config);

}