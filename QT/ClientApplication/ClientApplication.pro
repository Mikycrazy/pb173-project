#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T11:07:52
#
#-------------------------------------------------

QT       += core
QT       += network


QT       -= gui

TARGET = ClientApplication
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cpp \
    ClientApplicationTests.cpp \
    NetworkManager.cpp \
    Client.cpp \
    logger.cpp \
    myserver.cpp \
    CryptoManager.cpp \
    Crypto/rsa.c \
    Crypto/sha256.c \
    Crypto/aesni.c \
    Crypto/arc4.c \
    Crypto/asn1parse.c \
    Crypto/asn1write.c \
    Crypto/base64.c \
    Crypto/bignum.c \
    Crypto/blowfish.c \
    Crypto/camellia.c \
    Crypto/ccm.c \
    Crypto/certs.c \
    Crypto/cipher.c \
    Crypto/cipher_wrap.c \
    Crypto/ctr_drbg.c \
    Crypto/debug.c \
    Crypto/des.c \
    Crypto/dhm.c \
    Crypto/ecdh.c \
    Crypto/ecdsa.c \
    Crypto/ecp.c \
    Crypto/ecp_curves.c \
    Crypto/error.c \
    Crypto/gcm.c \
    Crypto/havege.c \
    Crypto/hmac_drbg.c \
    Crypto/md.c \
    Crypto/md2.c \
    Crypto/md4.c \
    Crypto/md5.c \
    Crypto/md_wrap.c \
    Crypto/memory_buffer_alloc.c \
    Crypto/net.c \
    Crypto/oid.c \
    Crypto/padlock.c \
    Crypto/pbkdf2.c \
    Crypto/pem.c \
    Crypto/pk.c \
    Crypto/pk_wrap.c \
    Crypto/pkcs11.c \
    Crypto/pkcs12.c \
    Crypto/pkcs5.c \
    Crypto/pkparse.c \
    Crypto/pkwrite.c \
    Crypto/platform.c \
    Crypto/ripemd160.c \
    Crypto/sha1.c \
    Crypto/sha512.c \
    Crypto/ssl_cache.c \
    Crypto/ssl_ciphersuites.c \
    Crypto/ssl_cli.c \
    Crypto/ssl_srv.c \
    Crypto/ssl_tls.c \
    Crypto/threading.c \
    Crypto/timing.c \
    Crypto/version.c \
    Crypto/version_features.c \
    Crypto/x509.c \
    Crypto/x509_create.c \
    Crypto/x509_crl.c \
    Crypto/x509_crt.c \
    Crypto/x509_csr.c \
    Crypto/x509write_crt.c \
    Crypto/x509write_csr.c \
    Crypto/xtea.c \
    Crypto/aes.c

HEADERS += \
    Client.h \
    CryptoManager.h \
    NetworkManager.h \
    catch.hpp \
    Opcodes.h \
    logger.h \
    myserver.h \
    Crypto/polarssl/aes.h \
    Crypto/polarssl/aesni.h \
    Crypto/polarssl/arc4.h \
    Crypto/polarssl/asn1.h \
    Crypto/polarssl/asn1write.h \
    Crypto/polarssl/base64.h \
    Crypto/polarssl/bignum.h \
    Crypto/polarssl/blowfish.h \
    Crypto/polarssl/bn_mul.h \
    Crypto/polarssl/camellia.h \
    Crypto/polarssl/ccm.h \
    Crypto/polarssl/certs.h \
    Crypto/polarssl/check_config.h \
    Crypto/polarssl/cipher.h \
    Crypto/polarssl/cipher_wrap.h \
    Crypto/polarssl/compat-1.2.h \
    Crypto/polarssl/config.h \
    Crypto/polarssl/ctr_drbg.h \
    Crypto/polarssl/debug.h \
    Crypto/polarssl/des.h \
    Crypto/polarssl/dhm.h \
    Crypto/polarssl/ecdh.h \
    Crypto/polarssl/ecdsa.h \
    Crypto/polarssl/ecp.h \
    Crypto/polarssl/entropy.h \
    Crypto/polarssl/entropy_poll.h \
    Crypto/polarssl/error.h \
    Crypto/polarssl/gcm.h \
    Crypto/polarssl/havege.h \
    Crypto/polarssl/hmac_drbg.h \
    Crypto/polarssl/md.h \
    Crypto/polarssl/md2.h \
    Crypto/polarssl/md4.h \
    Crypto/polarssl/md5.h \
    Crypto/polarssl/md_wrap.h \
    Crypto/polarssl/memory.h \
    Crypto/polarssl/memory_buffer_alloc.h \
    Crypto/polarssl/net.h \
    Crypto/polarssl/oid.h \
    Crypto/polarssl/openssl.h \
    Crypto/polarssl/padlock.h \
    Crypto/polarssl/pbkdf2.h \
    Crypto/polarssl/pem.h \
    Crypto/polarssl/pk.h \
    Crypto/polarssl/pk_wrap.h \
    Crypto/polarssl/pkcs11.h \
    Crypto/polarssl/pkcs12.h \
    Crypto/polarssl/pkcs5.h \
    Crypto/polarssl/platform.h \
    Crypto/polarssl/ripemd160.h \
    Crypto/polarssl/rsa.h \
    Crypto/polarssl/sha1.h \
    Crypto/polarssl/sha256.h \
    Crypto/polarssl/sha512.h \
    Crypto/polarssl/ssl.h \
    Crypto/polarssl/ssl_cache.h \
    Crypto/polarssl/ssl_ciphersuites.h \
    Crypto/polarssl/threading.h \
    Crypto/polarssl/timing.h \
    Crypto/polarssl/version.h \
    Crypto/polarssl/x509.h \
    Crypto/polarssl/x509_crl.h \
    Crypto/polarssl/x509_crt.h \
    Crypto/polarssl/x509_csr.h \
    Crypto/polarssl/xtea.h

