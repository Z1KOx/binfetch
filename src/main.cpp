#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>

size_t write_data(void* ptr, size_t size, const size_t nmemb, void* stream) {
    std::ofstream* out = static_cast<std::ofstream*>(stream);
    size_t total = size * nmemb;
    out->write(static_cast<char*>(ptr), total);
    return total;
}

class Request
{
public:
    explicit Request(const std::string& dlink) : m_dlink(dlink) {}
    ~Request() noexcept
    {
        if (m_outfile.is_open()) { m_outfile.close(); }
        if (m_curl) { curl_easy_cleanup(m_curl); }
    };

public:
    void init() noexcept
    {
        m_outfile.open("download.zip", std::ios::binary);
        m_curl = curl_easy_init();

        if (m_curl)
        {
            curl_easy_setopt(m_curl, CURLOPT_URL, m_dlink.c_str());
            curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "Mozilla/5.0");
            curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_outfile);

            const CURLcode res = curl_easy_perform(m_curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
        }
    }

private:
    size_t writeData(void* ptr, size_t size, const size_t nmemb, void* stream) const noexcept
    {
        auto out = static_cast<std::ofstream*>(stream);
        const size_t total = size * nmemb;
        out->write(static_cast<char*>(ptr), total);
        return total;
    }

private:
    std::string m_dlink;
    std::ofstream m_outfile;
    CURL* m_curl = nullptr;
};

int main()
{
    std::puts("Enter download link");
    std::cout << "> ";
    std::string dlink; std::getline(std::cin, dlink);

    Request req(dlink);
    req.init();
}
