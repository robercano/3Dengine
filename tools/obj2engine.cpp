#include "ModelInfo.hpp"
#include "ModelLoaders.hpp"
#include "ModelStorage.hpp"

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "OBJ model files to engine internal model file converter\n\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "    OBJ2Engine <input_obj> <output_engine>\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "input_obj: directory containing the geometry.obj, material.mtl and all textures files\n");
        fprintf(stderr, "output_engine: filename for the engine binary representation file\n");
        fprintf(stderr, "\n");
        exit(1);
    }

    Model3D model;

    if (ModelLoaders::LoadOBJModel(model, argv[1]) == false) {
        fprintf(stderr, "ERROR opening input OBJ model %s\n", argv[1]);
        exit(2);
    }

    ModelInfo::ShowModelInfo(model);

    if (ModelStorage::Save(argv[2], model) == false) {
        fprintf(stderr, "ERROR storing model to output file %s\n", argv[2]);
        exit(3);
    }

    fprintf(stderr, "\nCreated %s succesfully\n\n", argv[2]);

    return 0;
}
