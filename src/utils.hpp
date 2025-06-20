#pragma once

[[nodiscard]] inline size_t g_WriteCallBack(
    void* ptr,
    const size_t size,
    const size_t nmemb,
    void* stream
) noexcept
{
    if (!stream) return 0;

    const auto out = static_cast<std::ofstream*>(stream);
    const size_t total = size * nmemb;

    if (!out->write(static_cast<char*>(ptr), total)) {
        return 0;
    }

    return total;
}

[[nodiscard]] inline size_t g_StrWriteCallback(
    void* contents,
    const size_t size,
    const size_t nmemb,
    std::string* out
) noexcept
{
    const size_t totalSize = size * nmemb;
    out->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}