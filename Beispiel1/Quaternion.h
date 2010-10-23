#pragma once

#include "common.h"
#include <math.h>
#include "Vector.h"

// Unit quaternion class
class Quaternion
{
public:
	Quaternion()
	{
		Set(Vector(0.0f,0.0f,0.0f),0.0f);
	};

	Quaternion(const Vector &vecV, const float fS)
	{
		Set(vecV,fS);
	};

	Quaternion(const Vector &vecV)
	{
		Set(vecV,0.0f);
	};

	Quaternion(const float fAngle, const Vector &vecAxis)
	{
		float fD = fAngle * 0.5f;
		Set(vecAxis.GetNormalized() * sinf(fD),cosf(fD));
	};

	Quaternion(const Vector &vecFrom, const Vector &vecTo)
	{
		const float fAngle = acosf(vecFrom.GetNormalized().GetDot(vecTo.GetNormalized()));
		const Vector vecAxis = vecFrom.GetNormalized().GetCross(vecTo.GetNormalized());
		(*this) = Quaternion(fAngle,vecAxis);
	};

	~Quaternion()
	{
	};

	void Set(const Vector &vecV, const float fS)
	{
		m_vecVector = vecV;
		m_fScalar = fS;
		normalize();
	};

	void SetVector(const Vector &vecV)
	{
		Set(vecV,GetScalar());
	};

	void SetScalar(const float fS)
	{
		Set(GetVector(),fS);
	};

	const Vector & GetVector() const
	{
		return m_vecVector;
	};

	const float & GetScalar() const
	{
		return m_fScalar;
	};

	const float GetMagnitude() const
	{
		return sqrtf( GetScalar()*GetScalar() + GetVector().GetMagnitude() * GetVector().GetMagnitude());
	};

	const Quaternion GetNormalized() const
	{
		Quaternion quaNew = *this;
		float fMagnitude = quaNew.GetMagnitude();

		if (fMagnitude > 0.0f)
		{
			quaNew.m_vecVector /= fMagnitude;
			quaNew.m_fScalar /= fMagnitude;
		}

		return quaNew;
	};

	const Quaternion GetInverse() const
	{
		return Quaternion(GetVector()*-1.0f,GetScalar()).GetNormalized();
	};

	void normalize()
	{
		(*this) = GetNormalized();
	};

	void invert()
	{
		(*this) = GetInverse();
	};

	const bool operator==(const Quaternion & quaOther) const
	{
		return (GetVector() == quaOther.GetVector()) && (GetScalar() == quaOther.GetScalar());
	};

	const bool operator!=(const Quaternion & quaOther) const
	{
		return !(*this == quaOther);
	};

	const Quaternion & operator*=(const float & fOther)
	{
		Set(GetVector() * fOther,GetScalar() * fOther);
		return *this;
	};

	const Quaternion & operator/=(const float & fOther)
	{
		Set(GetVector() / fOther,GetScalar() / fOther);
		return *this;
	};

	const Quaternion & operator+=(const Quaternion & quaOther)
	{
		Set(GetVector() + quaOther.GetVector(),GetScalar() + quaOther.GetScalar());
		return *this;
	};

	const Quaternion & operator-=(const Quaternion & quaOther)
	{
		Set(GetVector() - quaOther.GetVector(),GetScalar() - quaOther.GetScalar());
		return *this;
	};

	const Quaternion & operator*=(const Quaternion& quaOther)
	{
		Set(
			GetVector().GetCross(quaOther.GetVector()) + GetVector() * quaOther.GetScalar()  + quaOther.GetVector() * GetScalar(),
			GetScalar() * quaOther.GetScalar() - GetVector().GetDot(quaOther.GetVector()));
		return *this;
	};

	const Quaternion operator*(const float fOther) const
	{
		Quaternion quaNew = *this;
		quaNew *= fOther;
		return quaNew;
	};

	const Quaternion operator/(const float fOther) const
	{
		Quaternion quaNew = *this;
		quaNew *= fOther;
		return quaNew;
	};

	const Quaternion operator+(const Quaternion & quaOther) const
	{
		Quaternion quaNew = *this;
		quaNew += quaOther;
		return quaNew;
	};

	const Quaternion operator-(const Quaternion & quaOther) const
	{
		Quaternion quaNew = *this;
		quaNew += quaOther;
		return quaNew;
	};

	const Quaternion operator*(const Quaternion & quaOther) const
	{
		Quaternion quaNew = *this;
		quaNew *= quaOther;
		return quaNew;
	};

	const Vector operator*(const Vector &vecOther) const
	{
		Quaternion quaRotated = (*this * Quaternion(vecOther) * GetInverse());
		return quaRotated.GetVector() * vecOther.GetMagnitude();
	};

private:
	Vector m_vecVector;
	float m_fScalar;
};


/*inline std::ostream & operator << (std::ostream & os, const Quaternion & quaQuaternion)
{
	os << "(" << quaQuaternion.GetVector() << ";" << quaQuaternion.GetScalar() << ")";
	return os;
}

inline std::istream & operator>> (std::istream & is, Quaternion & quaQuaternion)
{
	Vector vecV;
	float fS;

	if (is >> eat("(") >> vecV >> eat(";") >> fS >> eat(")"))
		quaQuaternion.Set(vecV,fS);

	return is;
}*/
