#include "public-key.h"

#define PUT_MESSAGE 2
#define THROW_EXCEPTION 16

namespace rncryptopp::RSA
{
    void generateKeyPair(jsi::Runtime &rt, const jsi::Value *args, jsi::Object &result)
    {
        int size;
        if (!valueToInt(args[0], &size))
            throwJSError(rt,
                         "RNCryptopp: RSA generateKeyPair size is not a number");

        // Generate Parameters
        AutoSeededRandomPool rng;
        InvertibleRSAFunction key_params;
        key_params.GenerateRandomWithKeySize(rng, size);

        // Create Keys
        CryptoPP::RSA::PrivateKey privateKey(key_params);
        CryptoPP::RSA::PublicKey publicKey(key_params);

        const Integer &n = key_params.GetModulus();
        std::stringstream n_stream;
        n_stream << n;

        const Integer &p = key_params.GetPrime1();
        std::stringstream p_stream;
        p_stream << p;

        const Integer &q = key_params.GetPrime2();
        std::stringstream q_stream;
        q_stream << q;

        const Integer &d = key_params.GetPrivateExponent();
        std::stringstream d_stream;
        d_stream << d;

        const Integer &e = key_params.GetPublicExponent();
        std::stringstream e_stream;
        e_stream << e;

        std::string pem_public;
        StringSink pem_public_sink(pem_public);
        PEM_Save(pem_public_sink, publicKey);

        std::string pem_private;
        StringSink pem_private_sink(pem_private);
        PEM_Save(pem_private_sink, privateKey);

        jsi::Object params = jsi::Object(rt);
        params.setProperty(rt, "n", n_stream.str());
        params.setProperty(rt, "p", p_stream.str());
        params.setProperty(rt, "q", q_stream.str());
        params.setProperty(rt, "d", d_stream.str());
        params.setProperty(rt, "e", e_stream.str());

        result.setProperty(rt, "public", pem_public);
        result.setProperty(rt, "private", pem_private);
        result.setProperty(rt, "parameters", params);
    }

    void encrypt(jsi::Runtime &rt, const jsi::Value *args, std::string *result)
    {
        std::string data;
        if (!binaryLikeValueToString(rt, args[0], &data))
            throwJSError(rt, "RNCryptopp: RSA encrypt data is not a string");

        std::string publicKeyString;
        if (!stringValueToString(rt, args[1], &publicKeyString))
            throwJSError(rt, "RNCryptopp: RSA encrypt publicKey is not a string");

        std::string encryptScheme;
        if (!stringValueToString(rt, args[2], &encryptScheme))
            throwJSError(rt, "RNCryptopp: RSA encrypt scheme is not a string");

        StringSource PKeyStringSource(publicKeyString, true);
        CryptoPP::RSA::PublicKey publicKey;
        PEM_Load(PKeyStringSource, publicKey);

        AutoSeededRandomPool rng;

        if (encryptScheme == "OAEP_SHA1")
        {
            RSAES<OAEP<SHA1>>::Encryptor e(publicKey);
            StringSource(data,
                         true,
                         new PK_EncryptorFilter(rng,
                                                e,
                                                new Base64Encoder(new StringSink(*result))));
        }
        else if (encryptScheme == "OAEP_SHA256")
        {
            RSAES<OAEP<SHA256>>::Encryptor e(publicKey);
            StringSource(data,
                         true,
                         new PK_EncryptorFilter(rng,
                                                e,
                                                new Base64Encoder(new StringSink(*result))));
        }
        else if (encryptScheme == "PKCS1v15")
        {
            RSAES<PKCS1v15>::Encryptor e(publicKey);
            StringSource(data,
                         true,
                         new PK_EncryptorFilter(rng,
                                                e,
                                                new Base64Encoder(new StringSink(*result))));
        }
        else
        {
            throwJSError(rt, "RNCryptopp: RSA encrypt invalid scheme");
        }
    }

