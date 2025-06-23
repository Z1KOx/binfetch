#include "downloader.hpp"
#include "utils.hpp"
namespace fs = std::filesystem;

Downloader::Downloader() : m_Res()
{
    std::cout << "[binfetch] Enter download link: ";
    std::getline(std::cin, m_DlUrl);
    std::puts("[binfetch] Downloading Binaries...");

    m_Curl = curl_easy_init();
    curl_easy_setopt(m_Curl, CURLOPT_URL, m_DlUrl.c_str());
    curl_easy_setopt(m_Curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_Curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, g_VecWriteCallback);
    curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &m_Buffer);
}

Downloader::~Downloader()
{
    if (m_Curl) { curl_easy_cleanup(m_Curl); }
}

void Downloader::request()
{
    m_Res = curl_easy_perform(m_Curl);
    if (m_Res != CURLE_OK) {
        THROW_FORMATTED_ERROR("Download failed: ", curl_easy_strerror(m_Res));
    }
}

void Downloader::confirmSave() noexcept
{
    std::puts("[binfetch] Are you sure you want to download this file? [y/n]");
    std::cout << "> ";
    char input; std::cin >> input;

    if('y' == input) {
        saveToFile();
    }
    else {
        std::puts("[binfetch] Download aborted");
    }
}

void Downloader::saveToFile() noexcept
{
    const size_t LSLASH = m_DlUrl.find_last_of('/');
    m_Filename = LSLASH != std::string::npos
        ? m_DlUrl.substr(LSLASH + 1)
        : "download.bin";

    std::ofstream file(m_Filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char*>(m_Buffer.data()), m_Buffer.size());
        file.close();
    }

    std::cout << "[binfetch] Saved to: " << fs::absolute(m_Filename) << '\n';
}