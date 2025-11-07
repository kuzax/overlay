#ifndef DECOMPILER_H
#define DECOMPILER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

class LuaDeobfuscator {
public:
    class LuaDeobfuscatorException : public std::runtime_error {
    public:
        explicit LuaDeobfuscatorException(const std::string& message) : std::runtime_error(message) {}
    };

    LuaDeobfuscator(const std::vector<uint8_t>& data);
    std::vector<uint8_t> getData() const;

private:
    std::vector<uint8_t> data;
    size_t n;

    void parseHeader();
    void parseFunction();
    void parseCode();
    void parseConstants();
    void parseDebug();

    static uint32_t bytesToInt(const std::vector<uint8_t>& bytes);
    static std::vector<uint8_t> intToBytes(uint32_t value);
};

class Script {
public:
    class ScriptException : public std::runtime_error {
    public:
        explicit ScriptException(const std::string& message) : std::runtime_error(message) {}
    };

    Script(const std::vector<uint8_t>& raw);

    bool isMagicOk() const;
    bool isChecksumOk() const;
    bool isHeaderOffsetOk() const;
    bool isSignatureOk() const;

private:
    std::vector<uint8_t> raw;
    uint32_t magic;
    uint32_t headerOffset;
    uint32_t checksum;
    uint32_t checksumCalculated;

    void parseHeader();
    void parseLuaBytecode();
    void parseSignature();
};

bool isUtf8Bom(const void* pData, unsigned int uiLength);

namespace sex {
    namespace c_scripts {

        std::vector<uint8_t> decompile(const void* pData, unsigned int uiLength);

    } // namespace c_scripts
} // namespace utilities

#endif // DECOMPILER_H 