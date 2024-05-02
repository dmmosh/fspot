#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16
bool decryptFile(const std::string& inputFile, const std::string& outputFile, const unsigned char* key, const unsigned char* iv) {
    FILE* inFile = fopen(inputFile.c_str(), "rb");
    FILE* outFile = fopen(outputFile.c_str(), "wb");
    if (!inFile || !outFile) {
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    int bytesRead = 0, bytesWritten = 0;
    unsigned char inBuffer[AES_BLOCK_SIZE];
    unsigned char outBuffer[AES_BLOCK_SIZE + AES_BLOCK_SIZE]; // Add space for padding

    while ((bytesRead = fread(inBuffer, 1, AES_BLOCK_SIZE, inFile)) > 0) {
        if (EVP_DecryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead) != 1) {
            std::cerr << "Error in DecryptUpdate." << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            fclose(inFile);
            fclose(outFile);
            return false;
        }
        fwrite(outBuffer, 1, bytesWritten, outFile);
    }

    int finalBytesWritten = 0;
    if (EVP_DecryptFinal_ex(ctx, outBuffer, &finalBytesWritten) != 1) {
        std::cerr << "Error in DecryptFinal." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        fclose(inFile);
        fclose(outFile);
        return false;
    }
    fwrite(outBuffer, 1, finalBytesWritten, outFile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(inFile);
    fclose(outFile);

    return true;
}

int main() {
    std::string folder = std::string(getenv("HOME")) + "/coding/fspot/fspot/";

    std::string inputFile = folder + "var/auth.obj.aes";
    std::string outputFile = folder + "var/auth.txt";
    const unsigned char* key = (unsigned char*)getenv("FSPOT_PASSWORD"); // 256-bit key
    const unsigned char* iv = (unsigned char*)"0123456789abcdef"; // 128-bit IV

    decryptFile(inputFile.c_str(), outputFile.c_str(), key, iv);

    std::cout << "Decryption complete." << std::endl;

    return 0;
}