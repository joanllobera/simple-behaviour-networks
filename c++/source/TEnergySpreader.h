
#pragma once

#include "Timepath_header.h"





/*!

\brief The basic class for all the modules spreading energy to any other module

\details

Behavior networks are, essentially, an energy spreading mechanism.

This is the basic class doing such task

*/




class TEnergySpreader : public TObject  
{
	public:
		

		TEnergySpreader();
		TEnergySpreader( string name, int index );
		virtual ~TEnergySpreader();

		void		Initialize();

		// energy functions
		virtual double		GetActivation() { return m_dMainActivation; };
		double				SetActivation(double activation);
	

		// notification
		void		OnAddedToDestination() { m_iNoOfDestinations++; };
		void		OnRemovedFromDestination() { m_iNoOfDestinations--; };

		// get and set functions
		virtual double 	GetMainActivation() const { return m_dMainActivation; };
		double	GetMaxActivation() const { return m_dMaxActivation; };
		void		SetMaxActivation(double activation) { m_dMaxActivation = activation; };
		string	GetName() { return m_strName; };
		void		SetName(string &str) { m_strName = str; };
		int		GetIndex() const { return m_iIndex; };
		void		SetIndex(int index) { m_iIndex = index; };

		bool		IsSelected(void) const { return m_bSelected; };

		void		SetSelected(bool sel = true) { m_bSelected = sel; };

	protected:

		string	m_strName;
		int		m_iIndex;
		double	m_dMainActivation;
		double	m_dActivationChange;
		double	m_dMaxActivation;
		int		m_iNoOfDestinations;



		bool		m_bSelected;

	private:

};


