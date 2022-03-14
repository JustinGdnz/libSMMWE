#pragma once

/// ...Mas cosas relacionadas con GMS
struct YYRValue
{
	double Value;
	int Unk;
	int Type;

	YYRValue() = default;
	YYRValue(double value)
	{
		Value = value;
		Unk = 0;
		Type = 0;
	}

	operator double() const
	{
		return Value;
	}
};