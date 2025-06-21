#include "vtscanner.hpp"
#include "utils.hpp"

#include <thread>
#include <iostream>
#include <curl/curl.h>

json VTScanner::scan() noexcept
{
    json uploadResponse = upload();

    if (!uploadResponse.contains("data") || !uploadResponse["data"].contains("id")) {
        std::cerr << "[binfetch] Upload failed or malformed response.\n";
        return {};
    }

    const std::string analysisId = uploadResponse["data"]["id"];
    std::cout << "[binfetch] Analysis ID: " << analysisId << '\n';

    for (int i = 0; i < 30; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        json analysis = analyze(analysisId);

        const std::string status = analysis["data"]["attributes"]["status"];
        std::cout << "[binfetch] Status: " << status << '\n';

        if (status == "completed") {
            m_Result = analysis;
            return analysis;
        }
    }

    return {};
}

void VTScanner::printResults() const noexcept
{
    if (m_Result.empty()) {
        std::cerr << "[binfetch] No scan result available.\n";
        return;
    }

    const auto& stats = m_Result["data"]["attributes"]["stats"];
    const int malicious = stats["malicious"];
    int total = 0;

    for (const auto& [_, value] : stats.items()) {
        total += value.get<int>();
    }

    std::cout << "\n[binfetch] " << malicious << "/" << total << " security vendors flagged this file as malicious\n";
}

json VTScanner::upload() noexcept
{
    CURL* curl = curl_easy_init();
    if (!curl) return {};

    curl_mime* mime = curl_mime_init(curl);
    curl_mimepart* part = curl_mime_addpart(mime);

    curl_mime_name(part, "file");
    curl_mime_filedata(part, m_Fpath.c_str());

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("x-apikey: " + m_ApiKey).c_str());
    headers = curl_slist_append(headers, "accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.virustotal.com/api/v3/files");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, g_StrWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_UploadBuff);

    std::puts("[binfetch] Uploading...");
    const CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << curl_easy_strerror(res) << '\n';
    }

    curl_mime_free(mime);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return json::parse(m_UploadBuff);
}

json VTScanner::analyze(const std::string& analysisId) noexcept
{
    m_AnalizeBuff.clear();

    CURL* curl = curl_easy_init();
    if (!curl) return {};

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("x-apikey: " + m_ApiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    const std::string URL = "https://www.virustotal.com/api/v3/analyses/" + analysisId;
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, g_StrWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_AnalizeBuff);

    const CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << curl_easy_strerror(res) << '\n';
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return json::parse(m_AnalizeBuff);
}