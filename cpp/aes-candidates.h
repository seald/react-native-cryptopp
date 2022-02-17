#ifndef REACT_NATIVE_CRYPTOPP_AES_CANDIDATES_H
#define REACT_NATIVE_CRYPTOPP_AES_CANDIDATES_H

#include <string>
#include <jsi/jsilib.h>
#include <jsi/jsi.h>

#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/osrng.h"
#include "cryptopp/modes.h"
#include "cryptopp/cryptlib.h"

#include "utils.h"

using namespace facebook;
using namespace facebook::jsi::detail;

namespace rncryptopp {
    void aes_encrypt(jsi::Runtime &rt, std::string &result, const jsi::Value *args);

    void aes_decrypt(jsi::Runtime &rt, std::string &result, const jsi::Value *args);
}


#endif //REACT_NATIVE_CRYPTOPP_AES_CANDIDATES_H
