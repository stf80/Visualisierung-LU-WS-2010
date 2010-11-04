#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>


#define PI 3.14159265f
#define BOUNDARY 0.9125f

// converts from string to Type
template <class Type>
inline const Type fromString(const std::string & strString)
{
	Type tValue;
	std::stringstream(std::string(strString)) >> tValue;		
	return tValue;
};

// converts from string to Type, returns true if successful
template <class Type>
inline bool fromString(Type & tValue, const std::string & strString)
{
	Type tTemp;

	std::stringstream ss;
	ss << strString;
	ss >> std::ws;

	if (ss >> tTemp)
	{
		ss >> std::ws;

		if (ss.eof())
		{
			tValue = tTemp;
			return true;
		}
	}

	return false;
};

// converts from Type to string
template <class Type>
inline const std::string toString(const Type & tValue)
{
	std::stringstream ss;
	ss << tValue;
	return ss.str();
};

// trims whitespace form the beginning of the string
inline const std::string trimLeft( const std::string &str, const std::string &strWhitespace = "\n\r\t ")
{
	size_t uIndex = str.find_first_not_of(strWhitespace);
	if( uIndex != std::string::npos )
		return str.substr(uIndex);

	return "";
};

// trims whitespace form the end of the string
inline const std::string trimRight( const std::string &str, const std::string &strWhitespace = "\n\r\t ")
{
	size_t  uIndex = str.find_last_not_of(strWhitespace);
	if( uIndex != std::string::npos )
		return str.substr(0,uIndex+1);

	return str;
};

// trims whitespace from beginning and end of the string
inline const std::string trim( const std::string &str, const std::string & strWhitespace = "\n\r\t ")
{
	return trimRight(trimLeft(str,strWhitespace),strWhitespace);
};


// Utility class for STL stream extraction operator
class eat
{
	friend std::istream & operator>>(std::istream &is, eat & e);

public:
	eat(const std::string & strString) : m_strString(strString)
	{
        }

private:
	void process(std::istream & is)
	{
                const unsigned int uLength = (unsigned int)m_strString.size();

		if (uLength > 0)
		{
			char c; 
			unsigned int uIndex = 0; 

			is >> std::ws;
			std::ios::fmtflags f = is.flags();
			is.unsetf(std::ios::skipws);

			do
			{
				if (!(is >> c)) 
				{
					break;
				}
				else
				{
					if (c != m_strString[uIndex])
					{
						is.setstate(std::ios::failbit);
						break;
					}
				}
			}
			while (++uIndex < uLength);

			is.flags(f);

			if (!is.fail())
				is >> std::ws;
		}
        }

private:
	std::string m_strString;
};

inline std::istream & operator>> (std::istream &is, eat & e)
{
	e.process(is);
	return is;
}
