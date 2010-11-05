#pragma once

#include "common.h"
#include <cmath>
#include "Vector.h"
#include "Quaternion.h"

// 4x4 Matrix class
class Matrix
{
public:

	Matrix()
	{
		m_vfElements.m_vfElements16[0]=m_vfElements.m_vfElements16[5]=m_vfElements.m_vfElements16[10]=m_vfElements.m_vfElements16[15]=1.0f;
		m_vfElements.m_vfElements16[1]=m_vfElements.m_vfElements16[2]=m_vfElements.m_vfElements16[3]=m_vfElements.m_vfElements16[4]=m_vfElements.m_vfElements16[6]=m_vfElements.m_vfElements16[7]=m_vfElements.m_vfElements16[8]=m_vfElements.m_vfElements16[9]=m_vfElements.m_vfElements16[11]=m_vfElements.m_vfElements16[12]=m_vfElements.m_vfElements16[13]=m_vfElements.m_vfElements16[14]=0.0f;
        }

	Matrix(	const float & fElement0, const float & fElement4, const float & fElement8, const float & fElement12,
					const float & fElement1, const float & fElement5, const float & fElement9, const float & fElement13,
					const float & fElement2, const float & fElement6, const float & fElement10, const float & fElement14,
					const float & fElement3, const float & fElement7, const float & fElement11, const float & fElement15)
	{
		m_vfElements.m_vfElements16[0]=fElement0; m_vfElements.m_vfElements16[4]=fElement4;  m_vfElements.m_vfElements16[8]=fElement8;  m_vfElements.m_vfElements16[12]=fElement12;
		m_vfElements.m_vfElements16[1]=fElement1; m_vfElements.m_vfElements16[5]=fElement5;  m_vfElements.m_vfElements16[9]=fElement9;  m_vfElements.m_vfElements16[13]=fElement13;
		m_vfElements.m_vfElements16[2]=fElement2; m_vfElements.m_vfElements16[6]=fElement6; m_vfElements.m_vfElements16[10]=fElement10; m_vfElements.m_vfElements16[14]=fElement14;
		m_vfElements.m_vfElements16[3]=fElement3; m_vfElements.m_vfElements16[7]=fElement7; m_vfElements.m_vfElements16[11]=fElement11; m_vfElements.m_vfElements16[15]=fElement15;
        }

	Matrix(const Vector & vecOne, const Vector & vecTwo, const Vector & vecThree, const Vector & vecFour = Vector(0.0f,0.0f,0.0f))
	{
		
		m_vfElements.m_vfElements16[0]=vecOne.GetX(); m_vfElements.m_vfElements16[4]=vecOne.GetY();  m_vfElements.m_vfElements16[8]=vecOne.GetZ();  m_vfElements.m_vfElements16[12]=0.0f;
		m_vfElements.m_vfElements16[1]=vecTwo.GetX(); m_vfElements.m_vfElements16[5]=vecTwo.GetY();  m_vfElements.m_vfElements16[9]=vecTwo.GetZ();  m_vfElements.m_vfElements16[13]=0.0f;
		m_vfElements.m_vfElements16[2]=vecThree.GetX(); m_vfElements.m_vfElements16[6]=vecThree.GetY(); m_vfElements.m_vfElements16[10]=vecThree.GetZ(); m_vfElements.m_vfElements16[14]=0.0f;
		m_vfElements.m_vfElements16[3]=0.0f; m_vfElements.m_vfElements16[7]=0.0f; m_vfElements.m_vfElements16[11]=0.0f; m_vfElements.m_vfElements16[15]=1.0f;	
        }

	Matrix(const float *pElements)
	{
		memcpy((void*)m_vfElements.m_vfElements16, (void *)pElements, 16*sizeof(float));
        }

	Matrix(const float vfElements[4][4])
	{
		memcpy((void*)m_vfElements.m_vfElements16, (void *)vfElements, 16*sizeof(float));
        }

	Matrix(const Quaternion & quaRotation)
	{
		m_vfElements.m_vfElements16[0]=m_vfElements.m_vfElements16[5]=m_vfElements.m_vfElements16[10]=m_vfElements.m_vfElements16[15]=1.0f;
		m_vfElements.m_vfElements16[1]=m_vfElements.m_vfElements16[2]=m_vfElements.m_vfElements16[3]=m_vfElements.m_vfElements16[4]=m_vfElements.m_vfElements16[6]=m_vfElements.m_vfElements16[7]=m_vfElements.m_vfElements16[8]=m_vfElements.m_vfElements16[9]=m_vfElements.m_vfElements16[11]=m_vfElements.m_vfElements16[12]=m_vfElements.m_vfElements16[13]=m_vfElements.m_vfElements16[14]=0.0f;
		rotate(quaRotation);
        }

	~Matrix()
	{
        }

	void Set(const float *pElements)
	{
		memcpy((void*)m_vfElements.m_vfElements16, (void *)pElements, 16*sizeof(float));
        }

