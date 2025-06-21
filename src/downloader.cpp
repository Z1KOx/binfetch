#include "downloader.hpp"
#include "utils.hpp"

#include <regex>
#include <iostream>

Downloader::Downloader() : m_Res()
{
    std::cout << "Enter download link: ";
    std::getline(std::cin, m_DlUrl);
    std::puts("[binfetch] Downloading Binaries...");

    const size_t LAST_SLASH = m_DlUrl.find_last_of('/');
    m_Filename = LAST_SLASH != std::string::npos
        ? m_DlUrl.substr(LAST_SLASH + 1)
        : "download.bin";

    m_Outfile.open(m_Filename, std::ios::binary);

    m_Curl = curl_easy_init();
    curl_easy_setopt(m_Curl, CURLOPT_URL, m_DlUrl.c_str());
    curl_easy_setopt(m_Curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_Curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, g_WriteCallBack);
    curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &m_Outfile);
}

Downloader::~Downloader()
{
    if (m_Outfile.is_open()) { m_Outfile.close(); }
    if (m_Curl) { curl_easy_cleanup(m_Curl); }
}

void Downloader::request() noexcept
{
    m_Res = curl_easy_perform(m_Curl);
    if (m_Res != CURLE_OK) {
        std::cerr << "Download Failed: " << curl_easy_strerror(m_Res) << '\n';
    }
}

std::string Downloader::getExtension(const std::string& url) noexcept
{
    const std::regex ext_regex(R"(\.([a-zA-Z0-9]+)(\?|$))");
    if (std::smatch match; std::regex_search(url, match, ext_regex)) {
        return "." + match[1].str();
    }
    return "";
}
