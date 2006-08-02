#ifndef MUTANT_SIMPLE_SKINNED_H_
#define MUTANT_SIMPLE_SKINNED_H_


namespace mutant
{
	struct simple_skinned
	{
		struct Vec3 {
			float data[3];
		};
		struct Mat16 {
			float data[16];
		};
		struct Bone
		{
			Mat16 matrix;
			std::string	name;
		};

		size_t vertexCount;
		Vec3* positions;

		size_t weightsPerVertex;
		float* weights;
		unsigned short* boneIndices;

		size_t indexCount;
		unsigned short* indices;

		size_t boneCount;
		Bone* bones;
	};
}

#endif // MUTANT_SIMPLE_SKINNED_H_
