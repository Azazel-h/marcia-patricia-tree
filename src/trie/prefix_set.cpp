#include "merkle-patricia-tree/trie/prefix_set.hpp"

#include <algorithm>
#include <utility>

namespace silkworm::trie {

    void PrefixSet::insert(ByteView key, bool marker) { insert(Bytes(key), marker); }

    void PrefixSet::insert(Bytes &&key, bool marker) {
        keys_.emplace_back(std::move(key), marker);
        sorted_ = false;
    }

    bool PrefixSet::contains(ByteView prefix) {
        if (keys_.empty()) {
            return false;
        }

        // Key uniqueness and sorting
        ensure_sorted();

        // We optimize for the case when contains() queries are issued with increasing prefixes,
        // e.g. contains("00"), contains("04"), contains("0b"), contains("0b05"), contains("0c"), contains("0f"), ...
        // instead of some random order.
        while (index_ > 0 && keys_[index_].first > prefix) {
            --index_;
        }

        for (size_t max_index{keys_.size() - 1};; ++index_) {
            const auto &[key, _]{keys_[index_]};
            if (key.starts_with(prefix)) {
                return true;
            }
            if (key > prefix || index_ == max_index) {
                return false;
            }
        }
    }

    std::pair<bool, ByteView> PrefixSet::contains_and_next_marked(ByteView prefix, size_t invariant_prefix_len) {
        bool is_contained{contains(prefix)};
        ByteView next_created{};

        invariant_prefix_len = std::min(invariant_prefix_len, prefix.size());

        // Lookup next marked created key
        for (size_t i{index_}, e{keys_.size()}; i < e; ++i) {
            auto &item{keys_[i]};

            // Check we're in the same invariant part of the prefix
            if (invariant_prefix_len && std::memcmp(&prefix[0], &item.first[0], invariant_prefix_len) != 0) {
                break;
            }

            if (item.second) {
                next_created = ByteView(item.first);
                break;
            }
        }

        return {is_contained, next_created};
    }

    void PrefixSet::ensure_sorted() {
        if (!sorted_) {
            std::sort(keys_.begin(), keys_.end());
            keys_.erase(std::unique(keys_.begin(), keys_.end()), keys_.end());
            sorted_ = true;
        }
    }

}  // namespace silkworm::trie

silkworm_PrefixSet *silkworm_PrefixSet_new() {
    return reinterpret_cast<silkworm_PrefixSet *>(new silkworm::trie::PrefixSet());
}

void silkworm_PrefixSet_free(silkworm_PrefixSet *set) {
    delete reinterpret_cast<silkworm::trie::PrefixSet *>(set);
}

void silkworm_PrefixSet_insert(silkworm_PrefixSet *set, silkworm_ByteView *key, int marker) {
    auto cpp_set = reinterpret_cast<silkworm::trie::PrefixSet *>(set);
    silkworm::ByteView cpp_key(key->data, key->length);
    cpp_set->insert(cpp_key, marker != 0);
}

void silkworm_PrefixSet_insert_move(silkworm_PrefixSet *set, silkworm_Bytes *key, int marker) {
    auto cpp_set = reinterpret_cast<silkworm::trie::PrefixSet *>(set);
    silkworm::Bytes cpp_key(key->data, key->length);
    cpp_set->insert(std::move(cpp_key), marker != 0);
}

int silkworm_PrefixSet_contains(silkworm_PrefixSet *set, silkworm_ByteView *prefix) {
    auto cpp_set = reinterpret_cast<silkworm::trie::PrefixSet *>(set);
    silkworm::ByteView cpp_prefix(prefix->data, prefix->length);
    return cpp_set->contains(cpp_prefix) ? 1 : 0;
}

silkworm_PrefixSetNextMarked
silkworm_PrefixSet_contains_and_next_marked(silkworm_PrefixSet *set, silkworm_ByteView *prefix,
                                            size_t invariant_prefix_len) {
    auto cpp_set = reinterpret_cast<silkworm::trie::PrefixSet *>(set);
    silkworm::ByteView cpp_prefix(prefix->data, prefix->length);
    auto [found, next_marked] = cpp_set->contains_and_next_marked(cpp_prefix, invariant_prefix_len);
    return silkworm_PrefixSetNextMarked{
            found ? 1 : 0,
            {next_marked.data(), next_marked.length()}
    };
}

size_t silkworm_PrefixSet_size(const silkworm_PrefixSet *set) {
    auto cpp_set = reinterpret_cast<const silkworm::trie::PrefixSet *>(set);
    return cpp_set->size();
}

int silkworm_PrefixSet_empty(const silkworm_PrefixSet *set) {
    auto cpp_set = reinterpret_cast<const silkworm::trie::PrefixSet *>(set);
    return cpp_set->empty() ? 1 : 0;
}

void silkworm_PrefixSet_clear(silkworm_PrefixSet *set) {
    auto cpp_set = reinterpret_cast<silkworm::trie::PrefixSet *>(set);
    cpp_set->clear();
}