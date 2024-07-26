#ifndef SILKWORM_DECODING_RESULT_HPP_
#define SILKWORM_DECODING_RESULT_HPP_

#ifdef __cplusplus

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

#include "tl/expected.hpp"

namespace silkworm {

// Error codes for RLP and other decoding
    enum class [[nodiscard]] DecodingError {
        kOverflow,
        kLeadingZero,
        kInputTooShort,
        kInputTooLong,
        kNonCanonicalSize,
        kUnexpectedLength,
        kUnexpectedString,
        kUnexpectedList,
        kUnexpectedListElements,
        kInvalidVInSignature,         // v != 27 && v != 28 && v < 35, see EIP-155
        kUnsupportedTransactionType,  // EIP-2718
        kInvalidFieldset,
        kUnexpectedEip2718Serialization,
        kInvalidHashesLength,  // trie::Node decoding
        kInvalidMasksSubsets,  // trie::Node decoding
    };

// TODO(C++23) Switch to std::expected
    using DecodingResult = tl::expected<void, DecodingError>;

}  // namespace silkworm

#endif // __cplusplus

// C interface
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SILKWORM_DECODING_ERROR_OVERFLOW,
    SILKWORM_DECODING_ERROR_LEADING_ZERO,
    SILKWORM_DECODING_ERROR_INPUT_TOO_SHORT,
    SILKWORM_DECODING_ERROR_INPUT_TOO_LONG,
    SILKWORM_DECODING_ERROR_NON_CANONICAL_SIZE,
    SILKWORM_DECODING_ERROR_UNEXPECTED_LENGTH,
    SILKWORM_DECODING_ERROR_UNEXPECTED_STRING,
    SILKWORM_DECODING_ERROR_UNEXPECTED_LIST,
    SILKWORM_DECODING_ERROR_UNEXPECTED_LIST_ELEMENTS,
    SILKWORM_DECODING_ERROR_INVALID_V_IN_SIGNATURE,
    SILKWORM_DECODING_ERROR_UNSUPPORTED_TRANSACTION_TYPE,
    SILKWORM_DECODING_ERROR_INVALID_FIELDSET,
    SILKWORM_DECODING_ERROR_UNEXPECTED_EIP2718_SERIALIZATION,
    SILKWORM_DECODING_ERROR_INVALID_HASHES_LENGTH,
    SILKWORM_DECODING_ERROR_INVALID_MASKS_SUBSETS
} silkworm_DecodingError;

typedef struct {
    int has_error;
    silkworm_DecodingError error;
} silkworm_DecodingResult;

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_DECODING_RESULT_HPP_