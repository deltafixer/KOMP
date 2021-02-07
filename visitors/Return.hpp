#pragma once

#include <exception>
#include <optional>

class Return : public std::exception
{
public:
    std::optional<int> result;

    Return()
    {
        this->result = {};
    }

    Return(int res)
    {
        this->result = res;
    }
};