#include <algorithm>
#include <cstring>
#include <merkle-patricia-tree/common/util.hpp>
#include <merkle-patricia-tree/rlp/encode.hpp>
#include <merkle-patricia-tree/types/evmc_bytes32.hpp>

namespace silkworm {

    evmc::bytes32 to_bytes32(ByteView bytes) {
        evmc::bytes32 out;
        if (!bytes.empty()) {
            size_t n{std::min(bytes.length(), kHashLength)};
            std::memcpy(out.bytes + kHashLength - n, bytes.data(), n);
        }
        return out;
    }

    std::string to_hex(const evmc::bytes32 &value, bool with_prefix) {
        return silkworm::to_hex(ByteView{value.bytes}, with_prefix);
    }

}  // namespace silkworm

namespace silkworm::rlp {

    void encode(Bytes &to, const evmc::bytes32 &value) {
        silkworm::rlp::encode(to, ByteView{value.bytes});
    }

    size_t length(const evmc::bytes32 &value) noexcept {
        return silkworm::rlp::length(ByteView{value.bytes});
    }

    DecodingResult decode(ByteView &from, evmc::bytes32 &to, Leftover mode) noexcept {
        return silkworm::rlp::decode(from, to.bytes, mode);
    }

}  // namespace silkworm::rlp
