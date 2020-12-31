#ifndef MUTANT_TYPES_IOS_H_
#define MUTANT_TYPES_IOS_H_

#include <ostream>
#include <sstream>

/*inline std::ostream& operator<<( std::ostream& out, D3DXVECTOR3 const& d ) {
	out << "(" << d.x << " " << d.y << " " << d.z << ")";
	return out;
}

inline std::ostream& operator<<( std::ostream& out, D3DXQUATERNION const& d ) {
	out << "(" << d.x << " " << d.y << " " << d.z << " " << d.w << ")";
	return out;
}

inline std::ostream& operator<<( std::ostream& out, D3DXMATRIX const& d ) {
	char buf[512];
	sprintf( buf,
		"[ %3.2f, %3.2f, %3.2f, %3.2f ]\n"
		"[ %3.2f, %3.2f, %3.2f, %3.2f ]\n"
		"[ %3.2f, %3.2f, %3.2f, %3.2f ]\n"
		"[ %3.2f, %3.2f, %3.2f, %3.2f ]",
		d._11, d._12, d._13, d._14,
		d._21, d._22, d._23, d._24,
		d._31, d._32, d._33, d._34,
		d._41, d._42, d._43, d._44 );
	out << buf;
	return out;
}*/

std::string operator+( std::string const& str, int n ) {
	std::stringstream ss;
	ss << n;
	return str + ss.str();
}

std::string operator+( std::string const& str, float n ) {
	std::stringstream ss;
	ss << n;
	return str + ss.str();
}

#endif
