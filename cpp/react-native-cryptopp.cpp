#include <iostream>
#include <sstream>

#include "react-native-cryptopp.h"
#include "hash-functions.h"

namespace rncryptopp {
    void install(jsi::Runtime &jsiRuntime) {
        /*
        Hash functions
        */
        auto sha1 = jsi::Function::createFromHostFunction(
                jsiRuntime,
                jsi::PropNameID::forAscii(jsiRuntime, "sha1"),
                2,
                [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args,
                   size_t count) -> jsi::Value {
                    std::string result;
                    rncryptopp::sha1(rt, result, args);
                    return jsi::Value(jsi::String::createFromUtf8(rt, result));
                }
        );
        auto sha2 = jsi::Function::createFromHostFunction(
                jsiRuntime,
                jsi::PropNameID::forAscii(jsiRuntime, "sha1"),
                2,
                [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args,
                   size_t count) -> jsi::Value {
                    std::string result;
                    rncryptopp::sha2(rt, result, args);
                    return jsi::Value(jsi::String::createFromUtf8(rt, result));
                }
        );
        auto sha3 = jsi::Function::createFromHostFunction(
                jsiRuntime,
                jsi::PropNameID::forAscii(jsiRuntime, "sha1"),
                2,
                [](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *args,
                   size_t count) -> jsi::Value {
                    std::string result;
                    rncryptopp::sha3(rt, result, args);
                    return jsi::Value(jsi::String::createFromUtf8(rt, result));
                }
        );

        jsi::Object SHA = jsi::Object(jsiRuntime);
        SHA.setProperty(jsiRuntime, "sha1", std::move(sha1));
        SHA.setProperty(jsiRuntime, "sha2", std::move(sha2));
        SHA.setProperty(jsiRuntime, "sha3", std::move(sha3));

        jsi::Object module = jsi::Object(jsiRuntime);
        module.setProperty(jsiRuntime, "SHA", std::move(SHA));

        jsiRuntime.global().setProperty(jsiRuntime, "cryptoppModule", std::move(module));
    }

    void cleanup() {
    }
}
