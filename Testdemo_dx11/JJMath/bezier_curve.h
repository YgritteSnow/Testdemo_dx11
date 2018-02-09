#pragma once

#include "template_func.h"
#include "template_const.h"

namespace JJMATH
{
	class BezierCurve_linear
	{
	public:
		BezierCurve_linear(float f0, float f1) :p0(f0), p1(f1) {}
		JDOUBLE getBezierValue(JDOUBLE t) {
			return p0 * (1 - t) + p1 * (t);
		}
	private:
		JDOUBLE p0, p1;
	};

	class BezierCurve_quadric
	{
	public:
		BezierCurve_quadric(float f0, float f1, float f2) :p0(f0), p1(f1), p2(f2) {}
		JDOUBLE getBezierValue(JDOUBLE t) {
			return (1 - t)*(1 - t) * p0
				+ 2 * (1 - t) * t * p1
				+ t * t * p2;
		}
	private:
		JDOUBLE p0, p1, p2;
	};

	class BezierCurve_cubic
	{
	public:
		BezierCurve_cubic(float f0, float f1, float f2, float f3) :p0(f0), p1(f1), p2(f2), p3(f3) {}
		JDOUBLE getBezierValue(JDOUBLE t) {
			return (1 - t)*(1 - t) * p0
				+ 2 * (1 - t) * t * p1
				+ t * t * p2;
		}
	private:
		JDOUBLE p0, p1, p2, p3;
	};

	// bezier curve of degree n
	template <int n>
	class BezierCurve
	{
	};
}