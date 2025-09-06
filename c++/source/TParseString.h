#pragma once
#include "Timepath_header.h"


/*!

\brief It adds parsing functionality to a string
\date 1998
*/

class TParseString : public string  
{
	public:
		TParseString();
		TParseString(string name);

		bool		ParseBracket(TParseString &str);
		bool		ParseString(string &str, char separator = ' ');
		int		ParseInteger();
		double	ParseDouble();
		void		OverreadLeadingSpaces();

		bool		HasErrorOccured() const;
		size_t		GetPosition() const;
		void		SetPosition(size_t pos);

		void	Replace(char source, char dest);
		
		string	GetRestString();

	private:
		size_t	m_iPosition;
		bool	m_bErrorOccured;
};

