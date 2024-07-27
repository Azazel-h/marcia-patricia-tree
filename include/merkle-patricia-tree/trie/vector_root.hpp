#ifndef SILKWORM_TRIE_ROOT_HASH_HPP
#define SILKWORM_TRIE_ROOT_HASH_HPP

#include "merkle-patricia-tree/rlp/encode.hpp"
#include "hash_builder.hpp"
#include "nibbles.hpp"

#ifdef __cplusplus

#include <concepts>
#include <utility>

namespace silkworm::trie {

// Lexicographic order for RLP-encoded integers is the same as their natural order,
// save for 0, which, due to its RLP encoding, should be placed between 0x7f and 0x80.
    inline size_t adjust_index_for_rlp(size_t i, size_t len) {
        if (i > 0x7f) {
            return i;
        } else if (i == 0x7f || i + 1 == len) {
            return 0;
        } else {
            return i + 1;
        }
    }

// Trie root hash of RLP-encoded values, the keys are RLP-encoded integers.
// See Section 4.3.2. "Holistic Validity" of the Yellow Paper.
    template<class Value, std::invocable<Bytes &, const Value &> Encoder>
    evmc::bytes32 root_hash(const std::vector<Value> &v, Encoder &&value_encoder) {
        Bytes index_rlp;
        Bytes value_rlp;

        HashBuilder hb;

        for (size_t j{0}; j < v.size(); ++j) {
            const size_t index{adjust_index_for_rlp(j, v.size())};
            index_rlp.clear();
            rlp::encode(index_rlp, index);
            value_rlp.clear();
            std::forward<Encoder>(value_encoder)(value_rlp, v[index]);

            hb.add_leaf(unpack_nibbles(index_rlp), value_rlp);
        }

        return hb.root_hash();
    }

}  // namespace silkworm::trie
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Тип функции для кодирования значения
typedef void (*silkworm_ValueEncoder)(silkworm_Bytes *out, const silkworm_ByteView *value);

// Вычисление корневого хэша
void silkworm_trie_root_hash(uint8_t out_hash[32], const void *values, size_t value_count, size_t value_size,
                             silkworm_ValueEncoder encoder);

// Вспомогательная функция для adjust_index_for_rlp
size_t silkworm_trie_adjust_index_for_rlp(size_t i, size_t len);

#ifdef __cplusplus
}
#endif
#endif // SILKWORM_TRIE_ROOT_HASH_HPP