        void clearMatrix()
	{
		m_vfElements.m_vfElements16[0]=m_vfElements.m_vfElements16[5]=m_vfElements.m_vfElements16[10]=m_vfElements.m_vfElements16[15]=1.0f;
		m_vfElements.m_vfElements16[1]=m_vfElements.m_vfElements16[2]=m_vfElements.m_vfElements16[3]=m_vfElements.m_vfElements16[4]=m_vfElements.m_vfElements16[6]=m_vfElements.m_vfElements16[7]=m_vfElements.m_vfElements16[8]=m_vfElements.m_vfElements16[9]=m_vfElements.m_vfElements16[11]=m_vfElements.m_vfElements16[12]=m_vfElements.m_vfElements16[13]=m_vfElements.m_vfElements16[14]=0.0f;
        }

        float * Get() const
	{
		return (float *) m_vfElements.m_vfElements16;
        }

	void Set(const unsigned int i, const unsigned int j, const float & fValue)
	{
		m_vfElements.m_vfElements44[i][j] = fValue;
        }

        const float & Get(const unsigned int i, const unsigned int j) const
	{
		return m_vfElements.m_vfElements44[i][j];
        }

        const float & Get(const unsigned int i) const
	{
		return m_vfElements.m_vfElements16[i];
        }

	void SetRow(const unsigned int i, const float vfValues[4])
	{
		m_vfElements.m_vfElements16[ i+0] = vfValues[0];
		m_vfElements.m_vfElements16[ i+4] = vfValues[1];
		m_vfElements.m_vfElements16[ i+8] = vfValues[2];
		m_vfElements.m_vfElements16[i+12] = vfValues[3];	
        }

	void SetColumn(const unsigned int i, const float vfValues[4])
	{
		m_vfElements.m_vfElements16[i*4+0] = vfValues[0];
		m_vfElements.m_vfElements16[i*4+1] = vfValues[1];
		m_vfElements.m_vfElements16[i*4+2] = vfValues[2];
		m_vfElements.m_vfElements16[i*4+3] = vfValues[3];
        }

	void SetRow(const unsigned int i, const Vector & vecVector)
	{
		m_vfElements.m_vfElements16[ i+0] = vecVector.GetX();
		m_vfElements.m_vfElements16[ i+4] = vecVector.GetY();
		m_vfElements.m_vfElements16[ i+8] = vecVector.GetZ();
        }

	void SetColumn(const unsigned int i, const Vector & vecVector)
	{
		m_vfElements.m_vfElements16[i*4+0] = vecVector.GetX();
		m_vfElements.m_vfElements16[i*4+1] = vecVector.GetY();
		m_vfElements.m_vfElements16[i*4+2] = vecVector.GetZ();
        }

	float & operator()(const unsigned int i, const unsigned int j)
	{
		return m_vfElements.m_vfElements44[i][j];
        }

	const float & operator()(const unsigned int i, const unsigned int j) const
	{
		return m_vfElements.m_vfElements44[i][j];
        }

	void SetTranslation(const Vector & vecTranslation)
	{
		m_vfElements.m_vfElements16[12] = vecTranslation.GetX();
		m_vfElements.m_vfElements16[13] = vecTranslation.GetY();
		m_vfElements.m_vfElements16[14] = vecTranslation.GetZ();
		m_vfElements.m_vfElements16[15] = 1.0f;
        }

	const Vector GetTranslation() const
	{
		return Vector(m_vfElements.m_vfElements16[12],m_vfElements.m_vfElements16[13],m_vfElements.m_vfElements16[14]);
        }

