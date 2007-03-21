#ifndef NEWAGE_RENDERER_H_
#define NEWAGE_RENDERER_H_

struct InstanceInput
{
	enum { MaxLights = 8 };
	enum { RequiredMatrices = BaseEffect::MaxCount_nMatrix };
	MatrixT			geometryMatrices[RequiredMatrices];
};

struct RenderBlock
{
	unsigned				instanceIndex;
	unsigned				surfaceIndex;
	BaseEffect*				fx;
	RenderableMesh const*	mesh;
	unsigned				subset;

	unsigned				slice;
	float					cameraDistanceSq;
	bool					hasZPass;
};

struct findVisibleActors
{
	RenderContextT& rc;
	findVisibleActors(RenderContextT& rc_, int) : rc(rc_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& visibleActors)
	{
		visibleActors.reserve(std::distance(first, last));
		for(; first != last; ++first)
			if(first->active)
				visibleActors.push_back(first);
//		std::copy(first, last, visibleActors.begin());
	}
};

struct blastInstanceInputs
{
	RenderableSceneT const& scene;
	RenderContextT const& rc;

	blastInstanceInputs(RenderableSceneT const& scene_, RenderContextT const& rc_) : scene(scene_), rc(rc_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& instanceInputs)
	{
		gatherSceneLights();

		MatrixT nativeMatrix;
		instanceInputs.resize(std::distance(first, last));
		for(unsigned block = 0; first != last; ++first, ++block)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			InstanceInput& instanceInput = instanceInputs[block];
			gatherInstanceLights();

			ASSERT(actor.id >= 0 && actor.id < this->scene.mState.actor2XformIndex.size());
			toNative(nativeMatrix, this->scene.mState.matrices[this->scene.mState.actor2XformIndex[actor.id]]);

			setWorldMatrix(instanceInput.geometryMatrices, rc, nativeMatrix);
		}
	}

	void gatherSceneLights() { /* @TBD:*/ }
	void gatherInstanceLights() { /* @TBD:*/ }
};

struct blastSurfaceInputs
{
	RenderableSceneT const& scene;
	blastSurfaceInputs(RenderableSceneT const& scene_, int) : scene(scene_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o) { operator()(c.begin(), c.end(), o); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& surfaceInputs)
	{
		surfaceInputs.reserve(std::distance(first, last));
		
		for(unsigned block = 0; first != last; ++first)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			ASSERT(!actor.materials.empty());
			surfaceInputs.resize(surfaceInputs.size() + actor.materials.size());
			for(unsigned q = 0; q < actor.materials.size(); ++q, ++block)
			{
				BaseEffect::Input::Surface& surfaceInput = surfaceInputs[block];
				toNative(surfaceInput, scene, actor.materials[q].shaderInput);
			}
		}
	}
};

struct blastRenderBlocks
{
	RenderableSceneT const& scene;
	Vec3 cameraPos;

	blastRenderBlocks(RenderableSceneT const& scene_, Vec3 cameraPos_) : scene(scene_), cameraPos(cameraPos_) {}

	template <typename Container, typename Out>
	void operator()(Container& c, Out& o1, Out& o2, Out& o3, Out& o4) { 
		operator()(c.begin(), c.end(), o1, o2, o3, o4); }

