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

#include "merkle-patricia-tree/trie/node.hpp"

#include <bit>
#include <utility>

#include "merkle-patricia-tree/common/assert.hpp"
#include "merkle-patricia-tree/common/endian.hpp"

namespace silkworm::trie {

Node::Node(uint16_t state_mask, uint16_t tree_mask, uint16_t hash_mask, std::vector<evmc::bytes32> hashes,
           const std::optional<evmc::bytes32>& root_hash)
    : state_mask_{state_mask},
      tree_mask_{tree_mask},
      hash_mask_{hash_mask},
      hashes_{std::move(hashes)},
      root_hash_{root_hash} {
    SILKWORM_ASSERT(is_subset(tree_mask, state_mask));
    SILKWORM_ASSERT(is_subset(hash_mask, state_mask));
    SILKWORM_ASSERT(std::cmp_equal(std::popcount(hash_mask_), hashes_.size()));
}

void Node::set_root_hash(const std::optional<evmc::bytes32>& root_hash) { root_hash_ = root_hash; }

Bytes Node::encode_for_storage() const {
    const size_t buf_size{/* 3 masks state/tree/hash 2 bytes each */ 6 +
                          /* root hash */ (root_hash_.has_value() ? kHashLength : 0u) +
                          /* hashes */ hashes_.size() * kHashLength};
    Bytes buf(buf_size, '\0');
    endian::store_big_u16(&buf[0], state_mask_);
    endian::store_big_u16(&buf[2], tree_mask_);
    endian::store_big_u16(&buf[4], hash_mask_);

    size_t pos{6};
    if (root_hash_.has_value()) {
        std::memcpy(&buf[pos], root_hash_->bytes, kHashLength);
        pos += kHashLength;
    }

    if (!hashes_.empty()) {
        std::memcpy(&buf[pos], hashes_.data(), hashes_.size() * kHashLength);
    }
    return buf;
}

DecodingResult Node::decode_from_storage(ByteView raw, Node& node) {
    // At least state/tree/hash masks need to be present
    if (raw.length() < 6) {
        return tl::unexpected{DecodingError::kInputTooShort};
    }
    // Beyond the 6th byte the length must be a multiple of kHashLength
    if ((raw.length() - 6) % kHashLength != 0) {
        return tl::unexpected{DecodingError::kInvalidHashesLength};
    }

    node.root_hash_.reset();
    node.hashes_.clear();
    node.state_mask_ = endian::load_big_u16(&raw[0]);
    node.tree_mask_ = endian::load_big_u16(&raw[2]);
    node.hash_mask_ = endian::load_big_u16(&raw[4]);

    if (!is_subset(node.tree_mask_, node.state_mask_) || !is_subset(node.hash_mask_, node.state_mask_)) {
        return tl::unexpected{DecodingError::kInvalidMasksSubsets};
    }

    raw.remove_prefix(6);

    size_t expected_num_hashes{static_cast<size_t>(std::popcount(node.hash_mask_))};
    size_t effective_num_hashes{raw.length() / kHashLength};

    if (effective_num_hashes < expected_num_hashes) {
        return tl::unexpected{DecodingError::kInvalidHashesLength};
    }

    size_t delta{effective_num_hashes - expected_num_hashes};
    if (delta > 1) {
        return tl::unexpected{DecodingError::kInvalidHashesLength};
    } else if (delta == 1) {
        node.root_hash_.emplace();
        std::memcpy(node.root_hash_->bytes, raw.data(), kHashLength);
        raw.remove_prefix(kHashLength);
        --effective_num_hashes;
    }

    node.hashes_.resize(effective_num_hashes);
    if (effective_num_hashes) {
        std::memcpy(node.hashes_.data(), raw.data(), raw.length());
    }
    return {};
}

}  // namespace silkworm::trie

#endif

// C interface implementation
extern "C" {

struct silkworm_Node {
    silkworm::trie::Node cpp_node;
};

silkworm_Node* silkworm_Node_create() {
    return new silkworm_Node();
}

void silkworm_Node_destroy(silkworm_Node* node) {
    delete node;
}

silkworm_Node* silkworm_Node_create_with_params(uint16_t state_mask, uint16_t tree_mask, uint16_t hash_mask,
                                                const silkworm_Bytes* hashes, size_t hashes_count,
                                                const uint8_t* root_hash) {
    std::vector<evmc::bytes32> cpp_hashes;
    for (size_t i = 0; i < hashes_count; ++i) {
        evmc::bytes32 hash;
        std::memcpy(hash.bytes, hashes[i].data, sizeof(evmc::bytes32));
        cpp_hashes.push_back(hash);
    }

    std::optional<evmc::bytes32> cpp_root_hash;
    if (root_hash) {
        cpp_root_hash = evmc::bytes32();
        std::memcpy(cpp_root_hash->bytes, root_hash, sizeof(evmc::bytes32));
    }

    auto node = new silkworm_Node();
    node->cpp_node = silkworm::trie::Node(state_mask, tree_mask, hash_mask, std::move(cpp_hashes), cpp_root_hash);
    return node;
}

uint16_t silkworm_Node_state_mask(const silkworm_Node* node) {
    return node->cpp_node.state_mask();
}

uint16_t silkworm_Node_tree_mask(const silkworm_Node* node) {
    return node->cpp_node.tree_mask();
}

uint16_t silkworm_Node_hash_mask(const silkworm_Node* node) {
    return node->cpp_node.hash_mask();
}

size_t silkworm_Node_hashes_count(const silkworm_Node* node) {
    return node->cpp_node.hashes().size();
}

const uint8_t* silkworm_Node_hash_at(const silkworm_Node* node, size_t index) {
    if (index >= node->cpp_node.hashes().size()) {
        return nullptr;
    }
    return node->cpp_node.hashes()[index].bytes;
}

bool silkworm_Node_has_root_hash(const silkworm_Node* node) {
    return node->cpp_node.root_hash().has_value();
}

const uint8_t* silkworm_Node_root_hash(const silkworm_Node* node) {
    if (!node->cpp_node.root_hash()) {
        return nullptr;
    }
    return node->cpp_node.root_hash()->bytes;
}

void silkworm_Node_set_root_hash(silkworm_Node* node, const uint8_t* root_hash) {
    if (root_hash) {
        evmc::bytes32 cpp_root_hash;
        std::memcpy(cpp_root_hash.bytes, root_hash, sizeof(evmc::bytes32));
        node->cpp_node.set_root_hash(cpp_root_hash);
    } else {
        node->cpp_node.set_root_hash(std::nullopt);
    }
}

silkworm_Bytes silkworm_Node_encode_for_storage(const silkworm_Node* node) {
    silkworm::Bytes encoded = node->cpp_node.encode_for_storage();
    silkworm_Bytes result = silkworm_Bytes_create(encoded.size());
    silkworm_Bytes_append(&result, encoded.data(), encoded.size());
    return result;
}

silkworm_DecodingResult silkworm_Node_decode_from_storage(const silkworm_ByteView* raw, silkworm_Node* node) {
    silkworm::ByteView cpp_raw(raw->data, raw->length);
    auto result = silkworm::trie::Node::decode_from_storage(cpp_raw, node->cpp_node);
    silkworm_DecodingResult c_result;
    if (result) {
        c_result.has_error = 0;
    } else {
        c_result.has_error = 1;
        c_result.error = static_cast<silkworm_DecodingError>(result.error());
    }
    return c_result;
}

bool silkworm_is_subset(uint16_t sub, uint16_t sup) {
    return silkworm::trie::is_subset(sub, sup);
}

} // extern "C"