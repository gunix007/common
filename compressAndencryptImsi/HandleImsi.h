#ifndef __HANDLEIMSI_H__
#define __HANDLEIMSI_H__

#if defined(__cplusplus)
extern "C" {
#endif

int CompressAndEncryptImsiData(unsigned char *cipherTextBuf,
        size_t cipherTextBufSize, char *rawData, char *key);
int DecryptAndUncompressImsiData(char *plainTextBuf, size_t plainTextBufSize,
        char *cipherHexString, char *key);

#if defined(__cplusplus)
}
#endif

#endif
