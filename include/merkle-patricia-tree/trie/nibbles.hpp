#ifndef SILKWORM_NIBBLES_HPP_
#define SILKWORM_NIBBLES_HPP_

#include "merkle-patricia-tree/common/base.hpp"
#include "merkle-patricia-tree/common/bytes.hpp"

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

namespace silkworm::trie {

//! \brief Transforms a string of of Nibbles into a string of Bytes
//! \def A Nibble's value is [0..16)
//! \see Erigon's CompressNibbles
    Bytes pack_nibbles(ByteView unpacked);

//! \brief Transforms a string of of bytes into a string of Nibbles
//! \def A Nibble's value is [0..16)
//! \see Erigon's DecompressNibbles
    Bytes unpack_nibbles(ByteView data);

}  // namespace silkworm::trie

#endif // __cplusplus

// C interface
#ifdef __cplusplus
extern "C" {
#endif

// silkworm_ByteView and silkworm_Bytes are defined in bytes.hpp
silkworm_Bytes silkworm_pack_nibbles(const silkworm_ByteView* unpacked);
silkworm_Bytes silkworm_unpack_nibbles(const silkworm_ByteView* data);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_NIBBLES_HPP_
