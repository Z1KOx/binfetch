#pragma once
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class VTScanner
{
public:
    explicit VTScanner(const std::string& fpath, std::string&& apiKey)
        : m_Fpath(fpath), m_ApiKey(std::move(apiKey)) {}
    ~VTScanner() noexcept = default;

    VTScanner(const VTScanner&) = delete;
    VTScanner& operator=(const VTScanner&) = delete;

public:
    json scan() noexcept;
    void printResults() const noexcept;

private:
    json upload() noexcept;
    json analyze(const std::string& analysisId) noexcept;

private:
    std::string m_Fpath, m_ApiKey, m_AnalizeBuff, m_UploadBuff;
    json m_Result;
};