	const Quaternion GetRotation() const
	{
		const float fTrace = m_vfElements.m_vfElements16[0] + m_vfElements.m_vfElements16[5] + m_vfElements.m_vfElements16[10] + 1.0f;

		if (fTrace > 0.0f)
		{
			const float fS = 0.5f / sqrt(fTrace);
			const float fW = 0.25f / fS;
			const float fX = ( m_vfElements.m_vfElements16[6] - m_vfElements.m_vfElements16[9] ) * fS;
			const float fY = ( m_vfElements.m_vfElements16[8] - m_vfElements.m_vfElements16[2] ) * fS;
			const float fZ = ( m_vfElements.m_vfElements16[1] - m_vfElements.m_vfElements16[4] ) * fS;
			return Quaternion(Vector(fX,fY,fZ).GetNormalized(),fW);
		}
		else
		{
			const float fMaximum = std::max(m_vfElements.m_vfElements44[0][0],std::max(m_vfElements.m_vfElements44[1][1],m_vfElements.m_vfElements44[2][2]));

			if (fMaximum == m_vfElements.m_vfElements44[0][0])
			{
				const float fS  = sqrtf( 1.0f + m_vfElements.m_vfElements16[0] - m_vfElements.m_vfElements16[5] - m_vfElements.m_vfElements16[10] ) * 2.0f;
				const float fX = 0.5f / fS;
				const float fY = (m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[1] ) / fS;
				const float fZ = (m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[2] ) / fS;
				const float fW = (m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[6] ) / fS;
				return Quaternion(Vector(fX,fY,fZ).GetNormalized(),fW);
			}
			else if (fMaximum == m_vfElements.m_vfElements44[1][1])
			{
				const float fS  = sqrtf( 1.0f + m_vfElements.m_vfElements16[5] - m_vfElements.m_vfElements16[0] - m_vfElements.m_vfElements16[10] ) * 2.0f;
				const float fX = (m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[1] ) / fS;
				const float fY = 0.5f / fS;
				const float fZ = (m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[6] ) / fS;
				const float fW = (m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[2] ) / fS;
				return Quaternion(Vector(fX,fY,fZ).GetNormalized(),fW);
			}
			else
			{
				const float fS  = sqrtf( 1.0f + m_vfElements.m_vfElements16[10] - m_vfElements.m_vfElements16[0] - m_vfElements.m_vfElements16[5] ) * 2.0f;
				const float fX = (m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[2] ) / fS;
				const float fY = (m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[6] ) / fS;
				const float fZ = 0.5f / fS;
				const float fW = (m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[1] ) / fS;
				return Quaternion(Vector(fX,fY,fZ).GetNormalized(),fW);
			}
		}
        }

	const Matrix GetTranslated(const Vector & vecTranslation) const
	{
		return (*this) * Matrix(1.0f,0.0f,0.0f,vecTranslation.GetX(), 0.0f,1.0f,0.0f,vecTranslation.GetY(), 0.0f,0.0f,1.0f,vecTranslation.GetZ(), 0.0f, 0.0f,0.0f,1.0f);
        }

	void translate(const Vector &vecTranslation)
	{
		*this = GetTranslated(vecTranslation);
        }

	const Matrix GetScaled(const Vector &vecScale) const
	{
		return (*this) * 	Matrix(	vecScale.GetX(),0.0f,0.0f,0.0f, 0.0f,vecScale.GetY(),0.0f,0.0f,	0.0f,0.0f,vecScale.GetZ(),0.0f, 0.0f,0.0f,0.0f,1.0f);
        }

	void scale(const Vector &vecScale)
	{
		*this = GetScaled(vecScale);
        }

	const Matrix GetRotated(const Quaternion & quaRotation) const
	{
		const float fXX = quaRotation.GetVector().GetX() * quaRotation.GetVector().GetX();
		const float fXY = quaRotation.GetVector().GetX() * quaRotation.GetVector().GetY();
		const float fXZ = quaRotation.GetVector().GetX() * quaRotation.GetVector().GetZ();
		const float fXW = quaRotation.GetVector().GetX() * quaRotation.GetScalar();
		const float fYY = quaRotation.GetVector().GetY() * quaRotation.GetVector().GetY();
		const float fYZ = quaRotation.GetVector().GetY() * quaRotation.GetVector().GetZ();
		const float fYW = quaRotation.GetVector().GetY() * quaRotation.GetScalar();
		const float fZZ = quaRotation.GetVector().GetZ() * quaRotation.GetVector().GetZ();
		const float fZW = quaRotation.GetVector().GetZ() * quaRotation.GetScalar();

		return (*this) * Matrix(1.0f - 2.0f * (fYY + fZZ), 2.0f * (fXY - fZW), 2.0f * (fXZ + fYW), 0.0f,  2.0f * (fXY + fZW), 1.0f - 2.0f * (fXX + fZZ), 2.0f * (fYZ - fXW), 0.0f, 2.0f * (fXZ - fYW), 2.0f * (fYZ + fXW), 1.0f - 2.0f * (fXX + fYY), 0.0f,	0.0f, 0.0f, 0.0f, 1.0f);
        }

	void rotate(const Quaternion & quaRotation)
	{
		*this = GetRotated(quaRotation);
        }

