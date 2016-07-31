#include "Asset3DLoaders.hpp"
#include "Asset3DStorage.hpp"
#include "Logging.hpp"

using namespace Logging;

int main(int argc, char **argv)
{
    if (argc < 3) {
        log("OBJ asset files to engine internal asset file converter\n\n");
        log("Usage:\n");
        log("    OBJ2Engine <input_obj> <output_engine>\n");
        log("\n");
        log("input_obj: directory containing the geometry.obj, material.mtl and all textures files\n");
        log("output_engine: filename for the engine binary representation file\n");
        log("\n");
        exit(1);
    }

    Asset3D *asset = Asset3D::New();

    if (Asset3DLoaders::LoadOBJ(*asset, argv[1]) == false) {
        log("ERROR opening input OBJ asset %s\n", argv[1]);
        exit(2);
    }

    log("Asset info", *asset);

    if (Asset3DStorage::Save(argv[2], *asset) == false) {
        log("ERROR storing asset to output file %s\n", argv[2]);
        exit(3);
    }

    log("\nCreated %s succesfully\n\n", argv[2]);

    Asset3D::Delete(asset);

    return 0;
}
