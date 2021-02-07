#pragma once

#include <exception>
#include <optional>

class Return : public std::exception
{
public:
    std::optional<Object *> result;

    Return()
    {
        this->result = {};
    }

    Return(Object *res)
    {
        this->result = res;
    }
};