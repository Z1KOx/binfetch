#pragma once
#include <fstream>
#include <string>

#include "curl/curl.h"

class Downloader
{
public:
    explicit Downloader(std::string dUrl)
        : m_dUrl(std::move(dUrl)), m_res()
    {}
    ~Downloader() noexcept;

    Downloader( const Downloader& ) = delete;
    Downloader& operator=( const Downloader& ) = delete;

public:
    void init() noexcept;
    void request() noexcept;

    [[nodiscard]] const std::string& getFileName() const {
        return m_filename;
    }

private:
    static size_t writeData(void* ptr, size_t size, size_t nmemb, void* stream) noexcept;
    [[nodiscard]] static std::string getExtension(const std::string& url) noexcept;

private:
    std::string m_dUrl;
    std::string m_filename;
    std::ofstream m_outfile;
    CURLcode m_res;
    CURL* m_curl = nullptr;
};
