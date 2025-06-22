#pragma once
#include "pch.h"

class VTScanner
{
public:
    explicit VTScanner(const std::vector<unsigned char>& data, std::string&& apiKey)
        : m_ApiKey(std::move(apiKey)), m_Data(data) {}
    ~VTScanner() noexcept = default;

    VTScanner(const VTScanner&) = delete;
    VTScanner& operator=(const VTScanner&) = delete;

public:
    nlohmann::json scan();
    void printResults() const;

private:
    nlohmann::json upload();
    nlohmann::json analyze(const std::string& analysisId);

private:
    std::string m_ApiKey, m_AnalizeBuff, m_UploadBuff;
    std::vector<unsigned char> m_Data;
    nlohmann::json m_Result;
};
