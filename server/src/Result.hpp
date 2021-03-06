#pragma once
#include <string>
#include <optional>

template<typename T = int>
struct Result {
    bool ok;
    std::optional<T> data;
    std::optional<std::string> errorMsg;

    static Result<T> success(T data) {
        return Result<T>{true, data};
    }

    static Result<T> success() {
        return Result<T>{true};
    }

    static Result<T> fail(std::string errorMsg){
        return Result<T>{false, {}, errorMsg};
    }
};
