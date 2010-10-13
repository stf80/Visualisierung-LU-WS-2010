#pragma once

#include "common.h"

// a (r,g,b,a) color value
class Color
{
public:

	Color(const unsigned char uR=0, const unsigned char uG=0, const unsigned char uB=0, const unsigned char uA=255)
	{
		m_uData[0] = uR;
		m_uData[1] = uG;
		m_uData[2] = uB;
		m_uData[3] = uA;
	};

	Color(const int iR, const int iG, const int iB, const int iA=255)
	{
		m_uData[0] = (unsigned char) std::max(0,std::min(255,iR));
		m_uData[1] = (unsigned char) std::max(0,std::min(255,iG));
		m_uData[2] = (unsigned char) std::max(0,std::min(255,iB));
		m_uData[3] = (unsigned char) std::max(0,std::min(255,iA));
	};

	Color(const float fR, const float fG, const float fB, const float fA=1.0f)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,fR*255.0f));
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,fG*255.0f));
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,fB*255.0f));
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,fA*255.0f));
	};

	Color(const unsigned char *pData)
	{
		m_uData[0] = pData[0];
		m_uData[1] = pData[1];
		m_uData[2] = pData[2];
		m_uData[3] = pData[3];
	};

	Color(const float *pData)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,pData[0]*255.0f));
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,pData[1]*255.0f));
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,pData[2]*255.0f));
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,pData[3]*255.0f));
	};

	~Color()
	{
	};

	void Set(const unsigned char uR, const unsigned char uG, const unsigned char uB, const unsigned char uA = 255)
	{
		m_uData[0] = uR;
		m_uData[1] = uG;
		m_uData[2] = uB;
		m_uData[3] = uA;
	};

	const unsigned char * Get() const
	{
		return m_uData;
	};

	void SetRed(const unsigned char uR)
	{
		m_uData[0] = uR;
	};

	const unsigned char GetRed() const
	{
		return m_uData[0];
	};

	void SetGreen(const unsigned char uG)
	{
		m_uData[1] = uG;
	};

	const unsigned char GetGreen() const
	{
		return m_uData[1];
	};

	void SetBlue(const unsigned char uB)
	{
		m_uData[2] = uB;
	};

	const unsigned char GetBlue() const
	{
		return m_uData[2];
	};

	void SetAlpha(const unsigned char uA)
	{
		m_uData[3] = uA;
	};

	const unsigned char GetAlpha() const
	{
		return m_uData[3];
	};

	void SetNormalized(const float fR, const float fG, const float fB, const float fA = 1.0f)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,fR*255.0f));
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,fG*255.0f));
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,fB*255.0f));
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,fA*255.0f));
	};

	void SetNormalizedRed(const float fR)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,fR*255.0f));
	};

	void SetNormalizedGreen(const float fG)
	{
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,fG*255.0f));
	};

	void SetNormalizedBlue(const float fB)
	{
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,fB*255.0f));
	};

	void SetNormalizedAlpha(const float fA)
	{
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,fA*255.0f));
	};

	const float GetNormalizedRed() const
	{
		return float(m_uData[0]) / 255.0f;
	};

	const float GetNormalizedGreen() const
	{
		return float(m_uData[1]) / 255.0f;
	};

	const float GetNormalizedBlue() const
	{
		return float(m_uData[2]) / 255.0f;
	};

	const float GetNormalizedAlpha() const
	{
		return float(m_uData[3]) / 255.0f;
	};

	void SetHSV(const float fHue, const float fSaturation, const float fValue)
	{
		float fRed = fValue;
		float fGreen = fValue;
		float fBlue = fValue;
		
		if(fSaturation > 0.0f)
		{
			const float fH = fHue * 6.0f;
			const unsigned int i = unsigned int(fH);
			const float fF = fH - i;

			const float fP = fValue * (1.0f - fSaturation);
			const float fQ = fValue * (1.0f - (fSaturation * fF));
			const float fT = fValue * (1.0f - (fSaturation * (1.0f - fF)));

			switch(i)
			{
			case 0:			
				fGreen = fT;
				fBlue = fP;
				break;
			case 1:
				fRed = fQ;
				fBlue = fP;
				break;
			case 2:
				fRed = fP;
				fBlue = fT;
				break;
			case 3:
				fRed = fP;
				fGreen = fQ;
				break;
			case 4:
				fRed = fT;
				fGreen = fP;
				break;
			case 5:
				fGreen = fP;
				fBlue = fQ;
				break;
			}
		}

		SetNormalized(fRed,fGreen,fBlue,GetNormalizedAlpha());
	};

	const Color & operator*=(const float fOther)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[0]) * fOther));
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[1]) * fOther));
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[2]) * fOther));
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[3]) * fOther));
		return *this;
	};

	const Color & operator/=(const float fOther)
	{
		m_uData[0] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[0]) / fOther));
		m_uData[1] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[1]) / fOther));
		m_uData[2] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[2]) / fOther));
		m_uData[3] = (unsigned char) std::max(0.0f,std::min(255.0f,float(m_uData[3]) / fOther));
		return *this;
	};

	const Color & operator+=(const Color &colOther)
	{
		m_uData[0] = (unsigned char) std::min(255,int(m_uData[0])+int(colOther.m_uData[0]));
		m_uData[1] = (unsigned char) std::min(255,int(m_uData[1])+int(colOther.m_uData[1]));
		m_uData[2] = (unsigned char) std::min(255,int(m_uData[2])+int(colOther.m_uData[2]));
		m_uData[3] = (unsigned char) std::min(255,int(m_uData[3])+int(colOther.m_uData[3]));
		return *this;
	};

	const Color & operator-=(const Color &colOther)
	{
		m_uData[0] = (unsigned char) std::max(0,int(m_uData[0])-int(colOther.m_uData[0]));
		m_uData[1] = (unsigned char) std::max(0,int(m_uData[1])-int(colOther.m_uData[1]));
		m_uData[2] = (unsigned char) std::max(0,int(m_uData[2])-int(colOther.m_uData[2]));
		m_uData[3] = (unsigned char) std::max(0,int(m_uData[2])-int(colOther.m_uData[3]));
		return *this;
	};

	const Color operator*(const float fOther) const
	{
		Color colNew = *this;
		colNew *= fOther;
		return colNew;
	};

	const Color operator/(const float fOther) const
	{
		Color colNew = *this;
		colNew *= fOther;
		return colNew;
	};

	const Color operator+(const Color &colOther) const
	{
		Color colNew = *this;
		colNew += colOther;
		return colNew;
	};

	const Color operator-(const Color &colOther) const
	{
		Color colNew = *this;
		colNew -= colOther;
		return colNew;
	};

	const bool operator==(const Color & colOther) const
	{
		return (memcmp((void*)m_uData,(void*)colOther.m_uData,sizeof(unsigned char)*4) == 0);
	};

	const bool operator!=(const Color & colOther) const
	{
		return !(*this == colOther);
	};

	const bool choose()
	{
		return chooseColor(*this);
	};

	static const bool chooseColor(Color & colColor)
	{
		CHOOSECOLOR cc;
		static COLORREF vcCustom[16];
		
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = GetForegroundWindow();
		cc.lpCustColors = (LPDWORD) vcCustom;
		cc.rgbResult = RGB(colColor.GetRed(),colColor.GetGreen(),colColor.GetBlue());
		cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;

		const bool bResult = (ChooseColor(&cc) != 0);

		if (bResult)
		{
			colColor.SetRed(GetRValue(cc.rgbResult));
			colColor.SetGreen(GetGValue(cc.rgbResult));
			colColor.SetBlue(GetBValue(cc.rgbResult));
		}

		return bResult;
	}

private:
	unsigned char m_uData[4];
};

inline std::ostream & operator<< (std::ostream & os, const Color & colColor)
{
	os << "(" << unsigned int(colColor.GetRed()) << ";" << unsigned int(colColor.GetGreen()) << ";" << unsigned int(colColor.GetBlue()) << ";" << unsigned short(colColor.GetAlpha()) << ")";
	return os;
}

inline std::istream & operator>> (std::istream & is, Color & colColor)
{	
	unsigned int uR,uG,uB,uA;

	if (is >> eat("(") >> uR >> eat(";") >> uG >> eat(";") >> uB >> eat(";") >> uA >> eat(")"))
		colColor.Set(unsigned char(uR),unsigned char(uG),unsigned char(uB),unsigned char(uA));

	return is;
}
