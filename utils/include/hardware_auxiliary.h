#pragma once

#include <iostream>

// namespace scss {

class HardwareAuxiliary {
public:
    static bool get_mac_address(std::string& mac_addr);
    static bool get_mac_address(const std::string& interface, std::string& mac_addr);
};

// } /* namespace scss*/

