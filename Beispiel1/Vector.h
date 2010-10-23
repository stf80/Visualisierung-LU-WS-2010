#pragma once

#include "common.h"
#include <math.h>

// 3-Vector class
class Vector
{
public:
	Vector()
	{
		m_vfElements[0] = 0.0f;
		m_vfElements[1] = 0.0f;
		m_vfElements[2] = 0.0f;
        }

	Vector(const float fX, const float fY, const float fZ)
	{
		m_vfElements[0] = fX;
		m_vfElements[1] = fY;
		m_vfElements[2] = fZ;
        }

        Vector(const float vfValues[])
	{
		m_vfElements[0] = vfValues[0];
		m_vfElements[1] = vfValues[1];
		m_vfElements[2] = vfValues[2];
        }

	~Vector()
	{
        }

	void Set(const float fX, const float fY, const float fZ)
	{
		m_vfElements[0] = fX;
		m_vfElements[1] = fY;
		m_vfElements[2] = fZ;
        }

	void SetX(const float fX)
	{
		m_vfElements[0] = fX;
        }

	void SetY(const float fY)
	{
		m_vfElements[1] = fY;
        }

	void SetZ(const float fZ)
	{
		m_vfElements[2] = fZ;
        }

	const float * Get() const
	{
		return m_vfElements;
        }

	const float Get(const unsigned int uIndex) const
	{
		return m_vfElements[uIndex];
        }

	const float GetX() const
	{
		return m_vfElements[0];
        }

	const float GetY() const
	{
		return m_vfElements[1];
        }

	const float GetZ() const
	{
		return m_vfElements[2];
        }

	const float GetMagnitude() const
	{
		return sqrtf(GetX()*GetX() + GetY()*GetY() + GetZ()*GetZ());
        }

	const float GetSquaredMagnitude() const
	{
		return (GetX()*GetX() + GetY()*GetY() + GetZ()*GetZ());
        }

	const float GetDot(const Vector &vecOther) const
	{
		return (GetX() * vecOther.GetX() + GetY()*vecOther.GetY() + GetZ()*vecOther.GetZ());
        }

	const Vector GetCross(const Vector &vecOther) const
	{
		return Vector(GetY()*vecOther.GetZ() - GetZ()*vecOther.GetY(), GetZ()*vecOther.GetX() - GetX() * vecOther.GetZ(), GetX()*vecOther.GetY() - GetY()*vecOther.GetX()); 
        }

	const Vector GetNormalized() const
	{
		Vector vecNew = *this;
		float fMagnitude = vecNew.GetMagnitude();

		if (fMagnitude > 0.0f)
			vecNew /= fMagnitude;

		return vecNew;
        }

	const Vector GetInverse() const
	{
		return Vector(1.0f/GetX(),1.0f/GetY(),1.0f/GetZ());
        }

	void normalize()
	{
		(*this) = GetNormalized();
        }

	void invert()
	{
		(*this) = GetInverse();
        }

	const bool operator==(const Vector & vecOther) const
	{
		return (GetX() == vecOther.GetX()) && (GetY() == vecOther.GetY()) && (GetZ() == vecOther.GetZ());
        }

	const bool operator!=(const Vector & vecOther) const
	{
		return !(*this == vecOther);
        }

	float & operator[](const unsigned int uIndex)
	{
		return m_vfElements[uIndex];
        }

	const float operator[](const unsigned int uIndex) const
	{
		return m_vfElements[uIndex];
        }

	const Vector & operator+=(const Vector & vecOther)
	{
		SetX(GetX()+vecOther.GetX());
		SetY(GetY()+vecOther.GetY());
		SetZ(GetZ()+vecOther.GetZ());
		return *this;
        }

	const Vector & operator-=(const Vector & vecOther)
	{
		SetX(GetX()-vecOther.GetX());
		SetY(GetY()-vecOther.GetY());
		SetZ(GetZ()-vecOther.GetZ());
		return *this;
        }

	const Vector & operator*=(const Vector & vecOther)
	{
		SetX(GetX()*vecOther.GetX());
		SetY(GetY()*vecOther.GetY());
		SetZ(GetZ()*vecOther.GetZ());
		return *this;
        }

	const Vector & operator*=(const float & fOther)
	{
		SetX(GetX()*fOther);
		SetY(GetY()*fOther);
		SetZ(GetZ()*fOther);
		return *this;
        }

	const Vector & operator/=(const Vector & vecOther)
	{
		SetX(GetX()/vecOther.GetX());
		SetY(GetY()/vecOther.GetY());
		SetZ(GetZ()/vecOther.GetZ());
		return *this;
        }

	const Vector & operator/=(const float & fOther)
	{
		SetX(GetX()/fOther);
		SetY(GetY()/fOther);
		SetZ(GetZ()/fOther);
		return *this;
        }

	const Vector operator+(const Vector & vecOther) const
	{
		Vector vecNew = *this;
		vecNew += vecOther;
		return vecNew;
        }

	const Vector operator-(const Vector & vecOther) const
	{
		Vector vecNew = *this;
		vecNew -= vecOther;
		return vecNew;
        }

	const Vector operator*(const Vector & vecOther) const
	{
		Vector vecNew = *this;
		vecNew *= vecOther;
		return vecNew;
        }

	const Vector operator*(const float & fOther) const
	{
		Vector vecNew = *this;
		vecNew *= fOther;
		return vecNew;
        }

	const Vector operator/(const Vector & vecOther) const
	{
		Vector vecNew = *this;
		vecNew /= vecOther;
		return vecNew;
        }

	const Vector operator/(const float & fOther) const
	{
		Vector vecNew = *this;
		vecNew /= fOther;
		return vecNew;
        }

	const Vector operator-() const
	{
		return Vector(-GetX(),-GetY(),-GetZ());
        }

private:
	float m_vfElements[3];
};

/*inline std::ostream & operator<< (std::ostream & os, const Vector & vecVector)
{
	os << "(" << vecVector.GetX() << ";" << vecVector.GetY() << ";" << vecVector.GetZ() << ")";
	return os;
}

inline std::istream & operator>> (std::istream & is, Vector & vecVector)
{

	float fX,fY,fZ;
	if (is >> eat("(") >> fX >> eat(";") >> fY >> eat(";") >> fZ >> eat(")"))
		vecVector.Set(fX,fY,fZ);

	return is;
}*/
