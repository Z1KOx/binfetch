#include "downloader.hpp"
#include "vtscanner.hpp"

#include <iostream>

int main()
{
    try
    {
        Downloader dl;
        dl.request();

        VTScanner vts(
            dl.getFileBinaries(),
            "API-Key"
        );
        vts.scan();
        vts.printResults();

        dl.confirmSave();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n'; return -1;
    }
}