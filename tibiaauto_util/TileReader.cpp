// TileReader.cpp: implementation of the CTileReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tibiaauto_util.h"
#include "TileReader.h"
#include "Util.h"
#include <time.h>

#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>


XERCES_CPP_NAMESPACE_USE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
int CTileReader::xmlInitialised=0;
CTibiaTile *CTileReader::tiles[MAX_TILES];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileReader::CTileReader()
{
	if (!xmlInitialised)
	{
		XMLPlatformUtils::Initialize();
		xmlInitialised=1;
		loadTiles();
	}
}

CTileReader::~CTileReader()
{
	
}

CTibiaTile *CTileReader::getTile(int tileNr)
{
	return tiles[tileNr];
}

void CTileReader::loadTiles()
{
	int i;
	for (i=0;i<MAX_TILES;i++)
	{
		tiles[i]=new CTibiaTile();
	}
	
	XercesDOMParser *parser = new XercesDOMParser();
	try
	{	
		char installPath[1024];
		unsigned long installPathLen=1023;
		installPath[0]='\0';
		HKEY hkey=NULL;
		if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Tibia Auto\\",0,KEY_ALL_ACCESS,&hkey))
		{
			RegQueryValueEx(hkey,TEXT("Install_Dir"),NULL,NULL,(unsigned char *)installPath,&installPathLen );
			RegCloseKey(hkey);
		}
		if (!strlen(installPath))
		{
			AfxMessageBox("ERROR! Unable to read TA install directory! Please reinstall!");
			exit(1);
		}
		
		
		int rootNr;
		

		char pathBuf[2048];
		sprintf(pathBuf,"%s\\mods\\tibiaauto-tiles.xml",installPath);
		parser->parse(pathBuf);
		
		DOMNode  *doc = parser->getDocument();	
		int rootCount=doc->getChildNodes()->getLength();		
		for (rootNr=0;rootNr<rootCount;rootNr++)
		{			
			
			DOMNode *root = doc->getChildNodes()->item(rootNr);
				
			if (wcscmp(root->getNodeName(),_L("tiles")))
				continue;					
			DOMNode *item = root->getFirstChild();
			if (item)
			{
				
				do
				{			
					
					if (!wcscmp(item->getNodeName(),_L("tile"))) {
						

						int tileId = CUtil::getNodeIntAttribute(item,_L("id"));
						
						
						tiles[tileId]->blocking=CUtil::getNodeIntAttribute(item,_L("blocking"));						
						tiles[tileId]->canWalkThrough=CUtil::getNodeIntAttribute(item,_L("canWalkThrough"));
						tiles[tileId]->goDown=CUtil::getNodeIntAttribute(item,_L("goDown"));
						tiles[tileId]->goUp=CUtil::getNodeIntAttribute(item,_L("goUp"));
						tiles[tileId]->ground=CUtil::getNodeIntAttribute(item,_L("ground"));
						tiles[tileId]->isContainer=CUtil::getNodeIntAttribute(item,_L("isContainer"));
						tiles[tileId]->isDepot=CUtil::getNodeIntAttribute(item,_L("isDepot"));
						tiles[tileId]->requireRope=CUtil::getNodeIntAttribute(item,_L("requireRope"));
						tiles[tileId]->requireShovel=CUtil::getNodeIntAttribute(item,_L("requireShovel"));
						tiles[tileId]->requireUse=CUtil::getNodeIntAttribute(item,_L("requireUse"));
						tiles[tileId]->speed=CUtil::getNodeIntAttribute(item,_L("speed"));												
						tiles[tileId]->notMoveable=CUtil::getNodeIntAttribute(item,_L("notMoveable"));
						tiles[tileId]->stackable=CUtil::getNodeIntAttribute(item,_L("stackable"));
						tiles[tileId]->alwaysOnTop=CUtil::getNodeIntAttribute(item,_L("alwaysOnTop"));
						tiles[tileId]->moreAlwaysOnTop=CUtil::getNodeIntAttribute(item,_L("moreAlwaysOnTop"));
						
					}		

				} while ((item=item->getNextSibling())!=NULL);
			}
			
		}				
		
	} catch (...)
	{
		AfxMessageBox("Unable to load tile definitions!");
	}

	delete parser;		

}