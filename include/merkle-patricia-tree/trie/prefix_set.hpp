#ifndef SILKWORM_TRIE_PREFIX_SET_HPP
#define SILKWORM_TRIE_PREFIX_SET_HPP

#include "merkle-patricia-tree/common/base.hpp"
#include "merkle-patricia-tree/common/bytes.hpp"

#ifdef __cplusplus

#include <vector>

namespace silkworm::trie {

/**
 * A set of "nibbled" byte strings with the following property:
 *  If x ∈ S and x starts with y, then y ∈ S.
 *  Corresponds to RetainList in Erigon.
 */
    class PrefixSet {
    public:
        //! \brief Constructs an empty set.
        PrefixSet() = default;

        // copyable
        PrefixSet(const PrefixSet &other) = default;

        PrefixSet &operator=(const PrefixSet &other) = default;

        void insert(ByteView key, bool marker = false);

        void insert(Bytes &&key, bool marker = false);

        //! \brief Returns whether or not provided prefix is contained in any of the owned keys
        //! \remarks Doesn't change the set logically, but is not marked const since it's not safe to call this method
        //! concurrently. \see Erigon's RetainList::Retain
        bool contains(ByteView prefix);

        //! \brief Returns the next key with marker==true in the list
        //! \see Erigon's RetainList::RetainWithMarker
        //! \param [in] prefix : the prefix to search for
        //! \param [in] invariant_prefix_len : when searching for next marked the scanned items must begin with this number
        //! of identical bytes
        std::pair<bool, ByteView> contains_and_next_marked(ByteView prefix, size_t invariant_prefix_len = 0);

        [[nodiscard]] size_t size() const { return keys_.size(); }

        [[nodiscard]] bool empty() const { return keys_.empty(); }

        void clear() noexcept {
            keys_.clear();
            index_ = 0;
            sorted_ = false;
        }

    private:
        void ensure_sorted();

        std::vector<std::pair<Bytes, bool>> keys_;  // Collection of nibbled keys with marker of newly created
        size_t index_{0};                           // Index of last compared key
        bool sorted_{false};                        // Whether nibbled_keys_ has been unique-ed and sorted
    };

}  // namespace silkworm::trie
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct silkworm_PrefixSet silkworm_PrefixSet;

// Создание и удаление PrefixSet
silkworm_PrefixSet *silkworm_PrefixSet_new();
void silkworm_PrefixSet_free(silkworm_PrefixSet *set);

// Вставка элементов
void silkworm_PrefixSet_insert(silkworm_PrefixSet *set, silkworm_ByteView *key, int marker);
void silkworm_PrefixSet_insert_move(silkworm_PrefixSet *set, silkworm_Bytes *key, int marker);

// Проверка наличия префикса
int silkworm_PrefixSet_contains(silkworm_PrefixSet *set, silkworm_ByteView *prefix);

// Поиск следующего помеченного элемента
typedef struct {
    int found;
    silkworm_ByteView next_marked;
} silkworm_PrefixSetNextMarked;

silkworm_PrefixSetNextMarked
silkworm_PrefixSet_contains_and_next_marked(silkworm_PrefixSet *set, silkworm_ByteView *prefix,
                                            size_t invariant_prefix_len);

// Получение размера и проверка на пустоту
size_t silkworm_PrefixSet_size(const silkworm_PrefixSet *set);
int silkworm_PrefixSet_empty(const silkworm_PrefixSet *set);

// Очистка множества
void silkworm_PrefixSet_clear(silkworm_PrefixSet *set);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_TRIE_PREFIX_SET_HPP