	template <typename In, typename Out>
	void operator()(In first, In last, Out& backBlocks, Out& opaqueBlocks, Out& transparentBlocks, Out& foreBlocks)
	{
		opaqueBlocks.reserve(std::distance(first, last));
		transparentBlocks.reserve(std::distance(first, last));
		
		unsigned actorIt = 0;
		unsigned surfaceIt = 0;
		for(; first != last; ++first, ++actorIt)
		{
			ASSERT(*first);
			mutalisk::data::scene::Actor const& actor = **first;

			RenderableMesh const* mesh = this->scene.mResources.meshes[actor.meshIndex].renderable.get();
			float cameraDistanceSq = calcCameraDistanceSq(
				this->scene.mState.matrices[this->scene.mState.actor2XformIndex[actor.id]].Move);

			ASSERT(!actor.materials.empty());
			/*renderBlocks.resize(renderBlocks.size() + actor.materials.size());
			for(unsigned q = 0; q < actor.materials.size(); ++q, ++blockIt)
			{
				RenderBlock& renderBlock = renderBlocks[blockIt];
				renderBlock.instanceIndex = actorIt;
				renderBlock.surfaceIndex = blockIt;
				renderBlock.fx = mutalisk::effects::getByIndex(actor.materials[q].shaderIndex);
				renderBlock.mesh = mesh;
				renderBlock.subset = q;
				renderBlock.cameraDistanceSq = cameraDistanceSq;
			}*/
			for(unsigned q = 0; q < actor.materials.size(); ++q, ++surfaceIt)
			{
				typedef mutalisk::data::shader_fixed Shader;
				Shader::ZBufferOp zBufferOp = actor.materials[q].shaderInput.zBufferOp;

				RenderBlock renderBlock;
				renderBlock.instanceIndex = actorIt;
				renderBlock.surfaceIndex = surfaceIt;
				renderBlock.fx = mutalisk::effects::getByIndex(actor.materials[q].shaderIndex);
				renderBlock.mesh = mesh;
				renderBlock.subset = q;
				renderBlock.slice = actor.slice;
				renderBlock.cameraDistanceSq = cameraDistanceSq;
				renderBlock.hasZPass = (zBufferOp == Shader::zboTwoPassReadWrite);

				if(zBufferOp == Shader::zboNone)
				{
					foreBlocks.push_back(renderBlock);
				}
				else
				{
					if((zBufferOp & Shader::zboWrite) || zBufferOp == Shader::zboTwoPassReadWrite)
					{
						opaqueBlocks.push_back(renderBlock);
					}
					if(!(zBufferOp & Shader::zboWrite) || zBufferOp == Shader::zboTwoPassReadWrite)
					{
						transparentBlocks.push_back(renderBlock);
					}
				}
			}
		}
	}

	float calcCameraDistanceSq(Vec3 const& pos) const
	{
		Vec3 d;
		Vec3_sub(&d, const_cast<Vec3*>(&this->cameraPos), const_cast<Vec3*>(&pos));
		return Vec3_dot(&d, &d);
	}
};

struct sortRenderBlocks
{
	template <typename T>
	struct BackToFront : public std::less<T>
	{
		bool operator()(T const& l, T const& r) const
		{
			if(l.slice > r.slice)
				return true;
			if(l.slice == r.slice)
				return (l.cameraDistanceSq > r.cameraDistanceSq);
			return false;
		}
	};
	template <typename T>
	struct FrontToBack : public std::less<T>
	{
		bool operator()(T const& l, T const& r) const
		{
			if(l.slice < r.slice)
				return true;
			if(l.slice == r.slice)
				return (l.cameraDistanceSq < r.cameraDistanceSq);
			return false;
		}
	};

	template <typename Container>
	void operator()(Container& c, bool backToFront = true) { operator()(c.begin(), c.end(), backToFront); }

	template <typename In>
	void operator()(In first, In last, bool backToFront = true)
	{
		if(backToFront)
			std::sort(first, last, BackToFront<typename In::value_type>());
		else
			std::sort(first, last, FrontToBack<typename In::value_type>());
	}
};

struct drawRenderBlocks
{
	RenderContextT& rc;
	RenderableSceneT const& scene;
	InstanceInput const* instanceInputs; size_t instanceInputCount;
	BaseEffect::Input::Surface const* surfaceInputs; size_t surfaceInputCount;

	drawRenderBlocks(RenderContextT& rc_, RenderableSceneT const& scene_, 
		InstanceInput const* instanceInputs_, size_t instanceInputCount_,
		BaseEffect::Input::Surface const* surfaceInputs_, size_t surfaceInputCount_)
		: rc(rc_), scene(scene_)
		, instanceInputs(instanceInputs_), instanceInputCount(instanceInputCount_)
		, surfaceInputs(surfaceInputs_), surfaceInputCount(surfaceInputCount_) {}

	typedef BaseEffect::Input::BufferControl	ControlT;

	template <typename Container>
	void operator()(Container& c, ControlT const& normal, ControlT const& zpass) { operator()(c.begin(), c.end(), normal, zpass); }

	template <typename In>
	void operator()(In first, In last, ControlT const& normal, ControlT const& zpass)
	{
		BaseEffect* currFx = 0;
		BaseEffect::Input fxInput;
		BaseEffect::clearInput(fxInput);

		static std::vector<LightT> sceneLights; sceneLights.resize(0);
		static std::vector<MatrixT> sceneLightMatrices; sceneLightMatrices.resize(0);
		fxInput.lights = gatherSceneLights(sceneLights, sceneLightMatrices);

		for(; first != last; ++first)
		{
			RenderBlock const& block = *first;

			if(currFx != block.fx)
			{
				if(currFx)
					currFx->end();
				currFx = block.fx;

				currFx->captureState();
				currFx->begin();
			}

			fxInput.surface = &surfaceInputs[block.surfaceIndex];
			fxInput.matrices = instanceInputs[block.instanceIndex].geometryMatrices;
			fxInput.bufferControl = (block.hasZPass)? &zpass: &normal;
	
			unsigned passCount = currFx->passCount(fxInput);
			for(unsigned pass = 0; pass < passCount; ++pass)
			{
				//currFx->passInfo();
				currFx->pass(fxInput, pass);

				ASSERT(block.mesh);
				render(rc, *block.mesh, block.subset);
			}
		}

		if(currFx)
			currFx->end();
	}

