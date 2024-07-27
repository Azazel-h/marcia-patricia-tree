#include <cassert>
#include <tuple>

#include "merkle-patricia-tree/rlp/decode.hpp"
#include "merkle-patricia-tree/common/endian.hpp"


namespace silkworm::rlp {

    tl::expected<Header, DecodingError> decode_header(ByteView &from) noexcept {
        if (from.empty()) {
            return tl::unexpected{DecodingError::kInputTooShort};
        }

        Header h{.list = false};
        uint8_t b{from[0]};
        if (b < 0x80) {
            h.payload_length = 1;
        } else if (b < 0xB8) {
            from.remove_prefix(1);
            h.payload_length = b - 0x80u;
            if (h.payload_length == 1) {
                if (from.empty()) {
                    return tl::unexpected{DecodingError::kInputTooShort};
                }
                if (from[0] < 0x80) {
                    return tl::unexpected{DecodingError::kNonCanonicalSize};
                }
            }
        } else if (b < 0xC0) {
            from.remove_prefix(1);
            const size_t len_of_len{b - 0xB7u};
            if (from.length() < len_of_len) {
                return tl::unexpected{DecodingError::kInputTooShort};
            }
            uint64_t len{0};
            if (DecodingResult res{endian::from_big_compact(from.substr(0, len_of_len), len)}; !res) {
                return tl::unexpected{res.error()};
            }
            h.payload_length = static_cast<size_t>(len);
            from.remove_prefix(len_of_len);
            if (h.payload_length < 56) {
                return tl::unexpected{DecodingError::kNonCanonicalSize};
            }
        } else if (b < 0xF8) {
            from.remove_prefix(1);
            h.list = true;
            h.payload_length = b - 0xC0u;
        } else {
            from.remove_prefix(1);
            h.list = true;
            const size_t len_of_len{b - 0xF7u};
            if (from.length() < len_of_len) {
                return tl::unexpected{DecodingError::kInputTooShort};
            }
            uint64_t len{0};
            if (DecodingResult res{endian::from_big_compact(from.substr(0, len_of_len), len)}; !res) {
                return tl::unexpected{res.error()};
            }
            h.payload_length = static_cast<size_t>(len);
            from.remove_prefix(len_of_len);
            if (h.payload_length < 56) {
                return tl::unexpected{DecodingError::kNonCanonicalSize};
            }
        }

        if (from.length() < h.payload_length) {
            return tl::unexpected{DecodingError::kInputTooShort};
        }

        return h;
    }

    DecodingResult decode(ByteView &from, Bytes &to, Leftover mode) noexcept {
        const auto h{decode_header(from)};
        if (!h) {
            return tl::unexpected{h.error()};
        }
        if (h->list) {
            return tl::unexpected{DecodingError::kUnexpectedList};
        }
        to = from.substr(0, h->payload_length);
        from.remove_prefix(h->payload_length);
        if (mode != Leftover::kAllow && !from.empty()) {
            return tl::unexpected{DecodingError::kInputTooLong};
        }
        return {};
    }

    DecodingResult decode(ByteView &from, bool &to, Leftover mode) noexcept {
        uint64_t i{0};
        if (DecodingResult res{decode(from, i, mode)}; !res) {
            return tl::unexpected{res.error()};
        }
        if (i > 1) {
            return tl::unexpected{DecodingError::kOverflow};
        }
        to = i;
        return {};
    }

}  // namespace silkworm::rlp

silkworm_rlp_DecodingResult silkworm_rlp_decode_header(silkworm_ByteView *from, silkworm_rlp_Header *header) {
    silkworm::ByteView cpp_from{from->data, from->length};
    auto result = silkworm::rlp::decode_header(cpp_from);
    if (result) {
        header->list = result->list;
        header->payload_length = result->payload_length;
        from->data = cpp_from.data();
        from->length = cpp_from.length();
        return {1, SILKWORM_DECODING_ERROR_OVERFLOW}; // Success, error value doesn't matter
    } else {
        return {0, static_cast<silkworm_DecodingError>(result.error())};
    }
}

silkworm_rlp_DecodingResult
silkworm_rlp_decode_bytes(silkworm_ByteView *from, silkworm_Bytes *to, silkworm_rlp_Leftover mode) {
    silkworm::ByteView cpp_from{from->data, from->length};
    silkworm::Bytes cpp_to;
    auto result = silkworm::rlp::decode(cpp_from, cpp_to, static_cast<silkworm::rlp::Leftover>(mode));
    if (result) {
        to->data = new uint8_t[cpp_to.size()];
        to->length = cpp_to.size();
        to->capacity = cpp_to.size();
        std::memcpy(to->data, cpp_to.data(), cpp_to.size());
        from->data = cpp_from.data();
        from->length = cpp_from.length();
        return {1, SILKWORM_DECODING_ERROR_OVERFLOW}; // Success, error value doesn't matter
    } else {
        return {0, static_cast<silkworm_DecodingError>(result.error())};
    }
}

silkworm_rlp_DecodingResult
silkworm_rlp_decode_uint64(silkworm_ByteView *from, uint64_t *to, silkworm_rlp_Leftover mode) {
    silkworm::ByteView cpp_from{from->data, from->length};
    auto result = silkworm::rlp::decode(cpp_from, *to, static_cast<silkworm::rlp::Leftover>(mode));
    if (result) {
        from->data = cpp_from.data();
        from->length = cpp_from.length();
        return {1, SILKWORM_DECODING_ERROR_OVERFLOW}; // Success, error value doesn't matter
    } else {
        return {0, static_cast<silkworm_DecodingError>(result.error())};
    }
}

silkworm_rlp_DecodingResult silkworm_rlp_decode_bool(silkworm_ByteView *from, bool *to, silkworm_rlp_Leftover mode) {
    silkworm::ByteView cpp_from{from->data, from->length};
    auto result = silkworm::rlp::decode(cpp_from, *to, static_cast<silkworm::rlp::Leftover>(mode));
    if (result) {
        from->data = cpp_from.data();
        from->length = cpp_from.length();
        return {1, SILKWORM_DECODING_ERROR_OVERFLOW}; // Success, error value doesn't matter
    } else {
        return {0, static_cast<silkworm_DecodingError>(result.error())};
    }
}

// TODO: FIX DECODING STATIC ASSERT ERROR!!!
//silkworm_rlp_DecodingResult
//silkworm_rlp_decode_fixed_bytes(silkworm_ByteView *from, uint8_t *to, size_t length, silkworm_rlp_Leftover mode) {
//    silkworm::ByteView cpp_from{from->data, from->length};
//    std::span<uint8_t, std::dynamic_extent> cpp_to{to, length};
//    auto result = silkworm::rlp::decode(cpp_from, cpp_to, static_cast<silkworm::rlp::Leftover>(mode));
//    if (result) {
//        from->data = cpp_from.data();
//        from->length = cpp_from.length();
//        return {1, SILKWORM_DECODING_ERROR_OVERFLOW}; // Success, error value doesn't matter
//    } else {
//        return {0, static_cast<silkworm_DecodingError>(result.error())};
//    }
//}
