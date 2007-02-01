#include "BaseEffect.h"
#include <algorithm>
#include <utility>

using namespace mutalisk;
using namespace mutalisk::effects;

BaseEffect::Input& BaseEffect::allocInput()
{
#if defined(DEBUG) || defined(_DEBUG)
	clearInput(mInput);
#endif
	return mInput;
}

namespace {
	template <typename PortIterator, typename InputArray>
	bool validateInputPorts(PortIterator first, PortIterator last, InputArray const& inputs)
	{
		for (; first != last; ++first)
			if(inputs[*first] == 0)
				return false;
		return true;
	}
}

bool BaseEffect::validateInput(Input const& input, Request const& request)
{
#if defined(DEBUG) || defined(_DEBUG)
	{
		for(int q = 0; q < request.lightCountRange.first; ++q)
			if(input.lights[q].first == 0)
				return false;

		bool valid = 
			validateInputPorts(request.required.textures.begin(), request.required.textures.end(), input.textures) &
			validateInputPorts(request.required.matrices.begin(), request.required.matrices.end(), input.matrices) &
			validateInputPorts(request.required.vecs.begin(), request.required.vecs.end(), input.vecs) &
			validateInputPorts(request.required.floats.begin(), request.required.floats.end(), input.floats) &
			validateInputPorts(request.required.ints.begin(), request.required.ints.end(), input.ints);

		if(!valid)
			return false;
	}
#endif
	return true;
}

namespace {
	template <typename PortIterator>
	unsigned minimalSize(PortIterator first, PortIterator last)
	{
		if(first == last)
			return 0;

		return *std::max_element(first, last) + 1;
	}
}

void BaseEffect::allocInput(Input& input, Request const& request)
{
	unsigned textureCount = max(
		minimalSize(request.required.textures.begin(), 
			request.required.textures.end()),
		minimalSize(request.optional.textures.begin(), 
			request.optional.textures.end()));

	unsigned matrixCount = max(
		minimalSize(request.required.matrices.begin(), 
			request.required.matrices.end()),
		minimalSize(request.optional.matrices.begin(), 
			request.optional.matrices.end()));

	unsigned vecCount = minimalSize(request.required.vecs.begin(), 
			request.required.vecs.end());
	unsigned floatCount = minimalSize(request.required.floats.begin(), 
			request.required.floats.end());
	unsigned intCount = minimalSize(request.required.ints.begin(), 
			request.required.ints.end());

	input.textures.resize(max(textureCount, BaseEffect::MaxCount_nTexture));
	input.matrices.resize(max(matrixCount, BaseEffect::MaxCount_nMatrix));
	input.vecs.resize(max(vecCount, BaseEffect::MaxCount_nVec));
	input.floats.resize(floatCount);
	input.ints.resize(intCount);

	if(request.lightCountRange.second > 0)
		input.lights.resize(request.lightCountRange.second);

	ASSERT(validateInput(input, request));
}

void BaseEffect::clearInput(Input& input)
{
	std::fill(input.lights.begin(), input.lights.end(), std::make_pair(static_cast<data::scene::Light*>(0), MatrixT()));
	std::fill(input.textures.begin(), input.textures.end(), static_cast<TextureT*>(0));
	std::fill(input.matrices.begin(), input.matrices.end(), static_cast<MatrixT*>(0));
	std::fill(input.vecs.begin(), input.vecs.end(), VecT());
	std::fill(input.floats.begin(), input.floats.end(), 0.0f);
	std::fill(input.ints.begin(), input.ints.end(), 0);
}