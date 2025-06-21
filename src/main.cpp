#include "downloader.hpp"
#include "vtscanner.hpp"

int main()
{
    Downloader dl;
    dl.request();

    VTScanner vts(
        dl.getFileName(),
        "API_KEY"
    );
    vts.scan();
    vts.printResults();
}
