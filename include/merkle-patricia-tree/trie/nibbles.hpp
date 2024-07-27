#ifndef SILKWORM_NIBBLES_HPP
#define SILKWORM_NIBBLES_HPP

#include "merkle-patricia-tree/common/base.hpp"
#include "merkle-patricia-tree/common/bytes.hpp"

#ifdef __cplusplus

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
silkworm_Bytes silkworm_pack_nibbles(const silkworm_ByteView *unpacked);
silkworm_Bytes silkworm_unpack_nibbles(const silkworm_ByteView *data);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_NIBBLES_HPP
