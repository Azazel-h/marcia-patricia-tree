#include "merkle-patricia-tree/common/base.hpp"

uint64_t silkworm_to_kibi(uint64_t x) {
    return x * SILKWORM_KIBI;
}

uint64_t silkworm_to_mebi(uint64_t x) {
    return x * SILKWORM_MEBI;
}

uint64_t silkworm_to_gibi(uint64_t x) {
    return x * SILKWORM_GIBI;
}

uint64_t silkworm_to_tebi(uint64_t x) {
    return x * SILKWORM_TEBI;
}