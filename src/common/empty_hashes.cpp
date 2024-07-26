#include "merkle-patricia-tree/common/empty_hashes.hpp"

silkworm_bytes32 silkworm_get_empty_hash() {
    silkworm_bytes32 result;
    std::memcpy(result.bytes, silkworm::kEmptyHash.bytes, 32);
    return result;
}

silkworm_bytes32 silkworm_get_empty_list_hash() {
    silkworm_bytes32 result;
    std::memcpy(result.bytes, silkworm::kEmptyListHash.bytes, 32);
    return result;
}

silkworm_bytes32 silkworm_get_empty_root() {
    silkworm_bytes32 result;
    std::memcpy(result.bytes, silkworm::kEmptyRoot.bytes, 32);
    return result;
}
