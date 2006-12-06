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

		unsigned int vertexCount;
		Vec3* positions;
		Vec3* normals;

		unsigned int weightsPerVertex;
		float* weights;
		unsigned short* boneIndices;

		unsigned int indexCount;
		unsigned short* indices;

		unsigned int boneCount;
		Bone* bones;
	};
}

#endif // MUTANT_SIMPLE_SKINNED_H_
