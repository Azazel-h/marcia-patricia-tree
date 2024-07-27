#ifndef SILKWORM_TYPES_EVMC_BYTES32_HPP
#define SILKWORM_TYPES_EVMC_BYTES32_HPP

#include <merkle-patricia-tree/common/bytes.hpp>
#include <merkle-patricia-tree/rlp/decode.hpp>

#ifdef __cplusplus

#include <string>
#include <evmc/evmc.hpp>


namespace silkworm {

// Converts bytes to evmc::bytes32; input is cropped if necessary.
// Short inputs are left-padded with 0s.
    evmc::bytes32 to_bytes32(ByteView bytes);

    std::string to_hex(const evmc::bytes32 &value, bool with_prefix = false);

}  // namespace silkworm

namespace silkworm::rlp {

    void encode(Bytes &to, const evmc::bytes32 &value);

    size_t length(const evmc::bytes32 &value) noexcept;

    DecodingResult decode(ByteView &from, evmc::bytes32 &to, Leftover mode = Leftover::kProhibit) noexcept;

}  // namespace silkworm::rlp

namespace evmc {
    using silkworm::rlp::encode;
    using silkworm::rlp::length;
}  // namespace evmc
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Convert bytes to evmc_bytes32
evmc_bytes32 evmc_to_bytes32(const uint8_t* bytes, size_t length);

// Convert evmc_bytes32 to hex string
// Returns a newly allocated string that must be freed by the caller
char* evmc_to_hex(const evmc_bytes32* value, bool with_prefix);

// RLP encode evmc_bytes32
// Returns the length of the encoded data
size_t evmc_rlp_encode(const evmc_bytes32* value, uint8_t* output, size_t output_size);

// Get the length of RLP encoded evmc_bytes32
size_t evmc_rlp_length(const evmc_bytes32* value);

// RLP decode to evmc_bytes32
// Returns 1 on success, 0 on failure
int evmc_rlp_decode(const uint8_t* input, size_t input_size, evmc_bytes32* output);


#ifdef __cplusplus
}
#endif

#endif // SILKWORM_TYPES_EVMC_BYTES32_HPP