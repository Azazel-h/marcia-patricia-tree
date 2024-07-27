/*
   Copyright 2022 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

// RLP encoding functions as per
// https://eth.wiki/en/fundamentals/rlp
#ifndef SILKWORM_rlp_ENCODE_HPP
#define SILKWORM_rlp_ENCODE_HPP

#include "merkle-patricia-tree/common/base.hpp"
#include "merkle-patricia-tree/common/bytes.hpp"
#include "merkle-patricia-tree/common/endian.hpp"

#ifdef __cplusplus

#include "intx/intx.hpp"


namespace silkworm::rlp {

    struct Header {
        bool list{false};
        size_t payload_length{0};
    };

    inline constexpr uint8_t kEmptyStringCode{0x80};
    inline constexpr uint8_t kEmptyListCode{0xC0};

    void encode_header(Bytes &to, Header header);

    void encode(Bytes &to, ByteView str);

    template<UnsignedIntegral T>
    void encode(Bytes &to, const T &n) {
        if (n == 0) {
            to.push_back(kEmptyStringCode);
        } else if (n < kEmptyStringCode) {
            to.push_back(static_cast<uint8_t>(n));
        } else {
            const ByteView be{endian::to_big_compact(n)};
            encode_header(to, {.list = false, .payload_length = be.length()});
            to.append(be);
        }
    }

    void encode(Bytes &to, bool);

    size_t length_of_length(uint64_t payload_length) noexcept;

    size_t length(ByteView) noexcept;

    template<UnsignedIntegral T>
    size_t length(const T &n) noexcept {
        if (n < kEmptyStringCode) {
            return 1;
        } else {
            const size_t n_bytes{intx::count_significant_bytes(n)};
            return n_bytes + length_of_length(n_bytes);
        }
    }

    inline size_t length(bool) noexcept {
        return 1;
    }

}  // namespace silkworm::rlp
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int list;
    uint64_t payload_length;
} silkworm_rlp_Header;

extern const uint8_t silkworm_rlp_kEmptyStringCode;
extern const uint8_t silkworm_rlp_kEmptyListCode;

void silkworm_rlp_encode_header(silkworm_Bytes *to, silkworm_rlp_Header header);
void silkworm_rlp_encode_bytes(silkworm_Bytes *to, silkworm_ByteView str);
void silkworm_rlp_encode_uint(silkworm_Bytes *to, uint64_t n);
void silkworm_rlp_encode_bool(silkworm_Bytes *to, int value);

size_t silkworm_rlp_length_of_length(uint64_t payload_length);
size_t silkworm_rlp_length_of_bytes(silkworm_ByteView bytes);
size_t silkworm_rlp_length_of_uint(uint64_t n);
size_t silkworm_rlp_length_of_bool(void);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_rlp_ENCODE_HPP
