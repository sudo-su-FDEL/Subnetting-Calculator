#include <iostream>
#include <string>
#include <bitset>
#include <regex>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

struct SubnetCalculator {
    std::string ip_address;
    int cidr;

    SubnetCalculator(std::string ip, int c) : ip_address(ip), cidr(c) {}

    uint32_t ipToUInt(const std::string& ip) {
        struct sockaddr_in sa;
        inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
        return ntohl(sa.sin_addr.s_addr);
    }

    std::string uintToIp(uint32_t ip) {
        struct in_addr addr;
        addr.s_addr = htonl(ip);
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
        return std::string(str);
    }

    void calculate() {
        uint32_t ip = ipToUInt(ip_address);
        uint32_t mask = (0xFFFFFFFF << (32 - cidr)) & 0xFFFFFFFF;
        uint32_t network = ip & mask;
        uint32_t broadcast = network | ~mask;

        int total_hosts = (1 << (32 - cidr));
        int usable_hosts = total_hosts - 2;

        std::cout << "IP Address: " << ip_address << std::endl;
        std::cout << "Network Address: " << uintToIp(network) << std::endl;
        std::cout << "Usable Host IP Range: " << uintToIp(network + 1) << " - " << uintToIp(broadcast - 1) << std::endl;
        std::cout << "Broadcast Address: " << uintToIp(broadcast) << std::endl;
        std::cout << "Total Number of Hosts: " << total_hosts << std::endl;
        std::cout << "Number of Usable Hosts: " << usable_hosts << std::endl;
        std::cout << "Subnet Mask: " << uintToIp(mask) << std::endl;
        std::cout << "CIDR Notation: /" << cidr << std::endl;
        std::cout << "IP Type: " << (ip >= ipToUInt("10.0.0.0") && ip <= ipToUInt("10.255.255.255") ? "Private" : "Public") << std::endl;
    }
};

bool isValidIP(const std::string& ip) {
    std::regex ip_regex(
        R"(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    return std::regex_match(ip, ip_regex);
}

bool isValidCIDR(int cidr) {
    return (cidr >= 0 && cidr <= 32);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    while (true) {
        std::string ip;
        int cidr;

        do {
            
            system("cls");
            std::cout << "Enter IP address: ";
            std::cin >> ip;
            if (!isValidIP(ip)) {
                 
                std::cout << "Invalid IP address format. Please try again." << std::endl;
                system("pause");
            }
        } while (!isValidIP(ip));

        do {
            std::cout << "Enter CIDR notation (0-32): ";
            std::cin >> cidr;
            std::cout << "\n\n";
            if (!isValidCIDR(cidr)) {
                
                std::cout << "Invalid CIDR notation. Please enter a number between 0 and 32." << std::endl;
                   system("pause");
            }
            
        } while (!isValidCIDR(cidr));

        SubnetCalculator calculator(ip, cidr);
        calculator.calculate();

        std::cout << "Press Enter to calculate another subnet, or type 'exit' to quit." << std::endl;
        std::cin.ignore(); // Ignore the newline character left in the buffer
        std::string exit_choice;
        std::getline(std::cin, exit_choice);
        if (exit_choice == "exit") {
            break;
            
        }
    }

    WSACleanup();
    return 0;
}