    void decrypt(jsi::Runtime &rt, const jsi::Value *args, std::string *result)
    {
        std::string data;
        if (!binaryLikeValueToString(rt, args[0], &data, 2, 0))
            throwJSError(rt, "RNCryptopp: RSA decrypt data is not a string");

        std::string privateKeyString;
        if (!stringValueToString(rt, args[1], &privateKeyString))
            throwJSError(rt, "RNCryptopp: RSA decrypt privateKey is not a string");

        std::string encryptScheme;
        if (!stringValueToString(rt, args[2], &encryptScheme))
            throwJSError(rt, "RNCryptopp: RSA decrypt scheme is not a string");

        StringSource PKeyStringSource(privateKeyString, true);
        CryptoPP::RSA::PrivateKey privateKey;
        PEM_Load(PKeyStringSource, privateKey);

        AutoSeededRandomPool rng;

        if (encryptScheme == "OAEP_SHA1")
        {
            RSAES<OAEP<SHA1>>::Decryptor e(privateKey);
            StringSource(data,
                         true,
                         new PK_DecryptorFilter(rng,
                                                e,
                                                new StringSink(*result)));
        }
        else if (encryptScheme == "OAEP_SHA256")
        {
            RSAES<OAEP<SHA256>>::Decryptor e(privateKey);
            StringSource ss1(data,
                             true,
                             new PK_DecryptorFilter(rng,
                                                    e,
                                                    new StringSink(*result)));
        }
        else if (encryptScheme == "PKCS1v15")
        {
            RSAES<PKCS1v15>::Decryptor e(privateKey);
            StringSource(data,
                         true,
                         new PK_DecryptorFilter(rng,
                                                e,
                                                new StringSink(*result)));
        }
        else
        {
            throwJSError(rt, "RNCryptopp: RSA encrypt invalid scheme");
        }
    }

    void sign(jsi::Runtime &rt, const jsi::Value *args, std::string *result)
    {
        std::string data;
        if (!binaryLikeValueToString(rt, args[0], &data))
            throwJSError(rt, "RNCryptopp: RSA sign data is not a string");

        std::string privateKeyString;
        if (!stringValueToString(rt, args[1], &privateKeyString))
            throwJSError(rt, "RNCryptopp: RSA sign privateKey is not a string");

        std::string signScheme;
        if (!stringValueToString(rt, args[2], &signScheme))
            throwJSError(rt, "RNCryptopp: RSA decrypt scheme is not a string");

        StringSource PKeyStringSource(privateKeyString, true);
        CryptoPP::RSA::PrivateKey privateKey;
        PEM_Load(PKeyStringSource, privateKey);

        AutoSeededRandomPool rng;

        if (signScheme == "PKCS1v15_SHA1")
        {
            RSASS<PKCS1v15, SHA1>::Signer signer(privateKey);
            StringSource(data,
                         true,
                         new SignerFilter(rng,
                                          signer,
                                          new Base64Encoder(new StringSink(*result))));
        }
        else if (signScheme == "PKCS1v15_SHA256")
        {
            RSASS<PKCS1v15, SHA256>::Signer signer(privateKey);
            StringSource(data,
                         true,
                         new SignerFilter(rng,
                                          signer,
                                          new Base64Encoder(new StringSink(*result))));
        }
        else if (signScheme == "PSSR_SHA1")
        {
            RSASS<PSSR, SHA1>::Signer signer(privateKey);
            StringSource(data,
                         true,
                         new SignerFilter(rng,
                                          signer,
                                          new Base64Encoder(new StringSink(*result)), true));
        }
        else if (signScheme == "PSSR_SHA256")
        {
            RSASS<PSSR, SHA256>::Signer signer(privateKey);
            StringSource(data,
                         true,
                         new SignerFilter(rng,
                                          signer,
                                          new Base64Encoder(new StringSink(*result)), true));
        }
        else if (signScheme == "PSSR_Whirlpool")
        {
            RSASS<PSSR, Whirlpool>::Signer signer(privateKey);
            StringSource(data,
                         true,
                         new SignerFilter(rng,
                                          signer,
                                          new Base64Encoder(new StringSink(*result)), true));
        }
        else
        {
            throwJSError(rt, "RNCryptopp: RSA sign invalid scheme");
        }
    }

