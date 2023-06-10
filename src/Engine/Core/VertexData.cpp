#include <Engine/Core/VertexData.h>

VertexData::~VertexData ()
{
	for (uint32_t i = 0; i < size_f; i++) {
        if (f[i])
		    delete[] f[i];
        if (data[i])
		    delete[] data[i];
	}

	delete[] f;
	delete[] data;
}
