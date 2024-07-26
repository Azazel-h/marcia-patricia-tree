#include "merkle-patricia-tree/trie/nibbles.hpp"

namespace silkworm::trie {

Bytes pack_nibbles(ByteView unpacked) {
    if (unpacked.empty()) {
        return {};
    }

    size_t pos{unpacked.length() & 1};
    Bytes out((unpacked.length() + pos) / 2, '\0');
    auto out_it{out.begin()};
    while (unpacked.size() > pos) {
        *out_it++ = static_cast<uint8_t>((unpacked[0] << 4) + unpacked[1]);
        unpacked.remove_prefix(2);
    }
    if (pos) {
        *out_it = static_cast<uint8_t>(unpacked[0] << 4);
        unpacked.remove_prefix(1);
    }

    return out;
}

Bytes unpack_nibbles(ByteView data) {
    Bytes out(2 * data.length(), '\0');
    size_t offset{0};
    for (const auto& b : data) {
        out[offset] = b >> 4;
        out[offset + 1] = b & 0x0F;
        offset += 2;
    }
    return out;
}

}  // namespace silkworm::trie

// C interface implementation
silkworm_Bytes silkworm_pack_nibbles(const silkworm_ByteView* unpacked) {
    if (unpacked == nullptr) {
        return silkworm_Bytes_create(0);
    }

    silkworm::ByteView cpp_unpacked(unpacked->data, unpacked->length);
    silkworm::Bytes cpp_result = silkworm::trie::pack_nibbles(cpp_unpacked);

    silkworm_Bytes result = silkworm_Bytes_create(cpp_result.size());
    silkworm_Bytes_append(&result, cpp_result.data(), cpp_result.size());

    return result;
}

silkworm_Bytes silkworm_unpack_nibbles(const silkworm_ByteView* data) {
    if (data == nullptr) {
        return silkworm_Bytes_create(0);
    }

    silkworm::ByteView cpp_data(data->data, data->length);
    silkworm::Bytes cpp_result = silkworm::trie::unpack_nibbles(cpp_data);

    silkworm_Bytes result = silkworm_Bytes_create(cpp_result.size());
    silkworm_Bytes_append(&result, cpp_result.data(), cpp_result.size());

    return result;
}

