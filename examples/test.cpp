#include <chrono>
#include <execution>
#include <iostream>
#include <thread>
#include <wincpp/patterns/scanner.hpp>
#include <wincpp/process.hpp>

using namespace wincpp;

int main()
{
    try
    {
        const auto process = process_t::open( "RobloxPlayerBeta.exe" );

        if ( !process )
        {
            std::cout << "Failed to open the process." << std::endl;
            return 1;
        }

        const auto& wvt = process->module_factory[ "WinTrust" ][ "WinVerifyTrust" ];

        std::cout << wvt << std::endl;
    }
    catch ( const std::system_error& e )
    {
        std::cout << "[-] Error [" << e.code() << "]: " << e.what() << std::endl;
    }
    catch ( const std::exception& e )
    {
        std::cout << "[-] Error: " << e.what() << std::endl;
    }

    return 0;
}