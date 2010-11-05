#pragma once

#include "common.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <algorithm>
#include <functional>

#include "Matrix.h"

// Simple linear volume class which supports loading from DAT files
class Volume
{
public:

	class Voxel
	{
	public:

		Voxel()
		{
			SetValue(0.0f);
                }

		Voxel(const Voxel & datOther)
		{
			m_fValue = datOther.m_fValue;
                }

		Voxel(const float fValue)
		{
			SetValue(fValue);
                }


		~Voxel()
		{
                }

		void SetValue(const float fValue)
		{
			m_fValue = fValue;
                }

                float GetValue() const
		{
			return m_fValue;
                }

                bool operator==(const Voxel &datOther) const
		{
			return (GetValue() == datOther.GetValue());
                }

                bool operator!=(const Voxel &datOther) const
		{
			return !(*this == datOther);
                }

                bool operator>(const Voxel &datOther) const
		{
			return GetValue() > datOther.GetValue();
                }

                bool operator>=(const Voxel &datOther) const
		{
			return GetValue() >= datOther.GetValue();
                }

                bool operator<(const Voxel &datOther) const
		{
			return GetValue() < datOther.GetValue();
                }

                bool operator<=(const Voxel &datOther) const
		{
			return GetValue() <= datOther.GetValue();
                }

		const Voxel & operator+=(const Voxel & datOther)
		{
			m_fValue += datOther.m_fValue;
			return *this;
                }

		const Voxel & operator-=(const Voxel & datOther)
		{
			m_fValue -= datOther.m_fValue;
			return *this;
                }

		const Voxel & operator*=(const float & fOther)
		{
			m_fValue *= fOther;
			return *this;
                }

		const Voxel & operator/=(const float & fOther)
		{
			m_fValue /= fOther;
			return *this;
                }

		const Voxel operator+(const Voxel & datOther) const
		{
			Voxel voxNew = *this;
			voxNew += datOther;
			return voxNew;
                }

		const Voxel operator-(const Voxel & datOther) const
		{
			Voxel voxNew = *this;
			voxNew -= datOther;
			return voxNew;
                }

		const Voxel operator*(const float & fOther) const
		{
			Voxel voxNew = *this;
			voxNew *= fOther;
			return voxNew;
                }

		const Voxel operator/(const float & fOther) const
		{
			Voxel voxNew = *this;
			voxNew /= fOther;
			return voxNew;
                }

	private:
		float m_fValue;
	};

public:
	Volume() : m_iWidth(1), m_iHeight(1), m_iDepth(1), m_vecVoxels(1)
	{
        }

	Volume(const std::string &strFilename) : m_iWidth(1), m_iHeight(1), m_iDepth(1), m_vecVoxels(1)
	{
		load(strFilename);
        }

	~Volume(void)
	{
        }

	const Voxel & Get(const int iX, const int iY, const int iZ) const
	{
		return m_vecVoxels[iX + iY*m_iWidth + iZ*m_iWidth*m_iHeight];
        }

	const Voxel & Get(const int iIndex) const
	{
		return m_vecVoxels[iIndex];
        }

	const Voxel * Get() const
	{
		return &(m_vecVoxels.front());
        }

        int GetWidth() const
	{
		return m_iWidth;
        }

        int GetHeight() const
	{
		return m_iHeight;
        }

        int GetDepth() const
	{
		return m_iDepth;
        }

        int GetSize() const
	{
		return int(m_vecVoxels.size());
        }
	
	
	void load(const std::string & strFilename)
	{
		std::cout << "- Loading file '" << strFilename << "' ... " << std::endl;
		FILE *fp = NULL;
		
                fp = fopen(strFilename.c_str(),"rb");

		if (!fp)
		{
			std::cerr << "+ Error loading file." << std::endl << std::endl;
		}
		else
		{
			unsigned short uWidth, uHeight, uDepth;
			fread(&uWidth,sizeof(unsigned short),1,fp);
			fread(&uHeight,sizeof(unsigned short),1,fp);
			fread(&uDepth,sizeof(unsigned short),1,fp);

			m_iWidth = int(uWidth);
			m_iHeight = int(uHeight);
			m_iDepth = int(uDepth);

			const int iSlice = m_iWidth * m_iHeight;
			const int iSize = iSlice * m_iDepth;
			m_vecVoxels.resize(iSize);

			std::vector<unsigned short> vecData;
			vecData.resize(iSize);

			fread((void*)&(vecData.front()),sizeof(unsigned short),iSize,fp);
			fclose(fp);

			std::cout << "- File loaded." << std::endl;

			for (int k=0;k<m_iDepth;k++)
			{
				for (int j=0;j<m_iHeight;j++)
				{
					for (int i=0;i<m_iWidth;i++)
					{
						//we convert the data to float values in an interval of [0..1]
						const float fValue = std::min(1.0f,float(vecData[i + j*m_iWidth + k*iSlice]) / 4095.0f);
						m_vecVoxels[i+j*m_iWidth+k*iSlice] = Voxel(fValue);
					}
				}
				std::cout << "\r- Preparing data (" << (k*100) / (m_iDepth-1) << "%) ...";
			}
			std::cout << std::endl << "- Data prepared." << std::endl;
		}
        }

protected:

private:
	int m_iWidth,m_iHeight,m_iDepth;
        std::vector<Voxel> m_vecVoxels;
};
