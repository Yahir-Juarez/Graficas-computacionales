#pragma once

#include <vector>

using uint32 = unsigned int;

template<class T>
using Vector = std::vector<T>;

#define SAFE_RELEASE(x) {if(x){x->Release(); x = nullptr;} }
class CGraphicsManager
{
public:
	CGraphicsManager(void * scrHandle,
					 bool bFullScreen,
					 bool bAntialasing,
					 uint32 samplesPerPixel,
					 uint32 sampleQuality);
	~CGraphicsManager();
private:
	bool m_bFullScreen;

};