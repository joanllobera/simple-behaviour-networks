
#pragma once
#include "Timepath_header.h" 

class TActionResource;
class TDorer2Competence;
#ifdef UNITYAGENT
class TCallbackConverter;
#endif


/*!

\brief class to deal with action callbacks

\date 01.1998
*/



class TAction : public TObject  
{
	public:
		TAction(string name, int index, TObject* pObj, PCallBackAction pFn);
#if UNITYAGENT==0
		void		SetAction(string name, int index, TObject* pObj, PCallBackAction pFn);
#else
		TAction(string name, int index, TCallbackConverter* pObj);
		void ResetCallCounter(); 
		int GetCallCount();
#endif

		virtual ~TAction();


		void		PerformAction(double activation);
		string	GetName(void) { return m_strName; };
		int		GetIndex() const { return m_iIndex; };
	//	double	GetDecisiveness() const { return m_dDecisiveness; };
	//	long		GetNoOfActionPerformed() const { return m_lNoOfActionPerformed; };

	protected:
		string				m_strName;
		int					m_iIndex;

#if UNITYAGENT==0
		PCallBackAction	m_pAction;
		TObject*				m_pObject;
#else
		TCallbackConverter* m_pObject;


#endif

		double			m_dMinActivation;
		double			m_dMaxActivation;
		double			m_dMeanActivation;
		double			m_dVarianz;
		long				m_lNoOfActionPerformed;
		double			m_dDecisiveness;


#if STORIES
		TActionResource* pActRes;
public:
		void TAction::setActRes(TActionResource* par){this->pActRes=par;};
		TActionResource* TAction::getActRes(){return pActRes;};

protected:
	TDorer2Competence* mod;
public:
	void SetModule(TDorer2Competence* m){
		this->mod=m;
	};
	TDorer2Competence* GetModule(){return mod;};

#endif





};

