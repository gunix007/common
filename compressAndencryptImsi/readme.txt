/* 函数名:   CompressAndEncryptImsiData
 * 功能描述: 将原始数据 rawData 先用 zlib 压缩再用 des-cbc 加密，
 *           然后把结果以十六进制字符串的形式保存到 cipherTextBuf 中
 * 参数描述:
 *           cipherTextBuf: 用于保存压缩加密后以十六进制字符串保存的结果数据
 *                          的数组，必须足够大以容纳最大的数据量，该数组中的内容是
 *                          用于填充到 MQTT 发送 IMSI 数据中的相应字段的，即原来
 *                          不压缩不加密的原始 IMSI 数据的字段
 *           cipherTextBufSize: cipherTextBuf 数组的大小
 *           rawData: 原始的数据，即原始的 IMSI 数据
 *           key: 用于 des 加密的 key，采用配置参数里面的 key，请自行获取后传给 key
 * 返回值: 成功则返回 cipherTextBuf 所包含的结果字符串的长度，大于 0;
 *         失败则返回负数
 */
int CompressAndEncryptImsiData(unsigned char *cipherTextBuf,
        size_t cipherTextBufSize, char *rawData, char *key);

/* 函数名: DecryptAndUncompressImsiData
 * 功能描述: 将 CompressAndEncryptImsiData() 函数中压缩加密后的十六进制字符串 cipherTextBuf
 *           解密在解压缩，还原原始的 IMSI 数据，该函数仅用于验证测试，项目中实际的解密和解压缩
 *           工作由中电慧安平台负责
 * 参数描述:
 *           plainTextBuf: 用于保存解密解压缩后的 IMSI 数据，必须足够大以容纳最大的原始 IMSI 数据，
 *                         可根据每条 IMSI 数据的大小乘以最大值 5000 来分配空间测试
 *           plainTextBufSize: plainTextBuf 数组的大小
 *           cipherHexString: 压缩并加密后的十六进制字符串，即 CompressAndEncryptImsiData() 函数中
 *                            的 cipherTextBuf
 *           key: 用于 des 解密的 key，与 CompressAndEncryptImsiData() 函数中的 key 参数一致
 * 返回值: 成功则返回 plainTextBuf 所包含的解密解压后字符串的长度，大于 0;
 *         失败则返回负数
 */
int DecryptAndUncompressImsiData(char *plainTextBuf, size_t plainTextBufSize,
        char *cipherHexString, char *key);

具体使用可参考 HandleImsiTest.c

HandleImsi.h, zconf.h 和 zlib.h 三个头文件都会用到，请保存到合适的位置

