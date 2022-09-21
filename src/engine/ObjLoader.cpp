#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <map>

void ObjLoader::load(const std::string& filename) {
    m_numVertices = 0;
    m_vertices.clear();
    m_positions.clear();
    m_texCoords.clear();
    m_normals.clear();

    m_numIndices = 0;
    m_indices.clear();

    std::fstream file;
    file.open(filename, std::fstream::in);
    if (!file.good()) {
        std::cerr << "ERR: failed to load file: " << filename << std::endl;
        return;
    }

    struct VertexIndices {
        unsigned int position;
        unsigned int texCoord;
        unsigned int normal;

        bool operator<(const VertexIndices& rhs) const {
            return std::tie(position, texCoord, normal) < std::tie(rhs.position, rhs.texCoord, rhs.normal);
        }
    };

    std::string line;
    std::map<VertexIndices, unsigned int> indexMap;
    while (!file.eof()) {
        // load next line into line
        std::getline(file, line);

        // clear any comments
        std::size_t hashPos = line.find('#');
        if (hashPos != std::string::npos) {
            line.erase(hashPos);
        }

        // clear trailing whitespace
        std::size_t trimPos = line.find_last_not_of(" \n");
        if (trimPos != std::string::npos) {
            line.erase(trimPos + 1);
        }
        else {
            line.clear();
        }

        // skip empty lines
        if (line.empty()) continue;

        std::size_t spacePos = line.find(' ');
        std::string type = line.substr(0, spacePos);
        //std::cout << type << " ";
        std::size_t startPos;
        if (type == "v") {
            Position position = {0.0f, 0.0f, 0.0f, 1.0f};

            // parse x coord
            startPos = spacePos;
            position.x = std::stof(line.substr(startPos), &spacePos);

            // parse y coord
            startPos += spacePos;
            position.y = std::stof(line.substr(startPos), &spacePos);

            // parse z coord
            startPos += spacePos;
            position.z = std::stof(line.substr(startPos), &spacePos);

            // parse (optional) w coord
            startPos += spacePos;
            if (startPos < line.length()) {
                position.w = std::stof(line.substr(startPos), &spacePos);
            }

            //std::cout << position.x << " " << position.y << " " << position.z << " " << std::endl;
            m_positions.push_back(position);
        } else if (type == "vt") {
            TexCoord texCoord = {0.0f, 0.0f, 0.0f};

            // parse u coord
            startPos = spacePos;
            texCoord.u = std::stof(line.substr(startPos), &spacePos);

            // parse (optional) v coord
            startPos += spacePos;
            if (startPos < line.length()) {
                texCoord.v = std::stof(line.substr(startPos), &spacePos);
            }

            // parse (optional) w coord
            startPos += spacePos;
            if (startPos < line.length()) {
                texCoord.w = std::stof(line.substr(startPos), &spacePos);
            }

            //std::cout << texCoord.u << " " << texCoord.v << " " << std::endl;
            m_texCoords.push_back(texCoord);
        } else if (type == "vn") {
            Normal normal = {0.0f, 0.0f, 0.0f};

            // parse x coord
            startPos = spacePos;
            normal.x = std::stof(line.substr(startPos), &spacePos);

            // parse y coord
            startPos += spacePos;
            normal.y = std::stof(line.substr(startPos), &spacePos);

            // parse z coord
            startPos += spacePos;
            normal.z = std::stof(line.substr(startPos), &spacePos);

            //std::cout << normal.x << " " << normal.y << " " << normal.z << " " << std::endl;
            m_normals.push_back(normal);
        } else if (type == "f") {
            std::vector<VertexIndices> vertices;

            auto parseVertex = [](const std::string& string) -> VertexIndices {
                VertexIndices vertex = {0, 0, 0};

                size_t startPos = 0;
                size_t spacePos = string.find('/');
                vertex.position = std::stoi(string.substr(startPos, spacePos - startPos));

                startPos = spacePos + 1;
                spacePos = string.find('/', startPos);
                vertex.texCoord = std::stoi(string.substr(startPos, spacePos - startPos));

                startPos = spacePos + 1;
                spacePos = string.find('/', startPos);
                vertex.normal = std::stoi(string.substr(startPos, spacePos - startPos));

                return vertex;
            };

            // extract all face vertices
            startPos = spacePos + 1; // index of start of numbers
            spacePos = line.find(' ', startPos);
            while (spacePos != std::string::npos) {
                std::string vertexString = line.substr(startPos, spacePos - startPos);
                vertices.push_back(parseVertex(vertexString));

                startPos = spacePos + 1;
                spacePos = line.find(' ', startPos);
            }
            { // run the last iteration
                std::string vertexString = line.substr(startPos, std::string::npos);
                vertices.push_back(parseVertex(vertexString));
            }

            // assemble triangles
            if (vertices.size() >= 3) {
                for (int i: {0, 1, 2}) {
                    unsigned int index;
                    VertexIndices vertexIndices = vertices[i];
                    if (indexMap.contains(vertexIndices)) {
                        // lookup index for vertex indices
                        index = indexMap[vertexIndices];
                        m_indices.push_back(index);
                    } else {
                        // create vertex using indices
                        Vertex vertex = {
                                m_positions[vertexIndices.position - 1],
                                m_texCoords[vertexIndices.texCoord - 1],
                                m_normals[vertexIndices.normal - 1],
                        };
                        m_vertices.push_back(vertex);

                        // associate index with vertex indices
                        index = m_numVertices;
                        m_numVertices++;
                        indexMap[vertexIndices] = index;
                        m_indices.push_back(index);
                    }
                }
            }
            if (vertices.size() == 4) {
                for (int i: {2, 3, 0}) {
                    unsigned int index;
                    VertexIndices vertexIndices = vertices[i];
                    if (indexMap.contains(vertexIndices)) {
                        // lookup index for vertex indices
                        index = indexMap[vertexIndices];
                        m_indices.push_back(index);
                    } else {
                        // create vertex using indices
                        Vertex vertex = {
                                m_positions[vertexIndices.position - 1],
                                m_texCoords[vertexIndices.texCoord - 1],
                                m_normals[vertexIndices.normal - 1],
                        };
                        m_vertices.push_back(vertex);

                        // associate index with vertex indices
                        index = m_numVertices;
                        m_numVertices++;
                        indexMap[vertexIndices] = index;
                        m_indices.push_back(index);
                    }
                }
            }
        } else if (type == "usemtl") {
            std::cout << "skipping mats for now" << std::endl;
        } else if (type == "mtllib") {
            std::cout << "skipping mats for now" << std::endl;
        }
    }
}

const std::vector<ObjLoader::Vertex>& ObjLoader::getVertices() {
    return m_vertices;
}

const std::vector<unsigned int>& ObjLoader::getIndices() {
    return m_indices;
}

