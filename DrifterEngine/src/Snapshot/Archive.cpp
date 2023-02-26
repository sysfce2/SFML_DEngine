#include <pch.h>
#include "Archive.h"


namespace snapshot 
{
	OutputArchive::OutputArchive(cereal::BinaryOutputArchive& binary)
		: binary(&binary)
	{}


	InputArchive::InputArchive(cereal::BinaryInputArchive& binary)
		: binary(&binary)
	{}

	Archive::Archive(cereal::BinaryOutputArchive& binary_out)
	{
		setNull();
		this->binary_out = &binary_out;
	}

	Archive::Archive(cereal::BinaryInputArchive& binary_in)
	{
		setNull();
		this->binary_in = &binary_in;
	}

	void Archive::setNull()
	{
		binary_out = nullptr;
		binary_in = nullptr;
	}

}