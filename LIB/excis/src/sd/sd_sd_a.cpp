﻿#include <basis/sys/logger.hpp>
#include <excis/sd.hpp>
#include <excis/exception.hpp>

namespace Ext {

	WinSD::WinSD(WinSD && rhs):
		m_sd(nullptr)
	{
		LogTrace();
		this->swap(rhs);
	}

	WinSD & WinSD::operator = (WinSD && rhs) {
		LogTrace();
		if (this != &rhs)
			this->swap(rhs);
//			WinSD(simstd::move(rhs)).swap(*this);
		return *this;
	}

	void WinSD::swap(WinSD & rhs) {
		LogTrace();
		using simstd::swap;
		swap(m_sd, rhs.m_sd);
	}

}
