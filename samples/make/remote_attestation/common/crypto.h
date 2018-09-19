// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef OE_SAMPLES_ATTESTATION_ENC_CRYPTO_H
#define OE_SAMPLES_ATTESTATION_ENC_CRYPTO_H

#include <openenclave/enclave.h>
// Includes for mbedtls shipped with oe.
// Also add the following libraries to your linker command line:
// -loeenclave -lmbedcrypto -lmbedtls -lmbedx509
#include <mbedtls/config.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>
#include <mbedtls/sha256.h>
#include "log.h"

#define PUBLIC_KEY_SIZE 512

class Crypto
{
  private:
    mbedtls_ctr_drbg_context m_ctr_drbg_context;
    mbedtls_entropy_context m_entropy_context;
    mbedtls_pk_context m_rsa_context;
    uint8_t m_my_public_key[512];
    bool m_initialized;

    // Public key of another enclave.
    uint8_t m_other_enclave_pem_public_key[PUBLIC_KEY_SIZE];

  public:
    Crypto();
    ~Crypto();

    /**
     * Get this enclave's own public key
     */
    void RetrievePublicKey(uint8_t pem_public_key[512]);

    /**
     * Encrypt encrypts the given data using the given public key.
     * Used to encrypt data using the public key of another enclave.
    */
    bool Encrypt(
        const uint8_t* pem_public_key,
        const uint8_t* data,
        size_t size,
        uint8_t* encrypted_data,
        size_t* encrypted_data_size);

    /**
     * Decrypt decrypts the given data using current enclave's private key.
     * Used to receive encrypted data from another enclave.
     */
    bool Decrypt(
        const uint8_t* encrypted_data,
        size_t encrypted_data_size,
        uint8_t* data,
        size_t* data_size);

    // Public key of another enclave.
    uint8_t* get_2ndenclave_public_key()
    {
        return m_other_enclave_pem_public_key;
    }

    /**
     * Compute the sha256 hash of given data.
     */
    void Sha256(const uint8_t* data, size_t data_size, uint8_t sha256[32]);

  private:
    /**
     * Crypto demonstrates use of mbedtls within the enclave to generate keys
     * and perform encryption. In this sample, each enclave instance generates
     * an ephemeral 2048-bit RSA key pair and shares the public key with the
     * other instance. The other enclave instance then replies with data
     * encrypted to the provided public key.
     */

    /** InitializeMbedtls initializes the crypto module.
     */
    bool InitializeMbedtls(void);

    void CleanupMbedtls(void);
};

#endif // OE_SAMPLES_ATTESTATION_ENC_CRYPTO_H
