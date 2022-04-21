#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "types.h"
#include "lib.h"
#include "ip_filter.h"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

constexpr static const int IP_POOL_VECTOR_SIZE = 10000;

string_vector split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

int_vector stringToInt(const std::vector<std::string>& ip)
{
    std::vector<int> result;
    for (const auto& byte : ip)
    {
        result.push_back(std::stoi(byte));
    }
    return result;
}

void printIpPool(const ip_pool_vector & ip_pool)
{
    for (const auto& ip : ip_pool)
    {
        for (auto byte = ip.cbegin(); byte != ip.cend(); ++byte)
        {
            if (byte != ip.cbegin())
            {
                std::cout << ".";
            }
            std::cout << *byte;
        }
        std::cout << std::endl;
    }
}

void reverseLexicographicalSortIpPool(ip_pool_vector& ip_pool) 
{
    std::sort(ip_pool.begin(), ip_pool.end(), std::greater<>());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{     
    try
    {
        ip_pool_vector ip_pool;
        ip_pool.reserve(IP_POOL_VECTOR_SIZE);        

        for (std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(stringToInt(split(v.at(0), '.')));
        }

        ip_pool.shrink_to_fit();
        // -------------------------------------------------
        // reverse lexicographically sort
        reverseLexicographicalSortIpPool(ip_pool);
        printIpPool(ip_pool);                

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // -------------------------------------------------
        // filter by first byte and output
        printIpPool(filterIpPool(ip_pool, 1));

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // -------------------------------------------------
        // filter by first and second bytes and output
        printIpPool(filterIpPool(ip_pool, 46, 70));

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // filter by any byte and output
        printIpPool(filterAnyIpPool(ip_pool, 46));

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	return EXIT_SUCCESS;
}
