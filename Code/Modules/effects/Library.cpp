#include "Library.h"
#include <stdlib.h>
#include <utility>
#include <string>
#include <memory>
#include <list>

#include "BaseEffect.h"
#include "all.h"

using namespace mutalisk;
using namespace mutalisk::effects;

unsigned  mutalisk::effects::version()
{
	return 0x0101;
}

struct Repository
{
	Repository(bool named = true)
		: mNamed(named)
	{
		{char const* n[] = {"lambert", "default", ""};
		entry(new Lambert(), n);}

		{char const* n[] = {"unlit", "baked", ""};
		entry(new Unlit(), n);}

		{char const* n[] = {"shiny", ""};
		entry(new Shiny(), n);}
	}

public:
	IndexT getIndexByName(std::string const& name) const
	{
		ASSERT(mNamed);

		IndexT index = 0;
		for(NamedEffectsT::const_iterator i = mEffects.begin(); i != mEffects.end(); ++i, ++index)
		{
			Repository::EffectWithNamesT::second_type::const_iterator n = i->second.begin();
			for(; n != i->second.end(); ++n)
				if(name == *n)
					break;
			if(n == i->second.end())
				continue;

			return index;
		}
		return NotFound;
	}

	BaseEffect* getByIndex(IndexT index) const
	{
		return mEffects[index].first;
	}

private:
	typedef std::pair<BaseEffect*, std::vector<std::string> >	EffectWithNamesT;
	typedef std::vector<EffectWithNamesT>						NamedEffectsT;
	NamedEffectsT	mEffects;
	bool			mNamed;

private:
	void entry(BaseEffect* fx, char const* namesArray[])
	{
		std::vector<std::string> names;
		if(mNamed)
			for(int q = 0; !std::string(namesArray[q]).empty(); ++q)
				names.push_back(namesArray[q]);
		mEffects.push_back(std::make_pair(fx, names));
	}
};
std::auto_ptr<Repository> gRepository;

IndexT mutalisk::effects::getIndexByName(std::string const& name)
{
	if(!gRepository.get())
		gRepository.reset(new Repository());
	return gRepository->getIndexByName(name);
}

BaseEffect* mutalisk::effects::getByIndex(IndexT index)
{
	if(!gRepository.get())
		gRepository.reset(new Repository(false));
	return gRepository->getByIndex(index);
}


bool mutalisk::effects::isSystemTexture(std::string const& name)
{
	unsigned index = static_cast<unsigned>(getSystemTextureIndexByName(name));
	return (index != ~0U && index < nSystemTexture_Count);
}

nSystemTexture mutalisk::effects::getSystemTextureIndexByName(std::string const& name)
{
	if(name == "shadow")
		return ShadowBuffer;
	else if(name == "mirror")
		return MirrorBuffer;
	return static_cast<nSystemTexture>(~0U);
}
