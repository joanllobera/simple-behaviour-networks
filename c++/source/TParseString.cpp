
#include "TParseString.h"

#include <math.h>

TParseString::TParseString() 
{
	m_iPosition = 0; 
}

TParseString::TParseString(string name) : string(name) 
{
	m_iPosition = 0; 
}


/*!

\brief It returns the content between two brackets
\return It returns the content between two brackets
\date 10.11.97

\param reference to the current position in the string
			  reference to the string that gets the content of the bracket
\details  returns the content between two brackets. iposition has to be the 
				 position of an opening bracket. This function allows nested brackets.
\todo find out why it is never called, and call when parsing rules!!

*/


bool TParseString::ParseBracket(TParseString &retString)
{
	int		iNoOfOpenBrackets = 1;
	size_t 	iRightPosition = m_iPosition + 1;	

	size_t		iLen = size();

	m_bErrorOccured = FALSE;
	// iposition must index an opening bracket

	if ((m_iPosition >= iLen) || this->at(m_iPosition) != '(' )
		return FALSE;

	while (iNoOfOpenBrackets > 0)
	{
		// reached the end of the string ? error
		if (iRightPosition >= iLen)
			return FALSE;

		// look at the next character
		switch (at(iRightPosition))
		{
			case '(':
				iNoOfOpenBrackets++;
				break;

			case ')':
				iNoOfOpenBrackets--;
				break;
		}
		iRightPosition++;
	}
	
	// only the content within the brackets
	// overread a space following the last closing bracket

	retString=substr(m_iPosition + 1, iRightPosition - m_iPosition - 2);

	m_iPosition = iRightPosition + 1;
	return TRUE;
}


/*!

\brief checks if there is a valid string
\return  TRUE, if there has been a valid string
\date 02.2014

\param reference to the current position in the string
			  reference to the string to parse into

			  \details reads in a string from strCurrentMsg at Position iPosition into str.
				 terminator is either ' ' or ')'. iPosition points behind terminator
*/



bool TParseString::ParseString(string &str, char separator)
{
	size_t		iRightPos;
	bool		bReady = FALSE;
	size_t		len = size();
	char		tmp;
	bool	withinQuotes = FALSE;

	m_bErrorOccured = FALSE;
	OverreadLeadingSpaces();
	iRightPos = m_iPosition + 1;

	if (m_iPosition  >= len)
	{
		// the expected string is not there
		m_iPosition++;
		return FALSE;
	}

	// search for a ' ' or ')'
	bReady = FALSE;
	while (!bReady && (iRightPos < len))
	{
	
		tmp = at(iRightPos);
		if (tmp == separator)
			bReady = TRUE;
		else
			switch (tmp)
			{
				case ' ':
				case ')':
				case '\r':
				case '\n':
				case '\t':
					if (withinQuotes)
						iRightPos++;
					else
						bReady = TRUE;
					break;
				
				case '\"':
					withinQuotes = !withinQuotes;
					iRightPos++;
					break;

				default:
					iRightPos++;
			}
	}

	// get the string
	//str = Mid(m_iPosition, iRightPos - m_iPosition);
	str = substr(m_iPosition, iRightPos - m_iPosition);

	m_iPosition = iRightPos + 1;

	return TRUE;
}





/*!

\brief reads in a signed integer value
\return TRUE, if a number has been in str at iPosition
\date 1997
\details reads in a signed integer value. assumes a ' ' at the end of the 
				 number, which is overread
\sa ParseDouble
*/


int TParseString::ParseInteger()
{
	int	iValue = 0;
	bool	bSign = FALSE;
	bool	bReady = FALSE;
	char	cChar;
	size_t	len = size();

	m_bErrorOccured = TRUE;
	OverreadLeadingSpaces();
	// read in the number
	while (!bReady && (m_iPosition < len))
	{
		cChar = at(m_iPosition);
		switch(cChar)
		{
			case '-':
				// only one - sign expected
				bSign = TRUE;
				m_iPosition++;
				break;
			
			case '+':
				m_iPosition++;
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				iValue = 10 * iValue + cChar - '0';
				m_bErrorOccured = FALSE;
				m_iPosition++;
				break;
			
			case ')':
			case ' ':
			case '\t':
				m_iPosition++;
				// no break here

			default:
				bReady = TRUE;
		}
	}
	
	// convert the number if neccessary
	if (bSign)
		iValue = -iValue;
	
	return iValue;
}