	const Matrix GetInverse() const
	{
		const float fTmp1 = m_vfElements.m_vfElements44[2][2]*m_vfElements.m_vfElements44[3][3];
		const float fTmp2 = m_vfElements.m_vfElements44[0][1]*fTmp1;
		const float fTmp4 = m_vfElements.m_vfElements44[2][3]*m_vfElements.m_vfElements44[3][2];
		const float fTmp5 = m_vfElements.m_vfElements44[0][1]*fTmp4;
		const float fTmp6 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[3][3];
		const float fTmp7 = m_vfElements.m_vfElements44[2][1]*fTmp6;
		const float fTmp8 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[3][2];
		const float fTmp9 = m_vfElements.m_vfElements44[2][1]*fTmp8;
		const float fTmp11 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[2][3];
		const float fTmp12 = m_vfElements.m_vfElements44[3][1]*fTmp11;
		const float fTmp14 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[2][2];
		const float fTmp15 = m_vfElements.m_vfElements44[3][1]*fTmp14;
		const float fTmp17 = m_vfElements.m_vfElements44[1][1]*fTmp1;
		const float fTmp19 = m_vfElements.m_vfElements44[1][1]*fTmp4;
		const float fTmp22 = m_vfElements.m_vfElements44[1][2]*m_vfElements.m_vfElements44[3][3];
		const float fTmp23 = m_vfElements.m_vfElements44[2][1]*fTmp22;
		const float fTmp26 = m_vfElements.m_vfElements44[1][3]*m_vfElements.m_vfElements44[3][2];
		const float fTmp27 = m_vfElements.m_vfElements44[2][1]*fTmp26;
		const float fTmp29 = m_vfElements.m_vfElements44[1][2]*m_vfElements.m_vfElements44[2][3];
		const float fTmp30 = m_vfElements.m_vfElements44[3][1]*fTmp29;
		const float fTmp32 = m_vfElements.m_vfElements44[1][3]*m_vfElements.m_vfElements44[2][2];
		const float fTmp33 = m_vfElements.m_vfElements44[3][1]*fTmp32;
		const float fTmp48 = m_vfElements.m_vfElements44[0][1]*fTmp22;
		const float fTmp50 = m_vfElements.m_vfElements44[0][1]*fTmp26;
		const float fTmp53 = m_vfElements.m_vfElements44[1][1]*fTmp6;
		const float fTmp56 = m_vfElements.m_vfElements44[1][1]*fTmp8;
		const float fTmp58 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[1][3];
		const float fTmp59 = m_vfElements.m_vfElements44[3][1]*fTmp58;
		const float fTmp61 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[1][2];
		const float fTmp62 = m_vfElements.m_vfElements44[3][1]*fTmp61;
		const float fTmp66 = m_vfElements.m_vfElements44[0][1]*fTmp29;
		const float fTmp69 = m_vfElements.m_vfElements44[0][1]*fTmp32;
		const float fTmp71 = m_vfElements.m_vfElements44[1][1]*fTmp11;
		const float fTmp73 = m_vfElements.m_vfElements44[1][1]*fTmp14;
		const float fTmp76 = m_vfElements.m_vfElements44[2][1]*fTmp58;
		const float fTmp79 = m_vfElements.m_vfElements44[2][1]*fTmp61;
		const float fTmp84 = 1.0f/(m_vfElements.m_vfElements44[0][0]*fTmp17-m_vfElements.m_vfElements44[0][0]*fTmp19-m_vfElements.m_vfElements44[0][0]*fTmp23+m_vfElements.m_vfElements44[0][0]*fTmp27+m_vfElements.m_vfElements44[0][0]*fTmp30-m_vfElements.m_vfElements44[0][0]*fTmp33-m_vfElements.m_vfElements44[1][0]*fTmp2+m_vfElements.m_vfElements44[1][0]*fTmp5+m_vfElements.m_vfElements44[1][0]*fTmp7-m_vfElements.m_vfElements44[1][0]*fTmp9-m_vfElements.m_vfElements44[1][0]*fTmp12+m_vfElements.m_vfElements44[1][0]*fTmp15+m_vfElements.m_vfElements44[2][0]*fTmp48-m_vfElements.m_vfElements44[2][0]*fTmp50-m_vfElements.m_vfElements44[2][0]*fTmp53+m_vfElements.m_vfElements44[2][0]*fTmp56+m_vfElements.m_vfElements44[2][0]*fTmp59-m_vfElements.m_vfElements44[2][0]*fTmp62-m_vfElements.m_vfElements44[3][0]*fTmp66+m_vfElements.m_vfElements44[3][0]*fTmp69+m_vfElements.m_vfElements44[3][0]*fTmp71-m_vfElements.m_vfElements44[3][0]*fTmp73-m_vfElements.m_vfElements44[3][0]*fTmp76+m_vfElements.m_vfElements44[3][0]*fTmp79);
		const float fTmp116 = m_vfElements.m_vfElements44[2][1]*m_vfElements.m_vfElements44[3][2];
		const float fTmp118 = m_vfElements.m_vfElements44[2][2]*m_vfElements.m_vfElements44[3][1];
		const float fTmp121 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[3][2];
		const float fTmp124 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[3][1];
		const float fTmp126 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[2][2];
		const float fTmp128 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[2][1];
		const float fTmp133 = m_vfElements.m_vfElements44[2][1]*m_vfElements.m_vfElements44[3][3];
		const float fTmp135 = m_vfElements.m_vfElements44[2][3]*m_vfElements.m_vfElements44[3][1];
		const float fTmp138 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[3][3];
		const float fTmp141 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[3][1];
		const float fTmp143 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[2][3];
		const float fTmp145 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[2][1];
		const float fTmp151 = m_vfElements.m_vfElements44[1][1]*m_vfElements.m_vfElements44[2][2];
		const float fTmp153 = m_vfElements.m_vfElements44[1][2]*m_vfElements.m_vfElements44[2][1];
		const float fTmp159 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[1][2];
		const float fTmp161 = m_vfElements.m_vfElements44[0][2]*m_vfElements.m_vfElements44[1][1];
		const float fTmp166 = m_vfElements.m_vfElements44[1][1]*m_vfElements.m_vfElements44[3][3];
		const float fTmp168 = m_vfElements.m_vfElements44[1][3]*m_vfElements.m_vfElements44[3][1];
		const float fTmp174 = m_vfElements.m_vfElements44[0][1]*m_vfElements.m_vfElements44[1][3];
		const float fTmp176 = m_vfElements.m_vfElements44[0][3]*m_vfElements.m_vfElements44[1][1];
		const float fTmp187 = m_vfElements.m_vfElements44[1][1]*m_vfElements.m_vfElements44[2][3];
		const float fTmp189 = m_vfElements.m_vfElements44[1][3]*m_vfElements.m_vfElements44[2][1];
		const float fTmp212 = m_vfElements.m_vfElements44[1][1]*m_vfElements.m_vfElements44[3][2];
		const float fTmp214 = m_vfElements.m_vfElements44[1][2]*m_vfElements.m_vfElements44[3][1];
		
		Matrix matNew;
		matNew.m_vfElements.m_vfElements44[0][1] = (-fTmp2+fTmp5+fTmp7-fTmp9-fTmp12+fTmp15)*fTmp84;
		matNew.m_vfElements.m_vfElements44[0][2] = -(-fTmp48+fTmp50+fTmp53-fTmp56-fTmp59+fTmp62)*fTmp84;
		matNew.m_vfElements.m_vfElements44[1][2] = -(m_vfElements.m_vfElements44[0][0]*fTmp22-m_vfElements.m_vfElements44[0][0]*fTmp26-m_vfElements.m_vfElements44[1][0]*fTmp6+m_vfElements.m_vfElements44[1][0]*fTmp8+m_vfElements.m_vfElements44[3][0]*fTmp58-m_vfElements.m_vfElements44[3][0]*fTmp61)*fTmp84;
		matNew.m_vfElements.m_vfElements44[1][3] = -(-m_vfElements.m_vfElements44[0][0]*fTmp29+m_vfElements.m_vfElements44[0][0]*fTmp32+m_vfElements.m_vfElements44[1][0]*fTmp11-m_vfElements.m_vfElements44[1][0]*fTmp14-m_vfElements.m_vfElements44[2][0]*fTmp58+m_vfElements.m_vfElements44[2][0]*fTmp61)*fTmp84;
		matNew.m_vfElements.m_vfElements44[3][1] = (m_vfElements.m_vfElements44[0][0]*fTmp116-m_vfElements.m_vfElements44[0][0]*fTmp118-m_vfElements.m_vfElements44[2][0]*fTmp121+m_vfElements.m_vfElements44[2][0]*fTmp124+m_vfElements.m_vfElements44[3][0]*fTmp126-m_vfElements.m_vfElements44[3][0]*fTmp128)*fTmp84;
		matNew.m_vfElements.m_vfElements44[2][1] = -(m_vfElements.m_vfElements44[0][0]*fTmp133-m_vfElements.m_vfElements44[0][0]*fTmp135-m_vfElements.m_vfElements44[2][0]*fTmp138+m_vfElements.m_vfElements44[2][0]*fTmp141+m_vfElements.m_vfElements44[3][0]*fTmp143-m_vfElements.m_vfElements44[3][0]*fTmp145)*fTmp84;
		matNew.m_vfElements.m_vfElements44[3][3] = (m_vfElements.m_vfElements44[0][0]*fTmp151-m_vfElements.m_vfElements44[0][0]*fTmp153-m_vfElements.m_vfElements44[1][0]*fTmp126+m_vfElements.m_vfElements44[1][0]*fTmp128+m_vfElements.m_vfElements44[2][0]*fTmp159-m_vfElements.m_vfElements44[2][0]*fTmp161)*fTmp84;
		matNew.m_vfElements.m_vfElements44[2][2] = (m_vfElements.m_vfElements44[0][0]*fTmp166-m_vfElements.m_vfElements44[0][0]*fTmp168-m_vfElements.m_vfElements44[1][0]*fTmp138+m_vfElements.m_vfElements44[1][0]*fTmp141+m_vfElements.m_vfElements44[3][0]*fTmp174-m_vfElements.m_vfElements44[3][0]*fTmp176)*fTmp84;
		matNew.m_vfElements.m_vfElements44[2][0] = (m_vfElements.m_vfElements44[1][0]*fTmp133-m_vfElements.m_vfElements44[1][0]*fTmp135-m_vfElements.m_vfElements44[2][0]*fTmp166+m_vfElements.m_vfElements44[2][0]*fTmp168+m_vfElements.m_vfElements44[3][0]*fTmp187-m_vfElements.m_vfElements44[3][0]*fTmp189)*fTmp84;
		matNew.m_vfElements.m_vfElements44[1][0] = -(m_vfElements.m_vfElements44[1][0]*fTmp1-m_vfElements.m_vfElements44[1][0]*fTmp4-m_vfElements.m_vfElements44[2][0]*fTmp22+m_vfElements.m_vfElements44[2][0]*fTmp26+m_vfElements.m_vfElements44[3][0]*fTmp29-m_vfElements.m_vfElements44[3][0]*fTmp32)*fTmp84;
		matNew.m_vfElements.m_vfElements44[0][3] = -(fTmp66-fTmp69-fTmp71+fTmp73+fTmp76-fTmp79)*fTmp84;
		matNew.m_vfElements.m_vfElements44[3][2] = -(m_vfElements.m_vfElements44[0][0]*fTmp212-m_vfElements.m_vfElements44[0][0]*fTmp214-m_vfElements.m_vfElements44[1][0]*fTmp121+m_vfElements.m_vfElements44[1][0]*fTmp124+m_vfElements.m_vfElements44[3][0]*fTmp159-m_vfElements.m_vfElements44[3][0]*fTmp161)*fTmp84;
		matNew.m_vfElements.m_vfElements44[0][0] = (fTmp17-fTmp19-fTmp23+fTmp27+fTmp30-fTmp33)*fTmp84;
		matNew.m_vfElements.m_vfElements44[2][3] = -(m_vfElements.m_vfElements44[0][0]*fTmp187-m_vfElements.m_vfElements44[0][0]*fTmp189-m_vfElements.m_vfElements44[1][0]*fTmp143+m_vfElements.m_vfElements44[1][0]*fTmp145+m_vfElements.m_vfElements44[2][0]*fTmp174-m_vfElements.m_vfElements44[2][0]*fTmp176)*fTmp84;
		matNew.m_vfElements.m_vfElements44[3][0] = -(m_vfElements.m_vfElements44[1][0]*fTmp116-m_vfElements.m_vfElements44[1][0]*fTmp118-m_vfElements.m_vfElements44[2][0]*fTmp212+m_vfElements.m_vfElements44[2][0]*fTmp214+m_vfElements.m_vfElements44[3][0]*fTmp151-m_vfElements.m_vfElements44[3][0]*fTmp153)*fTmp84;
		matNew.m_vfElements.m_vfElements44[1][1] = (m_vfElements.m_vfElements44[0][0]*fTmp1-m_vfElements.m_vfElements44[0][0]*fTmp4-m_vfElements.m_vfElements44[2][0]*fTmp6+m_vfElements.m_vfElements44[2][0]*fTmp8+m_vfElements.m_vfElements44[3][0]*fTmp11-m_vfElements.m_vfElements44[3][0]*fTmp14)*fTmp84;

		return matNew;
        }

