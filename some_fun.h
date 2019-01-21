#ifndef SOME_FUN_H
#define SOME_FUN_H

#include <QByteArray>
#include <QStack>
#include <QString>
#include <QTreeView>
#include <stdint.h>
#include <tuple>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)                                                                                                                               \
    {                                                                                                                                                \
        if ((p) != nullptr) {                                                                                                                        \
            delete (p);                                                                                                                              \
            (p) = nullptr;                                                                                                                           \
        }                                                                                                                                            \
    }
#endif

#ifndef COMMUNICATION_TIME_OUT_S
#define COMMUNICATION_TIME_OUT_S (2)
#endif

QByteArray QString2Hex(const QString &str);
QString ByteArrayToHexStr(const QByteArray &data);
void sleepMsec(int msec);

inline uint32_t ByteArrayToUint32(const QByteArray &data)
{
    uint32_t vn = (uint8_t)data[3] << 24 | (uint8_t)data[2] << 16 | (uint8_t)data[1] << 8 | (uint8_t)data[0];
    return vn;
}

#endif // SOME_FUN_H