/*!

\brief reads in a double value
\return value of the number
\date 1997

\param
\todo
\details  reads in a double value. assumes a ' ' at the end of the 
				 number, which is overread
\sa ParseInteger
*/

double TParseString::ParseDouble()
{
	double	dValue = 0.0;
	double	dNumber;
	bool		bSign = FALSE;
	bool		bReady = FALSE;
	bool		bDot = FALSE;
	double	dDivisor = 1.0;
	char		cChar;
//	int		len = GetLength();
	size_t		len = size();
	double	exponent;

	m_bErrorOccured = TRUE;
	// read in the number
	while (!bReady && (m_iPosition < len))
	{
//		cChar = GetAt(m_iPosition);
				cChar = at(m_iPosition);
		switch(cChar)
		{
			case '-':
				// only one - sign expected
				bSign = TRUE;
				m_iPosition++;
				break;
			
			case '+':
				m_iPosition++;
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				dValue = 10.0 * dValue + (double) (cChar - '0');
				dDivisor *= 10.0;
				m_bErrorOccured = FALSE;
				m_iPosition++;
				break;
			
			case '.':
			case ',':
				dDivisor = 1.0;
				bDot = TRUE;
				m_iPosition++;
				break;

			case ')':
			case ' ':
			case '\t':
				m_iPosition++;
				bReady = TRUE;
				break;

			case 'e':
			case 'E':
				// exponent may not be at the beginning
				if (!m_bErrorOccured)
				{
					m_iPosition++;
					exponent = ParseDouble();
					if (!m_bErrorOccured)
						dValue *= pow(10.0, exponent);
				}
				bReady = TRUE;
				break;

			default:
				bReady = TRUE;
		}
	}
	
	// convert the number if neccessary
	if (bDot)
		dNumber = dValue / dDivisor;
	else
		dNumber = dValue;

	if (bSign)
		dNumber = -dNumber;
	
	return dNumber;
}

/*-----------------------------------------------------------------------------
Function: void TParseString::OverreadLeadingSpaces()
Date    : 14.01.98
-------------------------------------------------------------------------------
Parameter: none
Returnval: (void)
Description: overreads leading spaces starting from m_iPosition and ending, when
			 a character other than SPACE or the end of the string is reached
-----------------------------------------------------------------------------*/


/*!

\brief overreads leading spaces
\date 1997

\details overreads leading spaces starting from m_iPosition and ending, when
			 a character other than SPACE or the end of the string is reached
*/


void TParseString::OverreadLeadingSpaces()
{
	char		tmp;

	// overread inital spaces, returns and new lines
		while (m_iPosition < size())
	{
		tmp = at(m_iPosition);
		switch (tmp)
		{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				m_iPosition++;
				break;
		
			default:
				return;
		}
	}
}


/*!

\brief sets the current parse position to the passed position.
\date 1998

*/


void TParseString::SetPosition(size_t pos)
{
		if (pos >= size())
	{
		m_iPosition = size() - 1;
		if (m_iPosition < 0)
			m_iPosition = 0;
	}	
	else if (pos < 0)
		m_iPosition = 0;
	else
		m_iPosition = pos; 
}


/*!

\brief returns the rest of the string
\return returns the rest of the string
\date 1998
*/


string TParseString::GetRestString()
{
	string temp=substr( m_iPosition,size()-m_iPosition);
	return temp;
}


/*-----------------------------------------------------------------------------
Function: void TParseString::Replace(char source, char dest)
Date    : 23.10.98
-------------------------------------------------------------------------------
Parameter: source: character, that should be replaced
			  dest: character to replace with
Returnval: (void)
Description: replaces each occurence of source with dest
-----------------------------------------------------------------------------*/


/*!

\brief replaces each occurence of source with dest
\date 1998

\param source: character, that should be replaced
			  dest: character to replace with
\sa
*/

void TParseString::Replace(char source, char dest)
{
	size_t	pos;

	while ((pos = find(source)) >= 0)
		replace(pos, 1,(const char*) dest);
		//		SetAt(pos, dest);
}



bool TParseString::HasErrorOccured() const 
{
	return m_bErrorOccured;
}

size_t TParseString::GetPosition() const 
{
	return m_iPosition;
}