	void invert()
	{
		(*this) = GetInverse();
	}

	const Matrix GetTransposed() const
	{
		return Matrix(
			m_vfElements.m_vfElements16[0],m_vfElements.m_vfElements16[1],m_vfElements.m_vfElements16[2],m_vfElements.m_vfElements16[3],
			m_vfElements.m_vfElements16[4],m_vfElements.m_vfElements16[5],m_vfElements.m_vfElements16[6],m_vfElements.m_vfElements16[7],
			m_vfElements.m_vfElements16[8],m_vfElements.m_vfElements16[9],m_vfElements.m_vfElements16[10],m_vfElements.m_vfElements16[11],
			m_vfElements.m_vfElements16[12],m_vfElements.m_vfElements16[13],m_vfElements.m_vfElements16[14],m_vfElements.m_vfElements16[15]);
        }

	void transpose()
	{
		(*this) = GetTransposed();
        }

	const Vector GetRotated(const Vector & vecOther) const
	{
		return Vector(
			(m_vfElements.m_vfElements16[0]*vecOther.GetX() + m_vfElements.m_vfElements16[4]*vecOther.GetY() + m_vfElements.m_vfElements16[8]*vecOther.GetZ()), 
			(m_vfElements.m_vfElements16[1]*vecOther.GetX() + m_vfElements.m_vfElements16[5]*vecOther.GetY() + m_vfElements.m_vfElements16[9]*vecOther.GetZ()),
			(m_vfElements.m_vfElements16[2]*vecOther.GetX() + m_vfElements.m_vfElements16[6]*vecOther.GetY() + m_vfElements.m_vfElements16[10]*vecOther.GetZ()));
        }

