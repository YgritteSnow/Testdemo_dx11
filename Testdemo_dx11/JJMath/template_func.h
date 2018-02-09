#pragma once

#define USE_TEMPLATE 1
#define USE_CONSTEXPR 0

namespace JJMATH {
	typedef double JDOUBLE;
	typedef int JINT;
}

// i th Bernstein basis polynomials of degree n
namespace JJMATH 
{
	template <JINT n, JINT i>
	JDOUBLE TBernsteinBasis(t) {
		return _CBernsteinBasis<n, i>::func(t);
	}

	template <int n, JINT i>
	struct _CBernsteinBasis {
		static JDOUBLE func(JDOUBLE t) {
			return
#if USE_CONSTEXPR
				combinationNumber(n, i)
				* power(n - i, 1 - t) * power(i, t);
#else
				CCombinationNumber<n, i>::Value
				* TPower<n - i>(1 - t) * TPower<i>(t);
#endif
		}
	};
}

// combination number
// TCombinationNumber(n, i) = n! / (n-i)! / i!
namespace JJMATH
{
#if USE_CONSTEXPR
	constexpr int combinationNumber(int n, int i) {
		// use: c(n, m) = c(n - 1, m - 1) + c(n - 1, m)
		return (n == i) ? 1 : (
			(i == 0) ? 1 :
			combinationNumber(n - 1, i - 1) + combinationNumber(n - 1, i));
	}
#else
	template <JINT n, JINT i>
	struct CCombinationNumber {
		enum {
			Value = CCombinationNumber<n - 1, i - 1>::Value + CCombinationNumber<n - 1, i>::Value
		};
	};

	template <JINT n>
	struct CCombinationNumber<n, 0> {
		enum {
			Value = 1;
		};
	};

	template <JINT n>
	struct CCombinationNumber<n, n> {
		enum {
			Value = 1;
		};
	};

	template <>
	struct CCombinationNumber<0, 0> {
		enum {
			Value = 1
		};
	};
#endif
}

// factorial
// TFactorial(n) = n!
namespace JJMATH
{
#if USE_CONSTEXPR
	JINT fractorial(int n) {
		return n * fractorial(n - 1);
	}
#else
	template <JINT n>
	struct CFactorial {
		enum { Value = CFactorial<n - 1> * n };
	};

	template <>
	struct CFactorial<0> {
		enum { Value = 1 };
	};
#endif
}

// TPower(t) = t^n
namespace JJMATH
{
#if USE_CONSTEXPR
	JDOUBLE power(n, t) {
		if (n == 1)return t;
		if (n == 0)return 0;
		JDOUBLE halfpower = power(n / 2, t);
		return halfpower * halfpower * ((n % 2) ? t : 1);
	}
#else
	template <JINT n>
	JDOUBLE TPower(JDOUBLE t) {
		return _CPower<n>::func(t);
	}

	template <JINT n>
	struct _CPower {
		static JDOUBLE func(JDOUBLE x) {
			return _CPower<n / 2>::func(x)
				* _CPower<n / 2>::func(x)
				* _CPower<n % 2>;
		}
	};
	template <>
	struct _CPower<0> {
		static JDOUBLE func(JDOUBLE x) {
			return 1;
		}
	};
	template <>
	struct _CPower<1> {
		static JDOUBLE func(JDOUBLE x) {
			return x;
		}
	};
#endif
}