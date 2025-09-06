//#include "StdAfx.h"
#include ".\threadDesc.h"
//#include ".\ARpfm.h"


IMPLEMENT_DYNCREATE(threadDesc, CThread)

threadDesc::threadDesc(void)
{
}

threadDesc::~threadDesc(void)
{
}


int threadDesc::Run_Thread()
{ 
/*
	switch(_type){
		case CThread::ThreadType::threadDescrip :
			{
				feature* feat = NULL;
				struct detection_data* ddata = NULL;
				

				for(int i = _init; i < _end; i++ )
				{
					//feat = CV_GET_SEQ_ELEM( feature,_features, i );//aki no es pot optimitzar res
					feat = _features[i];
					ddata = feat_detection_data( feat );//aki tampoc
					naturalFeature::DescrHist(_hist, _gaussPyr[ddata->octv][ddata->intvl], ddata->r,ddata->c, feat->ori, ddata->scl_octv );//akesta es la puta
					naturalFeature::HistToDescr(_hist,  feat, _idRef );
					naturalFeature::ReleaseDescrHist( _hist);
				}
				
			}
			break;
		case CThread::ThreadType::threadGaussPyr :
			{
				if(_octvs == 0){
					//_gaussPyr[0][0] = cvCreateImage(cvSize(_base->width,_base->height),_base->depth,_base->nChannels);
					_gaussPyr[0][0] = cvCloneImage(_base);
					int g = 0;
				}else{
					_gaussPyr[_octvs][0] = downsample2(_base,_octvs);
				}
				for(int i = 1; i< _intvls + 3 ; i++){///,	img->depth, img->nChannels );
					//_gaussPyr[_octvs][i] = cvCreateImage( cvGetSize(_gaussPyr[_octvs][i-1]),IPL_DEPTH_32F, 1 );
					_gaussPyr[_octvs][i] = cvCreateImage( cvGetSize(_gaussPyr[_octvs][i-1]),_gaussPyr[_octvs][i-1]->depth,_gaussPyr[_octvs][i-1]->nChannels );
					cvSmooth( _gaussPyr[_octvs][i-1],_gaussPyr[_octvs][i],CV_GAUSSIAN, 0, 0, _sigma[i], _sigma[i] );
					//prova de DOG
					//_dogPyr[_octvs][i-1] = cvCreateImage( cvGetSize(_gaussPyr[_octvs][i-1]),	IPL_DEPTH_32F, 1 );
					//cvSub( _gaussPyr[_octvs][i], _gaussPyr[_octvs][i-1], _dogPyr[_octvs][i-1], NULL );
				}
			}
			break;
	}
*/
	m_bPause = true;
	*m_pFinish = true;
	
	return 1;
}

void threadDesc::End()
{
	if(WaitForThread(INFINITE) == WAIT_OBJECT_0)
	{
		SuspendThread();
		ReleaseMutex(m_hMutex);
	}
}

void threadDesc::setParamDESC(int tipo,  int start,int endo ,int id)
{
	_type = tipo;
	_init = start;

	_end= endo;
	_idRef = id;

	

}
void threadDesc::setParamPYR(int tipo)
{
	_type = tipo;
	_init = NULL;
	_end= NULL;
	_idRef = NULL;
	


}