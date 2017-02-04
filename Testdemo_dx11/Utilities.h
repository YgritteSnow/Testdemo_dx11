#pragma once

namespace Utilities
{
	template <typename T>
	void ReleaseCom(T&& x) {
		if (x) {
			x->Release();
			x = NULL;
		}
	}

	template <typename T>
	void ReleasePointer(T&& x) {
		if (x) {
			delete x;
			x = NULL;
		}
	}
}