        bool operator==(const Matrix & matOther) const
	{
		for (unsigned int i=0;i < 16; i++)
		{
			if (m_vfElements.m_vfElements16[i] != matOther.m_vfElements.m_vfElements16[i])
				return false;
		}

		return true;
        }

        bool operator!=(const Matrix & matOther) const
	{
		return !(*this == matOther);
        }

	const Matrix operator*(const Matrix & matOther) const
	{
		return Matrix(
			m_vfElements.m_vfElements16[0]*matOther.m_vfElements.m_vfElements16[0] + m_vfElements.m_vfElements16[4]*matOther.m_vfElements.m_vfElements16[1] + m_vfElements.m_vfElements16[8]*matOther.m_vfElements.m_vfElements16[2] + m_vfElements.m_vfElements16[12]*matOther.m_vfElements.m_vfElements16[3],
			m_vfElements.m_vfElements16[0]*matOther.m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[4]*matOther.m_vfElements.m_vfElements16[5] + m_vfElements.m_vfElements16[8]*matOther.m_vfElements.m_vfElements16[6] + m_vfElements.m_vfElements16[12]*matOther.m_vfElements.m_vfElements16[7],
			m_vfElements.m_vfElements16[0]*matOther.m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[4]*matOther.m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[8]*matOther.m_vfElements.m_vfElements16[10] + m_vfElements.m_vfElements16[12]*matOther.m_vfElements.m_vfElements16[11],
			m_vfElements.m_vfElements16[0]*matOther.m_vfElements.m_vfElements16[12] + m_vfElements.m_vfElements16[4]*matOther.m_vfElements.m_vfElements16[13] + m_vfElements.m_vfElements16[8]*matOther.m_vfElements.m_vfElements16[14] + m_vfElements.m_vfElements16[12]*matOther.m_vfElements.m_vfElements16[15],

			m_vfElements.m_vfElements16[1]*matOther.m_vfElements.m_vfElements16[0] + m_vfElements.m_vfElements16[5]*matOther.m_vfElements.m_vfElements16[1] + m_vfElements.m_vfElements16[9]*matOther.m_vfElements.m_vfElements16[2] + m_vfElements.m_vfElements16[13]*matOther.m_vfElements.m_vfElements16[3],
			m_vfElements.m_vfElements16[1]*matOther.m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[5]*matOther.m_vfElements.m_vfElements16[5] + m_vfElements.m_vfElements16[9]*matOther.m_vfElements.m_vfElements16[6] + m_vfElements.m_vfElements16[13]*matOther.m_vfElements.m_vfElements16[7],
			m_vfElements.m_vfElements16[1]*matOther.m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[5]*matOther.m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[9]*matOther.m_vfElements.m_vfElements16[10] + m_vfElements.m_vfElements16[13]*matOther.m_vfElements.m_vfElements16[11],
			m_vfElements.m_vfElements16[1]*matOther.m_vfElements.m_vfElements16[12] + m_vfElements.m_vfElements16[5]*matOther.m_vfElements.m_vfElements16[13] + m_vfElements.m_vfElements16[9]*matOther.m_vfElements.m_vfElements16[14] + m_vfElements.m_vfElements16[13]*matOther.m_vfElements.m_vfElements16[15],

			m_vfElements.m_vfElements16[2]*matOther.m_vfElements.m_vfElements16[0] + m_vfElements.m_vfElements16[6]*matOther.m_vfElements.m_vfElements16[1] + m_vfElements.m_vfElements16[10]*matOther.m_vfElements.m_vfElements16[2] + m_vfElements.m_vfElements16[14]*matOther.m_vfElements.m_vfElements16[3],
			m_vfElements.m_vfElements16[2]*matOther.m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[6]*matOther.m_vfElements.m_vfElements16[5] + m_vfElements.m_vfElements16[10]*matOther.m_vfElements.m_vfElements16[6] + m_vfElements.m_vfElements16[14]*matOther.m_vfElements.m_vfElements16[7],
			m_vfElements.m_vfElements16[2]*matOther.m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[6]*matOther.m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[10]*matOther.m_vfElements.m_vfElements16[10] + m_vfElements.m_vfElements16[14]*matOther.m_vfElements.m_vfElements16[11],
			m_vfElements.m_vfElements16[2]*matOther.m_vfElements.m_vfElements16[12] + m_vfElements.m_vfElements16[6]*matOther.m_vfElements.m_vfElements16[13] + m_vfElements.m_vfElements16[10]*matOther.m_vfElements.m_vfElements16[14] + m_vfElements.m_vfElements16[14]*matOther.m_vfElements.m_vfElements16[15],

			m_vfElements.m_vfElements16[3]*matOther.m_vfElements.m_vfElements16[0] + m_vfElements.m_vfElements16[7]*matOther.m_vfElements.m_vfElements16[1] + m_vfElements.m_vfElements16[11]*matOther.m_vfElements.m_vfElements16[2] + m_vfElements.m_vfElements16[15]*matOther.m_vfElements.m_vfElements16[3],
			m_vfElements.m_vfElements16[3]*matOther.m_vfElements.m_vfElements16[4] + m_vfElements.m_vfElements16[7]*matOther.m_vfElements.m_vfElements16[5] + m_vfElements.m_vfElements16[11]*matOther.m_vfElements.m_vfElements16[6] + m_vfElements.m_vfElements16[15]*matOther.m_vfElements.m_vfElements16[7],
			m_vfElements.m_vfElements16[3]*matOther.m_vfElements.m_vfElements16[8] + m_vfElements.m_vfElements16[7]*matOther.m_vfElements.m_vfElements16[9] + m_vfElements.m_vfElements16[11]*matOther.m_vfElements.m_vfElements16[10] + m_vfElements.m_vfElements16[15]*matOther.m_vfElements.m_vfElements16[11],
			m_vfElements.m_vfElements16[3]*matOther.m_vfElements.m_vfElements16[12] + m_vfElements.m_vfElements16[7]*matOther.m_vfElements.m_vfElements16[13] + m_vfElements.m_vfElements16[11]*matOther.m_vfElements.m_vfElements16[14] + m_vfElements.m_vfElements16[15]*matOther.m_vfElements.m_vfElements16[15] );
        }

