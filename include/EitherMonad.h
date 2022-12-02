#pragma once

#include "Either.h"
#include "MonadUtils.h"
#include "Error.h"


template <typename TLeft, typename TRight, typename F>
auto operator>>=(const Either<TLeft, TRight>& ei, F f)
	-> decltype(f(std::declval<const TRight>()))
{
	return ei.isLeft() ? *fromLeft(ei) : f(*fromRight(ei));
}

template<typename Stream, typename TLeft, typename TRight>
Stream& operator<<(Stream& ss, const Either<TLeft, TRight>& x)
{
    if (x.isRight()) { return ss << *fromRight(x); }
    else { return ss << (*fromLeft(x)).toStr(); }
}

template <typename T>
auto Do(T x) { return mreturn<Either, Error, T>(x); }

template <typename T, typename... Args>
auto _Do(const T& x, const Args&... args)
{
	return (..., ( Do(x) >>= Either<Error, Args>(args)) );
}

