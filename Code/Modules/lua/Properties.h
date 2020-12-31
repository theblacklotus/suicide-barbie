#ifndef MUTALIKS_LUA__PROPERTIES_H
#define MUTALIKS_LUA__PROPERTIES_H

#include "cfg.h"
#include <map>
#include <vector>
#include <string>

namespace mutalisk { namespace lua
{
	struct Properties
	{
		typedef std::map<std::string, double>				NumbersT;
		typedef std::map<std::string, std::vector<double> >	VectorsT;
		typedef std::map<std::string, std::string>			StringsT;

		NumbersT	numbers;
		VectorsT	vectors;
		StringsT	strings;
	};
	typedef std::map<std::string, Properties>					PropertiesByNameT;

	bool readFromVariable(std::string const& varName, PropertiesByNameT& props);
	void readFromResult(PropertiesByNameT& props);
	void readFromResult(Properties& props);

} // namespace lua
} // namespace mutalisk


#endif // MUTALIKS_LUA__PROPERTIES_H