	const Matrix & operator*=(const Matrix & matOther)
	{
		*this = (*this) * matOther;
		return *this;
        }

	const Vector operator* (const Vector &vecOther) const
	{
		const float fW = m_vfElements.m_vfElements16[3]*vecOther.GetX() + m_vfElements.m_vfElements16[7]*vecOther.GetY() + m_vfElements.m_vfElements16[11]*vecOther.GetZ() + m_vfElements.m_vfElements16[15];
		return Vector(
			(m_vfElements.m_vfElements16[0]*vecOther.GetX() + m_vfElements.m_vfElements16[4]*vecOther.GetY() + m_vfElements.m_vfElements16[8]*vecOther.GetZ() + m_vfElements.m_vfElements16[12]) / fW, 
			(m_vfElements.m_vfElements16[1]*vecOther.GetX() + m_vfElements.m_vfElements16[5]*vecOther.GetY() + m_vfElements.m_vfElements16[9]*vecOther.GetZ() + m_vfElements.m_vfElements16[13]) / fW,
			(m_vfElements.m_vfElements16[2]*vecOther.GetX() + m_vfElements.m_vfElements16[6]*vecOther.GetY() + m_vfElements.m_vfElements16[10]*vecOther.GetZ() + m_vfElements.m_vfElements16[14]) / fW );
        }

private:

