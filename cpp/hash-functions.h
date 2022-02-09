#ifndef CRYPTOPPEXAMPLE_HASH_FUNCTIONS_H
#define CRYPTOPPEXAMPLE_HASH_FUNCTIONS_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

using namespace facebook;

namespace rncryptopp {
    void sha1(jsi::Runtime &rt, std::string &result, const jsi::Value *args);

    void sha2(jsi::Runtime &rt, std::string &result, const jsi::Value *args);

    void sha3(jsi::Runtime &rt, std::string &result, const jsi::Value *args);
}


#endif //CRYPTOPPEXAMPLE_HASH_FUNCTIONS_H
