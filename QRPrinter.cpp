#include "QRPrinter.h"
#include <iostream>
#include <cstring>

void QRPrinter::printMatrix(QRcode* qr) {
    if (!qr) return;

    // Print top border
    std::cout << "┌";
    for (int i = 0; i < qr->width * 2; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";

    // Print QR code content
    for (int y = 0; y < qr->width; y++) {
        std::cout << "│";
        for (int x = 0; x < qr->width; x++) {
            // Check if the module is set (black)
            if (qr->data[y * qr->width + x] & 1) {
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "│\n";
    }

    // Print bottom border
    std::cout << "└";
    for (int i = 0; i < qr->width * 2; i++) {
        std::cout << "─";
    }
    std::cout << "┘\n";
}

void QRPrinter::cleanupQRCode(QRcode* qr) {
    if (qr) {
        QRcode_free(qr);
    }
}

void QRPrinter::printQRCode(const std::string& qrData) {
    // Generate QR code with error correction level M (15%)
    QRcode* qr = QRcode_encodeString(qrData.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
    
    if (!qr) {
        std::cerr << "Error: Could not generate QR code\n";
        return;
    }

    printMatrix(qr);
    cleanupQRCode(qr);
} 