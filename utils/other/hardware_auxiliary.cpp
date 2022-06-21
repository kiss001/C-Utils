#include "hardware_auxiliary.h"

#include <unistd.h>
#include <dirent.h>
#include <netinet/in.h>
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <sys/stat.h>
#include <sys/types.h>  
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "auxiliary.h"

// namespace scss {

bool HardwareAuxiliary::get_mac_address(std::string& mac_addr) {
    static std::string mac_address;
    if (!mac_address.empty()) {
        mac_addr = mac_address;
        return true;
    }

    mac_addr.clear();    
    int sock = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock < 0) {  
        return false;
    }  
    
    struct ifreq ifr;
    struct ifconf ifc;
    
    uint32_t max_len = 4096;
    UniquePtr<char[]> buf(new char[max_len]);

    ifc.ifc_len = max_len;
    ifc.ifc_buf = buf.get();
    bool ret = (ioctl(sock, SIOCGIFCONF, &ifc) >= 0);
    if (!ret) {
        close(sock);
        return false;
    }
    
    struct ifreq* it = ifc.ifc_ifcu.ifcu_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    char sz_mac[64];
    for (; it != end; ++it) {
        memcpy(ifr.ifr_name, it->ifr_name, sizeof(ifr.ifr_name) - 1);  
        ret = ioctl(sock, SIOCGIFFLAGS, &ifr) >= 0;
        if (!ret) {
            continue;
        }
        if (ifr.ifr_flags & IFF_LOOPBACK) { // don't count loopback
            continue;
        }            
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
            unsigned char* ptr = (unsigned char*)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
            snprintf(sz_mac, 64, "%02X%02X%02X%02X%02X%02X", 
                    *ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));
            
            std::string(sz_mac).swap(mac_addr);
            close(sock);
            mac_address = mac_addr;
            return true;
        }
    }
    close(sock);
    return false;
}

bool HardwareAuxiliary::get_mac_address(const std::string& interface, std::string& mac_addr) {
    mac_addr.clear();    
    int sock = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock < 0) {  
        return(false);  
    }  
  
    struct ifreq ifr = { 0 };  
    memcpy(ifr.ifr_name, interface.c_str(), sizeof(ifr.ifr_name) - 1);  
    bool ret = (ioctl(sock, SIOCGIFHWADDR, &ifr) >= 0);
    close(sock);

    if (ret) {
        char mac[16] = { 0 };  
        for (int index = 0; index < 6; ++index) {  
            size_t value = ifr.ifr_hwaddr.sa_data[index] & 0xFF;  
            mac[2 * index + 0] = Auxiliary::hex2char(value / 16);  
            mac[2 * index + 1] = Auxiliary::hex2char(value % 16);  
        }  
        std::string(mac).swap(mac_addr);
    }
    return(ret);
}

// } /* namespace secure_keystore */
