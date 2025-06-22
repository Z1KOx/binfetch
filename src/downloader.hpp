#pragma once
#include <vector>
#include <string>
#include <filesystem>

#include "curl/curl.h"

class Downloader
{
public:
    explicit Downloader();
    ~Downloader() noexcept;

    Downloader(const Downloader&) = delete;
    Downloader& operator=(const Downloader&) = delete;

public:
    void request();
    void confirmSave() noexcept;
    [[nodiscard]] std::vector<unsigned char> getFileBinaries() const { return m_Buffer; }

private:
    void saveToFile() noexcept;

private:
    std::filesystem::path m_Filename;
    std::vector<unsigned char> m_Buffer{};
    std::string m_DlUrl;
    CURLcode m_Res;
    CURL* m_Curl = nullptr;
};