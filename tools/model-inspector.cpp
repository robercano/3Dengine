#include "ModelInfo.hpp"
#include "ModelLoaders.hpp"
#include "ModelStorage.hpp"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Shows statistics about an internal model\n\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "    inspect-model <input_model>\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "input_model: internal model path and name\n");
        fprintf(stderr, "\n");
        exit(1);
    }

    Model3D model;

    if (ModelStorage::Load(argv[1], model) == false) {
        fprintf(stderr, "ERROR loading model from file %s\n", argv[1]);
        exit(3);
    }

    fprintf(stderr, "\nModel:     %s\n", argv[1]);
    ModelInfo::ShowModelInfo(model);

    return 0;
}
