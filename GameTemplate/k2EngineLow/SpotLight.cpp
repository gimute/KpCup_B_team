#include "k2EngineLowPreCompile.h"
#include "SpotLight.h"

namespace nsK2EngineLow {

	SpotLight::SpotLight()
	{
		if (m_spotLight == nullptr)
		{
			m_spotLight = g_sceneLight->NewSpotLight();
		}
	}

	SpotLight::~SpotLight()
	{
		g_sceneLight->DeleteSpotLight(m_spotLight);
	}


}