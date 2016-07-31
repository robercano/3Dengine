#include "Logging.hpp"
#include "Asset3DLoaders.hpp"
#include "Asset3DStorage.hpp"

using namespace Logging;

int main(int argc, char **argv)
{
    if (argc < 2) {
        log("Shows statistics about an internal asset\n\n");
        log("Usage:\n");
        log("    inspect-asset <input_asset>\n");
        log("\n");
        log("input_asset: internal asset path and name\n");
        log("\n");
        exit(1);
    }

    Asset3D *asset = Asset3D::New();

    if (Asset3DStorage::Load(argv[1], *asset) == false) {
        log("ERROR loading asset from file %s\n", argv[1]);
        exit(3);
    }

    log("\nAsset3D:     %s\n", argv[1]);
    log("Asset data", *asset);

    Asset3D::Delete(asset);

    return 0;
}
