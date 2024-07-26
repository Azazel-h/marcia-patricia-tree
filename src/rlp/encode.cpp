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

#include "merkle-patricia-tree/rpl/encode.hpp"

namespace silkworm::rlp {

void encode_header(Bytes& to, Header header) {
    if (header.payload_length < 56) {
        const uint8_t code{header.list ? kEmptyListCode : kEmptyStringCode};
        to.push_back(static_cast<uint8_t>(code + header.payload_length));
    } else {
        auto len_be{endian::to_big_compact(header.payload_length)};
        const uint8_t code = header.list ? 0xF7 : 0xB7;
        to.push_back(static_cast<uint8_t>(code + len_be.length()));
        to.append(len_be);
    }
}

size_t length_of_length(uint64_t payload_length) noexcept {
    if (payload_length < 56) {
        return 1;
    } else {
        return 1 + intx::count_significant_bytes(payload_length);
    }
}

void encode(Bytes& to, bool x) {
    to.push_back(x ? uint8_t{1} : kEmptyStringCode);
}

void encode(Bytes& to, ByteView s) {
    if (s.length() != 1 || s[0] >= kEmptyStringCode) {
        encode_header(to, {.list = false, .payload_length = s.length()});
    }
    to.append(s);
}

size_t length(ByteView s) noexcept {
    size_t len{s.length()};
    if (s.length() != 1 || s[0] >= kEmptyStringCode) {
        len += length_of_length(s.length());
    }
    return len;
}

}  // namespace silkworm::rlp

// rlp_encode_wrapper.cpp

const uint8_t silkworm_rlp_kEmptyStringCode = silkworm::rlp::kEmptyStringCode;
const uint8_t silkworm_rlp_kEmptyListCode = silkworm::rlp::kEmptyListCode;

void silkworm_rlp_encode_header(silkworm_Bytes* to, silkworm_rlp_Header header) {
    silkworm::Bytes cpp_to(to->data, to->length);
    silkworm::rlp::Header cpp_header{header.list, header.payload_length};
    silkworm::rlp::encode_header(cpp_to, cpp_header);
    to->length = cpp_to.length();
}

void silkworm_rlp_encode_bytes(silkworm_Bytes* to, silkworm_ByteView str) {
    silkworm::Bytes cpp_to(to->data, to->length);
    silkworm::ByteView cpp_str(str.data, str.length);
    silkworm::rlp::encode(cpp_to, cpp_str);
    to->length = cpp_to.length();
}

void silkworm_rlp_encode_uint(silkworm_Bytes* to, uint64_t n) {
    silkworm::Bytes cpp_to(to->data, to->length);
    silkworm::rlp::encode(cpp_to, n);
    to->length = cpp_to.length();
}

void silkworm_rlp_encode_bool(silkworm_Bytes* to, int value) {
    silkworm::Bytes cpp_to(to->data, to->length);
    silkworm::rlp::encode(cpp_to, value != 0);
    to->length = cpp_to.length();
}

size_t silkworm_rlp_length_of_length(uint64_t payload_length) {
    return silkworm::rlp::length_of_length(payload_length);
}

size_t silkworm_rlp_length_of_bytes(silkworm_ByteView bytes) {
    silkworm::ByteView cpp_bytes(bytes.data, bytes.length);
    return silkworm::rlp::length(cpp_bytes);
}

size_t silkworm_rlp_length_of_uint(uint64_t n) {
    return silkworm::rlp::length(n);
}

size_t silkworm_rlp_length_of_bool(void) {
    return silkworm::rlp::length(true);
}