	BaseEffect::Input::Lights gatherSceneLights(std::vector<LightT>& lights, std::vector<MatrixT>& lightMatrices)
	{
		BaseEffect::Input::Lights fxLights;
		fxLights.count = 0;

		if(scene.mBlueprint.lights.empty())
			return fxLights;

		lights.resize(scene.mBlueprint.lights.size());
		lightMatrices.resize(scene.mBlueprint.lights.size());

		MatrixT nativeMatrix;
		for(size_t q = 0; q < scene.mBlueprint.lights.size(); ++q)
		{
			mutalisk::data::scene::Light const& light = scene.mBlueprint.lights[q];

			ASSERT(q >= 0 && q < scene.mState.light2XformIndex.size());
			toNative(nativeMatrix, scene.mState.matrices[scene.mState.light2XformIndex[q]]);

			lights[q] = light;
			lightMatrices[q] = nativeMatrix;
		}

		fxLights.data = &lights[0];
		fxLights.matrices = &lightMatrices[0];
		fxLights.count = scene.mBlueprint.lights.size();
		return fxLights;
	}
};

/*
void render(RenderContextT& rc, RenderableSceneT const& scene, int maxActors)
{
	Vec3 cameraPos;

	static std::vector<InstanceInput> instanceInputs;
	static std::vector<BaseEffect::Input::Surface> surfaceInputs;
	static std::vector<RenderBlock> bgRenderBlocks, opaqueRenderBlocks, transparentRenderBlocks, fgRenderBlocks;

	instanceInputs.resize(0);
	surfaceInputs.resize(0); 
	bgRenderBlocks.resize(0); fgRenderBlocks.resize(0); 
	opaqueRenderBlocks.resize(0); transparentRenderBlocks.resize(0); 

	static std::vector<mutalisk::data::scene::Actor const*> visibleActors;
	visibleActors.resize(0);

	RenderContext& camera = rc; // @TBD:
	findVisibleActors(camera, 0) (scene.mBlueprint.actors, visibleActors);
	blastInstanceInputs(scene, camera) (visibleActors, instanceInputs);
	blastSurfaceInputs(scene, 0) (visibleActors, surfaceInputs);
	blastRenderBlocks(scene, cameraPos) (visibleActors, bgRenderBlocks, opaqueRenderBlocks, transparentRenderBlocks, fgRenderBlocks);
	sortRenderBlocks()(transparentRenderBlocks);
	if(instanceInputs.empty() || surfaceInputs.empty())
	{
		ASSERT(visibleActors.empty());
	}
	else
	{
		BaseEffect::Input::BufferControl background;
		background.colorWriteEnable = true;
		background.zWriteEnable = false;
		background.zReadEnable = true;
		background.zEqual = false;

		BaseEffect::Input::BufferControl opaque[2];
		opaque[0].colorWriteEnable = true;
		opaque[0].zWriteEnable = true;
		opaque[0].zReadEnable = true;
		opaque[0].zEqual = false;
		// zpass
		opaque[1] = opaque[0];
		opaque[1].colorWriteEnable = false;

		BaseEffect::Input::BufferControl transparent[2];
		transparent[0].colorWriteEnable = true;
		transparent[0].zWriteEnable = false;
		transparent[0].zReadEnable = true;
		transparent[0].zEqual = false;
		// zpass
		transparent[1] = transparent[0];
		transparent[1].zEqual = true;

		BaseEffect::Input::BufferControl foreground;
		foreground.colorWriteEnable = true;
		foreground.zWriteEnable = false;
		foreground.zReadEnable = false;
		foreground.zEqual = false;

		drawRenderBlocks draw(rc, scene, 
			&instanceInputs[0], instanceInputs.size(), &surfaceInputs[0], surfaceInputs.size());
		
		draw(opaqueRenderBlocks,		opaque[0], opaque[1]);
		draw(bgRenderBlocks,			background, background);
		draw(transparentRenderBlocks,	transparent[0], transparent[1]);
		draw(fgRenderBlocks,			foreground, foreground);
	}
}*/

#endif // NEWAGE_RENDERER_H_
