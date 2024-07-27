#include <algorithm>
#include <cstring>
#include <merkle-patricia-tree/common/util.hpp>
#include <merkle-patricia-tree/rlp/encode.hpp>
#include <merkle-patricia-tree/types/evmc_bytes32.hpp>

namespace silkworm {

    evmc::bytes32 to_bytes32(ByteView bytes) {
        evmc::bytes32 out;
        if (!bytes.empty()) {
            size_t n{std::min(bytes.length(), kHashLength)};
            std::memcpy(out.bytes + kHashLength - n, bytes.data(), n);
        }
        return out;
    }

    std::string to_hex(const evmc::bytes32 &value, bool with_prefix) {
        return silkworm::to_hex(ByteView{value.bytes}, with_prefix);
    }

}  // namespace silkworm

namespace silkworm::rlp {

    void encode(Bytes &to, const evmc::bytes32 &value) {
        silkworm::rlp::encode(to, ByteView{value.bytes});
    }

    size_t length(const evmc::bytes32 &value) noexcept {
        return silkworm::rlp::length(ByteView{value.bytes});
    }

    DecodingResult decode(ByteView &from, evmc::bytes32 &to, Leftover mode) noexcept {
        return silkworm::rlp::decode(from, to.bytes, mode);
    }

}
// Вспомогательная функция для конвертации evmc_bytes32 в evmc::bytes32
evmc::bytes32 to_cpp_bytes32(const evmc_bytes32& c_bytes32) {
    evmc::bytes32 cpp_bytes32;
    std::memcpy(cpp_bytes32.bytes, c_bytes32.bytes, sizeof(cpp_bytes32.bytes));
    return cpp_bytes32;
}

evmc_bytes32 evmc_to_bytes32(const uint8_t* bytes, size_t length) {
    auto cpp_result = silkworm::to_bytes32(silkworm::ByteView(bytes, length));
    evmc_bytes32 result;
    std::memcpy(result.bytes, cpp_result.bytes, sizeof(result.bytes));
    return result;
}

char* evmc_to_hex(const evmc_bytes32* value, bool with_prefix) {
    std::string result = silkworm::to_hex(to_cpp_bytes32(*value), with_prefix);
    char* c_result = (char*)malloc(result.length() + 1);
    strcpy(c_result, result.c_str());
    return c_result;
}

size_t evmc_rlp_encode(const evmc_bytes32* value, uint8_t* output, size_t output_size) {
    silkworm::Bytes to;
    silkworm::rlp::encode(to, to_cpp_bytes32(*value));
    size_t to_copy = std::min(output_size, to.size());
    std::memcpy(output, to.data(), to_copy);
    return to.size();
}

size_t evmc_rlp_length(const evmc_bytes32* value) {
    return silkworm::rlp::length(to_cpp_bytes32(*value));
}

int evmc_rlp_decode(const uint8_t* input, size_t input_size, evmc_bytes32* output) {
    silkworm::ByteView from(input, input_size);
    evmc::bytes32 to;
    auto result = silkworm::rlp::decode(from, to);
    if (result.has_value()) {  // Проверяем, успешно ли выполнено декодирование
        std::memcpy(output->bytes, to.bytes, sizeof(output->bytes));
        return 1;
    }
    return 0;
}