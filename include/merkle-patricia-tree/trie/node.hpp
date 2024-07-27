#ifndef SILKWORM_NODE_HPP_
#define SILKWORM_NODE_HPP_

#include "merkle-patricia-tree/common/bytes.hpp"
#include "merkle-patricia-tree/common/decoding_result.hpp"

#ifdef __cplusplus

#include "evmc/evmc.hpp"

#include <optional>
#include <vector>

namespace silkworm::trie {

// Used in node/silkworm/trie/intermediate_hashes.hpp
//
// Presumed invariants:
// 1) tree_mask ⊆ state_mask
// 2) hash_mask ⊆ state_mask
// 3) #hash_mask == #hashes
    class Node {
    public:
        Node() = default;

        explicit Node(uint16_t state_mask, uint16_t tree_mask, uint16_t hash_mask, std::vector<evmc::bytes32> hashes,
                      const std::optional<evmc::bytes32> &root_hash = std::nullopt);

        // copyable
        Node(const Node &other) = default;

        Node &operator=(const Node &other) = default;

        [[nodiscard]] uint16_t state_mask() const { return state_mask_; }

        [[nodiscard]] uint16_t tree_mask() const { return tree_mask_; }

        [[nodiscard]] uint16_t hash_mask() const { return hash_mask_; }

        [[nodiscard]] const std::vector<evmc::bytes32> &hashes() const { return hashes_; }

        [[nodiscard]] const std::optional<evmc::bytes32> &root_hash() const { return root_hash_; }

        void set_root_hash(const std::optional<evmc::bytes32> &root_hash);

        friend bool operator==(const Node &, const Node &) = default;

        //! \see Erigon's MarshalTrieNodeTyped
        [[nodiscard]] Bytes encode_for_storage() const;

        //! \see Erigon's UnmarshalTrieNodeTyped
        [[nodiscard]] static DecodingResult decode_from_storage(ByteView raw, Node &node);

    protected:
        uint16_t state_mask_{0};  // Each bit set indicates parenting of a hashed state
        uint16_t tree_mask_{0};   // Each bit set indicates parenting of a child
        uint16_t hash_mask_{0};   // Each bit set indicates ownership of a valid hash
        std::vector<evmc::bytes32> hashes_{};
        std::optional<evmc::bytes32> root_hash_{std::nullopt};

    private:
    };

    inline bool is_subset(uint16_t sub, uint16_t sup) { return (sub & sup) == sub; }

}  // namespace silkworm::trie

#endif // __cplusplus

// C interface
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


typedef struct silkworm_Node silkworm_Node;

silkworm_Node *silkworm_Node_create();
void silkworm_Node_destroy(silkworm_Node *node);

silkworm_Node *silkworm_Node_create_with_params(uint16_t state_mask, uint16_t tree_mask, uint16_t hash_mask,
                                                const silkworm_Bytes *hashes, size_t hashes_count,
                                                const uint8_t *root_hash);

uint16_t silkworm_Node_state_mask(const silkworm_Node *node);
uint16_t silkworm_Node_tree_mask(const silkworm_Node *node);
uint16_t silkworm_Node_hash_mask(const silkworm_Node *node);

size_t silkworm_Node_hashes_count(const silkworm_Node *node);
const uint8_t *silkworm_Node_hash_at(const silkworm_Node *node, size_t index);

bool silkworm_Node_has_root_hash(const silkworm_Node *node);
const uint8_t *silkworm_Node_root_hash(const silkworm_Node *node);

void silkworm_Node_set_root_hash(silkworm_Node *node, const uint8_t *root_hash);

silkworm_Bytes silkworm_Node_encode_for_storage(const silkworm_Node *node);

silkworm_DecodingResult silkworm_Node_decode_from_storage(const silkworm_ByteView *raw, silkworm_Node *node);

bool silkworm_is_subset(uint16_t sub, uint16_t sup);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_NODE_HPP_
