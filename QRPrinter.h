#pragma once
#include <string>
#include <vector>
#include <qrencode.h>

class QRPrinter {
public:
    static void printQRCode(const std::string& qrData);
    
private:
    static void printMatrix(QRcode* qr);
    static void cleanupQRCode(QRcode* qr);
}; 