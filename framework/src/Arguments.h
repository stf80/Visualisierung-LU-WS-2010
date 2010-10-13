#pragma once

#include "common.h"
#include <iostream>
#include <map>


// Utility class for accessing command-line arguments, ignores everything apart from "-<optionname> <optionvalue>
class Arguments
{
	friend std::ostream & operator << (std::ostream & os, const Arguments & argArguments);

public:

	Arguments(int argc = 0, char **argv = NULL) : m_strApplicationFilename("VisLuFramework.exe")
	{
		if (argc > 0)
		{
			m_strApplicationFilename = argv[0];

			for (int i=1;i<argc;i++)
			{
				const std::string strArgument = trim(argv[i]);

				if (strArgument.size() > 1)
				{
					if (strArgument[0] == '-')
					{
						const std::string strOptionName = trim(strArgument.substr(1,strArgument.size()-1));

						if (i < argc-1)
						{
							const std::string strOptionValue = trim(argv[i+1]);
							m_mapOptions[strOptionName] = strOptionValue;
							i++;
						}
					}

				}
			}
		}
	};

	~Arguments()
	{

	};

	void SetApplicationFilename(const std::string & strApplicationFilename)
	{
		m_strApplicationFilename = strApplicationFilename;
	};

	const std::string & GetApplicationFilename() const
	{
		return m_strApplicationFilename;
	};

	const bool HasOption(const std::string & strOptionName) const
	{
		return m_mapOptions.find(strOptionName) != m_mapOptions.end();
	};

	void SetOption(const std::string & strOptionName, const std::string & strOptionValue)
	{
		m_mapOptions[strOptionName] = strOptionValue;
	};

	const std::string GetOption(const std::string & strOptionName) const
	{
		std::map<std::string,std::string>::const_iterator i = m_mapOptions.find(strOptionName);

		if (i != m_mapOptions.end())
			return i->second;

		return std::string();
	};

private:
	std::string m_strApplicationFilename;
	std::map<std::string,std::string> m_mapOptions;
};

inline std::ostream & operator << (std::ostream & os, const Arguments & argArguments)
{
	if (argArguments.GetApplicationFilename().find(" ") != std::string::npos)
		os << "\"" << argArguments.GetApplicationFilename() << "\"";
	else
		os << argArguments.GetApplicationFilename();


	for (std::map<std::string,std::string>::const_iterator i=argArguments.m_mapOptions.begin();i!=argArguments.m_mapOptions.end();i++)
	{
		if (i->second.find(" ") != std::string::npos)
			os << " -" << i->first << " " << "\"" << i->second << "\"";
		else
			os << " -" << i->first << " " << i->second;
	}

	return os;
}

