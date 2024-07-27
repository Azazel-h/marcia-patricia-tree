#ifndef SILKWORM_rlp_DECODE_HPP
#define SILKWORM_rlp_DECODE_HPP

#include "merkle-patricia-tree/common/base.hpp"
#include "merkle-patricia-tree/common/bytes.hpp"
#include "merkle-patricia-tree/common/decoding_result.hpp"
#include "encode.hpp"

#ifdef __cplusplus

#include <array>
#include <cstring>
#include <span>

#include "intx/intx.hpp"

namespace silkworm::rlp {

// Whether to allow or prohibit trailing characters in an input after decoding.
// If prohibited and the input does contain extra characters, decode() returns DecodingResult::kInputTooLong.
    enum class Leftover {
        kProhibit,
        kAllow,
    };

// Consumes an RLP header unless it's a single byte in the [0x00, 0x7f] range,
// in which case the byte is put back.
    [[nodiscard]] tl::expected<Header, DecodingError> decode_header(ByteView &from) noexcept;

    DecodingResult decode(ByteView &from, Bytes &to, Leftover mode = Leftover::kProhibit) noexcept;

    template<UnsignedIntegral T>
    DecodingResult decode(ByteView &from, T &to, Leftover mode = Leftover::kProhibit) noexcept {
        const auto h{decode_header(from)};
        if (!h) {
            return tl::unexpected{h.error()};
        }
        if (h->list) {
            return tl::unexpected{DecodingError::kUnexpectedList};
        }
        if (DecodingResult res{endian::from_big_compact(from.substr(0, h->payload_length), to)}; !res) {
            return res;
        }
        from.remove_prefix(h->payload_length);
        if (mode != Leftover::kAllow && !from.empty()) {
            return tl::unexpected{DecodingError::kInputTooLong};
        }
        return {};
    }

    DecodingResult decode(ByteView &from, bool &to, Leftover mode = Leftover::kProhibit) noexcept;

    template<size_t N>
    DecodingResult decode(ByteView &from, std::span<uint8_t, N> to, Leftover mode = Leftover::kProhibit) noexcept {
        static_assert(N != std::dynamic_extent);

        const auto h{decode_header(from)};
        if (!h) {
            return tl::unexpected{h.error()};
        }
        if (h->list) {
            return tl::unexpected{DecodingError::kUnexpectedList};
        }
        if (h->payload_length != N) {
            return tl::unexpected{DecodingError::kUnexpectedLength};
        }

        std::memcpy(to.data(), from.data(), N);
        from.remove_prefix(N);
        if (mode != Leftover::kAllow && !from.empty()) {
            return tl::unexpected{DecodingError::kInputTooLong};
        }
        return {};
    }

    template<size_t N>
    DecodingResult decode(ByteView &from, uint8_t (&to)[N], Leftover mode = Leftover::kProhibit) noexcept {
        return decode<N>(from, std::span<uint8_t, N>{to}, mode);
    }

    template<size_t N>
    DecodingResult decode(ByteView &from, std::array<uint8_t, N> &to, Leftover mode = Leftover::kProhibit) noexcept {
        return decode<N>(from, std::span<uint8_t, N>{to}, mode);
    }

}  // namespace silkworm::rlp
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    SILKWORM_RLP_LEFTOVER_PROHIBIT,
    SILKWORM_RLP_LEFTOVER_ALLOW
} silkworm_rlp_Leftover;

// C-compatible representation of DecodingResult
typedef struct {
    int success;
    silkworm_DecodingError error;
} silkworm_rlp_DecodingResult;

// Function declarations for C interface

silkworm_rlp_DecodingResult silkworm_rlp_decode_header(silkworm_ByteView *from, silkworm_rlp_Header *header);
silkworm_rlp_DecodingResult
silkworm_rlp_decode_bytes(silkworm_ByteView *from, silkworm_Bytes *to, silkworm_rlp_Leftover mode);
silkworm_rlp_DecodingResult
silkworm_rlp_decode_uint64(silkworm_ByteView *from, uint64_t *to, silkworm_rlp_Leftover mode);
silkworm_rlp_DecodingResult silkworm_rlp_decode_bool(silkworm_ByteView *from, bool *to, silkworm_rlp_Leftover mode);
silkworm_rlp_DecodingResult
silkworm_rlp_decode_fixed_bytes(silkworm_ByteView *from, uint8_t *to, size_t length, silkworm_rlp_Leftover mode);

#ifdef __cplusplus
}
#endif
#endif // SILKWORM_rlp_DECODE_HPP