	union
	{
		float m_vfElements16[16];
		float m_vfElements44[4][4];
	} m_vfElements;
};

/*inline std::ostream & operator << (std::ostream & os, const Matrix & matMatrix)
{
	os	<< "((" << matMatrix.Get(0,0) << ";" << matMatrix.Get(0,1) << ";" << matMatrix.Get(0,2) << ";" << matMatrix.Get(0,3) << ")" << ";"
		<< "(" << matMatrix.Get(1,0) << ";" << matMatrix.Get(1,1) << ";" << matMatrix.Get(1,2) << ";" << matMatrix.Get(1,3) << ")" << ";"
		<< "(" << matMatrix.Get(2,0) << ";" << matMatrix.Get(2,1) << ";" << matMatrix.Get(2,2) << ";" << matMatrix.Get(2,3) << ")" << ";"
		<< "(" << matMatrix.Get(3,0) << ";" << matMatrix.Get(3,1) << ";" << matMatrix.Get(3,2) << ";" << matMatrix.Get(3,3) << "))";
	return os;
}

inline std::istream & operator>> (std::istream & is,  Matrix & matMatrix)
{
	float f00, f01, f02, f03;
	float f10, f11, f12, f13;
	float f20, f21, f22, f23;
	float f30, f31, f32, f33;

	if (is 
		>> eat ("(")
		>> eat("(") >> f00 >> eat(";") >> f01 >> eat(";") >> f02 >> eat(";") >> f03 >> eat(")") >> eat(";")
		>> eat("(") >> f10 >> eat(";") >> f11 >> eat(";") >> f12 >> eat(";") >> f13 >> eat(")") >> eat(";")
		>> eat("(") >> f20 >> eat(";") >> f21 >> eat(";") >> f22 >> eat(";") >> f23 >> eat(")") >> eat(";")
		>> eat("(") >> f30 >> eat(";") >> f31 >> eat(";") >> f32 >> eat(";") >> f33 >> eat(")") 
		>> eat(")"))
	{
		matMatrix.Set(0,0,f00);	matMatrix.Set(0,1,f01);	matMatrix.Set(0,2,f02);	matMatrix.Set(0,3,f03);
		matMatrix.Set(1,0,f10);	matMatrix.Set(1,1,f11);	matMatrix.Set(1,2,f12);	matMatrix.Set(1,3,f13);
		matMatrix.Set(2,0,f20);	matMatrix.Set(2,1,f21);	matMatrix.Set(2,2,f22);	matMatrix.Set(2,3,f23);
		matMatrix.Set(3,0,f30);	matMatrix.Set(3,1,f31);	matMatrix.Set(3,2,f32);	matMatrix.Set(3,3,f33);

	}

	return is;
}*/
