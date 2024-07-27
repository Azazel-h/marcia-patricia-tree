#include "merkle-patricia-tree/trie/vector_root.hpp"


void silkworm_trie_root_hash(uint8_t out_hash[32], const void *values, size_t value_count, size_t value_size,
                             silkworm_ValueEncoder encoder) {
    std::vector<silkworm::Bytes> cpp_values;
    cpp_values.reserve(value_count);

    const uint8_t *byte_values = static_cast<const uint8_t *>(values);
    for (size_t i = 0; i < value_count; ++i) {
        cpp_values.emplace_back(byte_values + i * value_size, byte_values + (i + 1) * value_size);
    }

    auto cpp_encoder = [encoder](silkworm::Bytes &out, const silkworm::Bytes &value) {
        silkworm_Bytes c_out{out.data(), out.size(), out.capacity()};
        silkworm_ByteView c_value{value.data(), value.size()};
        encoder(&c_out, &c_value);
        out.resize(c_out.length);
    };

    evmc::bytes32 result = silkworm::trie::root_hash(cpp_values, cpp_encoder);
    std::memcpy(out_hash, result.bytes, 32);
}

size_t silkworm_trie_adjust_index_for_rlp(size_t i, size_t len) {
    return silkworm::trie::adjust_index_for_rlp(i, len);
}


