#pragma once
#include "pch.h"

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
    out->write(static_cast<char*>(ptr), total);

    return total;
}

[[nodiscard]] inline size_t g_StrWriteCallback(
    void* contents,
    const size_t size,
    const size_t nmemb,
    std::string* out
) noexcept
{
    if (!out) return 0;

    const size_t totalSize = size * nmemb;
    out->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

[[nodiscard]] inline size_t g_VecWriteCallback(
    void* contents,
    const size_t size,
    const size_t nmemb,
    void* userp
) noexcept
{
    const size_t totalSize = size * nmemb;
    auto* buffer = static_cast<std::vector<unsigned char>*>(userp);
    const auto* data = static_cast<unsigned char*>(contents);
    buffer->insert(buffer->end(), data, data + totalSize);
    return totalSize;
}