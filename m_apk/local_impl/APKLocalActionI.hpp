#pragma once

#ifndef CSIGNAPK_APKLOCALACTIONI_HPP
#define CSIGNAPK_APKLOCALACTIONI_HPP

namespace m
{

inline namespace apk
{

template<typename _Cond, typename _CondValue>
class APKLocalActionI
{
public:
    virtual ~APKLocalActionI();

    virtual void start(const Jchar *path) = 0;

    virtual Jbool select(std::vector<_CondValue> &array) = 0;

    virtual Jbool select(_Cond cond, _CondValue &value, std::vector<_CondValue> &array) = 0;

    virtual Jbool insert(_CondValue &value) = 0;

    virtual Jbool update(_Cond cond, _CondValue &value, _CondValue &write) = 0;

    virtual Jbool remove() = 0;

    virtual Jbool remove(_Cond cond, _CondValue &value) = 0;
};

template<typename _Cond, typename _CondValue>
APKLocalActionI<_Cond, _CondValue>::~APKLocalActionI() = default;

}

}

#endif //CSIGNAPK_APKLOCALACTIONI_HPP
