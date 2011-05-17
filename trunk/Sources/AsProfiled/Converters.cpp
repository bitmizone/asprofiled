#include "StdAfx.h"
#include "Converters.h"

std::wstring Converters::IntToString(int value) {
		std::wostringstream stream;
		stream << value;
		return stream.str();
	}