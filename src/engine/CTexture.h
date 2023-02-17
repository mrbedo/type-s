#ifndef CTEXTURE_H
#define CTEXTURE_H

#include <vector>

class CTexture
{
public:
	CTexture();
	~CTexture();

	void CreateTexture(std::vector<unsigned int>& textureArray, const char* strFileName, int textureID) const;
};
#endif