    void verify(jsi::Runtime &rt, const jsi::Value *args, bool *result)
    {
        std::string data;
        if (!binaryLikeValueToString(rt, args[0], &data))
            throwJSError(rt, "RNCryptopp: RSA verify data is not a string");

        std::string publicKeyString;
        if (!stringValueToString(rt, args[1], &publicKeyString))
            throwJSError(rt, "RNCryptopp: RSA verify publicKey is not a string");

        std::string signScheme;
        if (!binaryLikeValueToString(rt, args[2], &signScheme, 0, 0))
            throwJSError(rt, "RNCryptopp: RSA verify signature scheme is not a string");

        std::string signature;
        if (!binaryLikeValueToString(rt, args[3], &signature, 2, 0))
            throwJSError(rt, "RNCryptopp: RSA verify signature is not a string");

        StringSource PKeyStringSource(publicKeyString, true);
        CryptoPP::RSA::PublicKey publicKey;
        PEM_Load(PKeyStringSource, publicKey);

        if (signScheme == "PKCS1v15_SHA1")
        {
            RSASS<PKCS1v15, SHA1>::Verifier verifier(publicKey);
            *result = verifier.VerifyMessage((const byte *)data.data(), data.size(),
                                             (const byte *)signature.data(), signature.size());
        }
        else if (signScheme == "PKCS1v15_SHA256")
        {
            RSASS<PKCS1v15, SHA256>::Verifier verifier(publicKey);
            *result = verifier.VerifyMessage((const byte *)data.data(), data.size(),
                                             (const byte *)signature.data(), signature.size());
        }
        else
        {
            throwJSError(rt, "RNCryptopp: RSA verify invalid scheme");
        }
    }

    void recover(jsi::Runtime &rt, const jsi::Value *args, std::string *result)
    {
        std::string signature;
        if (!binaryLikeValueToString(rt, args[0], &signature, 2, 0))
            throwJSError(rt, "RNCryptopp: RSA recover signature is not a string");

        std::string publicKeyString;
        if (!stringValueToString(rt, args[1], &publicKeyString))
            throwJSError(rt, "RNCryptopp: RSA recover publicKey is not a string");

        std::string signScheme;
        if (!binaryLikeValueToString(rt, args[2], &signScheme, 0, 0))
            throwJSError(rt, "RNCryptopp: RSA recover signature scheme is not a string");

        StringSource PKeyStringSource(publicKeyString, true);
        CryptoPP::RSA::PublicKey publicKey;
        PEM_Load(PKeyStringSource, publicKey);

        // try catch
        try
        {
            if (signScheme == "PSSR_SHA1")
            {
                RSASS<PSSR, SHA1>::Verifier verifier(publicKey);
                StringSource(signature, true,
                             new SignatureVerificationFilter(
                                 verifier,
                                 new StringSink(*result),
                                 THROW_EXCEPTION | PUT_MESSAGE));
            }
            else if (signScheme == "PSSR_SHA256")
            {
                RSASS<PSSR, SHA256>::Verifier verifier(publicKey);
                StringSource(signature, true,
                             new SignatureVerificationFilter(
                                 verifier,
                                 new StringSink(*result),
                                 THROW_EXCEPTION | PUT_MESSAGE));
            }
            else if (signScheme == "PSSR_Whirlpool")
            {
                RSASS<PSSR, Whirlpool>::Verifier verifier(publicKey);
                StringSource(signature, true,
                             new SignatureVerificationFilter(
                                 verifier,
                                 new StringSink(*result),
                                 THROW_EXCEPTION | PUT_MESSAGE));
            }
            else
            {
                // Will not be caught by std::exception &e
                throwJSError(rt, "RNCryptopp: RSA recover invalid scheme");
            }
        }
        catch (const std::exception &e)
        {
            throwJSError(rt, "RNCryptopp: RSA signatures do not match");
        }
    }
}