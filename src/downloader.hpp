#pragma once
#include <fstream>
#include <string>

#include "curl/curl.h"

class Downloader
{
public:
    explicit Downloader();
    ~Downloader() noexcept;

    Downloader(const Downloader&) = delete;
    Downloader& operator=(const Downloader&) = delete;

public:
    void request() noexcept;
    [[nodiscard]] const std::string& getFileName() const { return m_Filename; }

private:
    [[nodiscard]] static std::string getExtension(const std::string& url) noexcept;

private:
    std::string m_DlUrl, m_Filename;
    std::ofstream m_Outfile;
    CURLcode m_Res;
    CURL* m_Curl = nullptr;
};
