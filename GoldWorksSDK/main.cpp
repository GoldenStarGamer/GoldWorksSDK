#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

// Structure representing the header of a GoldWorksShader file.
struct ShaderHeader {
    uint32_t version = 0;           // Version for compatibility
    uint32_t vertexSize = 0;        // Size of the vertex code
    uint32_t fragmentSize = 0;      // Size of the fragment code
    uint32_t vertexLocation = 0;    // Location of the vertex code
    uint32_t fragmentLocation = 0;  // Location of the fragment code
};

int main(int argc, char* argv[]) {

    // Check command-line arguments
    if (argc < 3 || argc > 4) {
        cout << "Invalid Arguments" << endl
            << "Format: GWShaderMaker.exe <compiled vertex shader file> <compiled fragment shader file> <output file>(optional)";
        return 1;
    }

    string name; // Name of the file

    // Set the output file name
    if (argc == 3) {
        name = "shader.gws"; // Default name
    }
    else if (argc == 4) {
        name = argv[3];
        name.erase(remove(name.begin(), name.end(), '"'), name.end()); // Remove double quotes
    }

    vector<char> vertexCode;   // Vector representing the Vertex Shader code
    vector<char> fragmentCode; // Vector representing the Fragment Shader code

    // Read Vertex Shader code
    ifstream vertexShaderFile(argv[1], ios::binary | ios::ate);

    if (!vertexShaderFile.is_open()) {
        cout << "Failed to open vertex shader file." << endl;
        return 1;
    }

    // Get the Vertex Code Size
    size_t vertexSize = vertexShaderFile.tellg();

    // Resize Vector to fit Vertex Shader Code
    vertexCode.resize(vertexSize);

    // Go to beggining of file
    vertexShaderFile.seekg(0, ios::beg);

    // Copy contents to Vector
    vertexShaderFile.read(vertexCode.data(), vertexSize);

    // Close File
    vertexShaderFile.close();

    // Do the same for Fragment Shader
    ifstream fragmentShaderFile(argv[2], ios::binary | ios::ate);

    if (!fragmentShaderFile.is_open()) {
        cout << "Failed to open fragment shader file." << endl;
        return 1;
    }

    
    size_t fragmentSize = fragmentShaderFile.tellg();

    fragmentCode.resize(fragmentSize);

    fragmentShaderFile.seekg(0, ios::beg);

    fragmentShaderFile.read(fragmentCode.data(), fragmentSize);

    fragmentShaderFile.close();

    

    ShaderHeader buffer; // Structure of GoldWorksShader file


    // Set info that we already gathered

    // Set sizes
    buffer.vertexSize = static_cast<uint32_t>(vertexSize);
    buffer.fragmentSize = static_cast<uint32_t>(fragmentSize);

    // Set locations
    buffer.vertexLocation = sizeof(ShaderHeader);
    buffer.fragmentLocation = buffer.vertexLocation + buffer.vertexSize;



    // Create output file
    ofstream outputFile(name, ios::binary | ios::trunc);

    if (!outputFile.is_open()) {
        cout << "Failed to create/open output file." << endl;
        return 1;
    }

    // Write Header
    outputFile.write(reinterpret_cast<const char*>(&buffer), sizeof(ShaderHeader));

    // Write the Shader code
    outputFile.write(vertexCode.data(), vertexCode.size());
    outputFile.write(fragmentCode.data(), fragmentCode.size());

    // Close the file
    outputFile.close();

    cout << "Output file created successfully: " << name << endl;

    return 0; // finish
}

// The End