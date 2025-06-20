#include "downloader.hpp"

#include <regex>
#include <iostream>

Downloader::Downloader(const std::string& dUrl) : m_dUrl(dUrl), m_res()
{
    const size_t LAST_SLASH = m_dUrl.find_last_of('/');
    m_filename = LAST_SLASH != std::string::npos
        ? m_dUrl.substr(LAST_SLASH + 1)
        : "download.bin";

    m_outfile.open(m_filename, std::ios::binary);

    m_curl = curl_easy_init();
    curl_easy_setopt(m_curl, CURLOPT_URL, m_dUrl.c_str());
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_outfile);
}

Downloader::~Downloader()
{
    if (m_outfile.is_open()) { m_outfile.close(); }
    if (m_curl) { curl_easy_cleanup(m_curl); }
}

void Downloader::request() noexcept
{
    if (!m_curl) return;

    m_res = curl_easy_perform(m_curl);
    if (m_res != CURLE_OK) {
        std::cerr << "Download Failed: " << curl_easy_strerror(m_res) << '\n'; return;
    }
}

size_t Downloader::writeData(void* ptr, size_t size, size_t nmemb, void* stream) noexcept
{
    auto out = static_cast<std::ofstream*>(stream);
    size_t total = size * nmemb;
    out->write(static_cast<char*>(ptr), total);
    return total;
}

std::string Downloader::getExtension(const std::string& url) noexcept
{
    const std::regex ext_regex(R"(\.([a-zA-Z0-9]+)(\?|$))");
    std::smatch match;
    if (std::regex_search(url, match, ext_regex))
        return "." + match[1].str();
    return "";
}
