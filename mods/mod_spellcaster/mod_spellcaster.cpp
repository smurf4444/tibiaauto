/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/


// mod_spellcaster.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "mod_spellcaster.h"

#include "ConfigDialog.h"
#include "ConfigData.h"
#include "TibiaContainer.h"
#include "MemConstData.h"

#include "MemReaderProxy.h"
#include "PackSenderProxy.h"
#include "ModuleUtil.h"
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMod_spellcasterApp

BEGIN_MESSAGE_MAP(CMod_spellcasterApp, CWinApp)
//{{AFX_MSG_MAP(CMod_spellcasterApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// Tool thread function

int toolThreadShouldStop=0;
HANDLE toolThreadHandle;
monster monstersInfo[5000];

int initalizeCreatures();
int isInitializedCreatures();
int getMonsterNumberFromName(char *);

DWORD WINAPI toolThreadProc( LPVOID lpParam )
{		
	CMemReaderProxy reader;
	CPackSenderProxy sender;
	CMemConstData memConstData = reader.getMemConstData();
	CConfigData *config = (CConfigData *)lpParam;
	int currentMonsterNumber = 0;
	char text[128] = {0};

	if (isInitializedCreatures() == 0)
		initalizeCreatures();
	
	while (!toolThreadShouldStop)
	{			
		Sleep(200);	
		CTibiaCharacter *self = reader.readSelfCharacter();
		int attackedCreature = reader.getAttackedCreature();
		
		//T4: First try to heal
		if (config->life && (self->hp<=config->lifeHp || self->hp<=config->vitaHp || self->hp<=config->granHp || self->hp<=config->exuraHp)) {
			// Akilez:	Give 1st priority to custom spells!
			if (config->customSpell && self->hp<=config->lifeHp && self->mana >= config->lifeSpellMana){
				sender.say(config->lifeSpell);
				Sleep(600);
				self = reader.readSelfCharacter();
			}
			else if(config->vitaSpell && self->hp<=config->vitaHp && self->mana >= config->vitaSpellMana){
				sender.say("Exura Vita");
				Sleep(600);
			}
			else if(config->granSpell && self->hp<=config->granHp && self->mana >= config->granSpellMana){
				sender.say("Exura Gran");
				Sleep(600);
			}
			else if((config->exuraSpell && self->hp<=config->exuraHp && self->mana >= config->exuraSpellMana) || (config->paralysisSpell && reader.getSelfEventFlags() & 32 == 32)) {
				sender.say("Exura");
				Sleep(600);
			}
			else {
				sender.sendTAMessage("WARNING!!! Not enough mana to Heal!!!");
				Sleep(600);
			}
		}	
		else if(config->poisonSpell && reader.getSelfEventFlags() & 1 == 1) {
			for (int i = 0; i<6; i++) {
				for (int j = 0; j<10; j++) {
					char word = reader.getMemIntValue(0x76D928+i*10+j); 
					if (word % 0x100 == 0) text[i*10+j] = ' '; 
					text[i*10+j] = word%0x100;
				}				
			}
			if ((strstr(text,"hitpoints.") != 0) || (strstr(text,"hitpoint.") != 0)) {
				char pointLossText[3] = {0};
				pointLossText[0] = text[9];
				if (text[10] != ' ') pointLossText[1] = text[10];
				if (atoi(pointLossText) >= config->minPoisonDmg && atoi(pointLossText) != 5) sender.sayWhisper("exana pox");
			}
		}
		
			//Akilez: Use mana for strike spells
		if(config->strike && self->mana>=config->manaStrike && attackedCreature){
			attackedCreature = reader.getAttackedCreature();
			//T4: If any creature is attacked
			if (attackedCreature) {			
				//T4: Get attacked creature stucture
				CTibiaCharacter *ch = reader.getCharacterByTibiaId(attackedCreature);
				currentMonsterNumber = getMonsterNumberFromName(ch->name);
				if (ch->name && ch->hpPercLeft && currentMonsterNumber > -1) {
					if ((monstersInfo[currentMonsterNumber].hp * ch->hpPercLeft * .01 > config->strikeSpellHpMin) || (currentMonsterNumber == -1))
					{
						//T4: cords
						int chX=ch->x;
						int chY=ch->y;
						int chZ=ch->z;
						
						int xDist = abs(self->x-chX);
						int yDist = abs(self->y-chY);
						int maxDist = xDist;
						if (yDist>maxDist) maxDist=yDist;
						if (maxDist <= 3) {
							
							if (config->flam && monstersInfo[currentMonsterNumber].weakFire) {
								sender.say("Exori Flam");
								Sleep(600);
							}
							else if (config->frigo && monstersInfo[currentMonsterNumber].weakIce) {
								sender.say("Exori Frigo");
								Sleep(600);
							}
							else if (config->tera && monstersInfo[currentMonsterNumber].weakEarth) {
								sender.say("Exori Tera");
								Sleep(600);
							}
							else if (config->vis && monstersInfo[currentMonsterNumber].weakEnergy) {
								sender.say("Exori Vis");
								Sleep(600);
							}
							else if (config->mort && monstersInfo[currentMonsterNumber].weakDeath) {
								sender.say("Exori Mort");
								Sleep(600);
							}
							else if (config->defaultStrikeSpell) {
								sender.say(config->defaultStrikeSpell);
								Sleep(600);
							}
							else {
								sender.sendTAMessage("WARNING!!! No appropriate strike spell configured!");
							}
						}
					}
				}
				delete ch;
				attackedCreature = 0;
			}			
		}	
			//T4: Use mana in other pupouse otherwise
		else if(config->mana && self->mana>=config->manaMana){
			sender.say(config->manaSpell);
			Sleep(600);
		}	
		else{
			// now try to summon creatures
			int chNr;
			int summonCount=0;
			for (chNr=0;chNr<memConstData.m_memMaxCreatures;chNr++) {
				CTibiaCharacter *ch = reader.readVisibleCreature(chNr);
				
				if (ch->z==self->z&&ch->visible&&!strcmpi(config->summonName,ch->name))
					summonCount++;
				
				delete ch;
			}
			if (config->summon && summonCount<config->summonLessThan && self->mana>=config->summonMana) {				
				// we should summon something
				char buf[256];
				sprintf(buf,"utevo res %s",config->summonName);
				sender.say(buf);
				Sleep(600);
			}
		}
		
		
		delete self;
		
	}
	toolThreadShouldStop=0;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMod_spellcasterApp construction

CMod_spellcasterApp::CMod_spellcasterApp()
{
	m_configDialog =NULL;
	m_started=0;
	m_configData = new CConfigData();	
}

CMod_spellcasterApp::~CMod_spellcasterApp()
{
	if (m_configDialog)
	{
		delete m_configDialog;
	}
	delete m_configData;	
}

char * CMod_spellcasterApp::getName()
{
	return "Spell Caster";
}


int CMod_spellcasterApp::isStarted()
{
	return m_started;
}


void CMod_spellcasterApp::start()
{	
	if (m_configDialog)
	{
		m_configDialog->disableControls();
		m_configDialog->activateEnableButton(true);
	}
	
	DWORD threadId;
	
	toolThreadShouldStop=0;
	toolThreadHandle =  ::CreateThread(NULL,0,toolThreadProc,m_configData,0,&threadId);				
	m_started=1;
}

void CMod_spellcasterApp::stop()
{
	toolThreadShouldStop=1;
	while (toolThreadShouldStop) {
		Sleep(50);
	};
	m_started=0;
	
	if (m_configDialog)
	{
		m_configDialog->enableControls();
		m_configDialog->activateEnableButton(false);
	}
} 

void CMod_spellcasterApp::showConfigDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	if (!m_configDialog)
	{
		m_configDialog = new CConfigDialog(this);
		m_configDialog->Create(IDD_CONFIG);
		configToControls();
	}
	m_configDialog->ShowWindow(SW_SHOW);
}


void CMod_spellcasterApp::configToControls()
{
	if (m_configDialog)
	{		
		
		m_configDialog->configToControls(m_configData);
	}
}


void CMod_spellcasterApp::controlsToConfig()
{
	if (m_configDialog)
	{
		delete m_configData;
		m_configData = m_configDialog->controlsToConfig();
	}
}


void CMod_spellcasterApp::disableControls()
{
	if (m_configDialog)
	{
		m_configDialog->disableControls();
	}
}

void CMod_spellcasterApp::enableControls()
{
	if (m_configDialog)
	{
		m_configDialog->enableControls();
	}
}


char *CMod_spellcasterApp::getVersion()
{
	return "2.4";
}


int CMod_spellcasterApp::validateConfig(int showAlerts)
{
	if (m_configData->mana){
		if (!strlen(m_configData->manaSpell))
		{
			if (showAlerts) AfxMessageBox("Some mana spell to cast must be defined!");
			return 0;
		}
		if (m_configData->manaMana<0)
		{
			if (showAlerts) AfxMessageBox("'mana above' must be >=0!");
			return 0;
		}
	}
	
	if (m_configData->life){
		int test = m_configData->customSpell+m_configData->vitaSpell+m_configData->granSpell+m_configData->exuraSpell+m_configData->paralysisSpell+m_configData->poisonSpell;
		if (!test) {
			AfxMessageBox("At least one healing spell must be defined!");
			return 0;
		}
		if (m_configData->customSpell) {
			if (!strlen(m_configData->lifeSpell)) {
					if (showAlerts) AfxMessageBox("Some life spell to cast must be defined!");
					return 0;
				}
			if (m_configData->lifeHp<0)	{
				if (showAlerts) AfxMessageBox("'Cast when life below' must be >=0!");
				return 0;
			}
			if (m_configData->lifeSpellMana<1) {
				if (showAlerts) AfxMessageBox("'Spell mana' must be >=1!");
				return 0;
			}			
		}
		if (m_configData->vitaSpell) {
			if (m_configData->vitaHp<0)
			{
				if (showAlerts) AfxMessageBox("'Cast when life below' must be >=0!");
				return 0;
			}
			if (m_configData->vitaSpellMana<160) {
				if (showAlerts) AfxMessageBox("'Spell mana' must be >=160!");
				return 0;
			}
		}
		if (m_configData->granSpell) {
			if (m_configData->granHp<0) {
				if (showAlerts) AfxMessageBox("'Cast when life below' must be >=0!");
				return 0;
			}
			if (m_configData->granSpellMana<70) {
				if (showAlerts) AfxMessageBox("'Spell mana' must be >=70!");
				return 0;
			}
		}
		if (m_configData->exuraSpell) {
			if (m_configData->exuraHp<0) {
				if (showAlerts) AfxMessageBox("'Cast when life below' must be >=20!");
				return 0;
			}
			if (m_configData->exuraSpellMana<20) {
				if (showAlerts) AfxMessageBox("'Spell mana' must be >=1!");
				return 0;
			}
		}
		if (m_configData->poisonSpell) {
			if (m_configData->minPoisonDmg<1) {
				if (showAlerts) AfxMessageBox("'Cure poison when damage greater than' must be >=1!");
				return 0;
			}
		}
	}
	
	
	if (m_configData->summon){
		if (m_configData->summonLessThan<0||m_configData->summonLessThan>2)
		{
			if (showAlerts) AfxMessageBox("'summon less than' must be between 0 and 2!");
			return 0;
		}
		if (!strlen(m_configData->summonName))
		{
			if (showAlerts) AfxMessageBox("Summon's name must be not empty!");
			return 0;
		}
		if (m_configData->summonMana<0)
		{
			if (showAlerts) AfxMessageBox("'summon mana' must be >=0!");
			return 0;
		}
	}
	
	if (m_configData->strike){
		if (m_configData->strikeSpellHpMin<0) {
			if (showAlerts) AfxMessageBox("'Target's HP above' must be >=0!");
			return 0;
		}
		if (m_configData->manaStrike<20) {
			if (showAlerts) AfxMessageBox("'Strike mana above' must be >=20!");
			return 0;
		}
		int test = m_configData->flam+m_configData->frigo+m_configData->mort+m_configData->tera+m_configData->vis;
		if (!test) {
			if (showAlerts) AfxMessageBox("You must choose one strike spell!!");
			return 0;
		}
		
	}
	
	return 1;
}

void CMod_spellcasterApp::resetConfig()
{
	m_configData = new CConfigData();
}

void CMod_spellcasterApp::loadConfigParam(char *paramName,char *paramValue)
{
	if (!strcmp(paramName,"mana")) m_configData->mana=atoi(paramValue);
	if (!strcmp(paramName,"manaMana")) m_configData->manaMana=atoi(paramValue);
	if (!strcmp(paramName,"manaSpell")) lstrcpyn(m_configData->manaSpell,paramValue,128);
	
	if (!strcmp(paramName,"life")) m_configData->life=atoi(paramValue);
	if (!strcmp(paramName,"customSpell")) m_configData->customSpell=atoi(paramValue);
	if (!strcmp(paramName,"lifeHp")) m_configData->lifeHp=atoi(paramValue);
	if (!strcmp(paramName,"lifeSpell")) lstrcpyn(m_configData->lifeSpell,paramValue,128);
	if (!strcmp(paramName,"lifeSpellMana")) m_configData->lifeSpellMana=atoi(paramValue);
	if (!strcmp(paramName,"exuraSpell")) m_configData->exuraSpell=atoi(paramValue);
	if (!strcmp(paramName,"exuraHp")) m_configData->exuraHp=atoi(paramValue);
	if (!strcmp(paramName,"exuraSpellMana")) m_configData->exuraSpellMana=atoi(paramValue);
	if (!strcmp(paramName,"granSpell")) m_configData->granSpell=atoi(paramValue);
	if (!strcmp(paramName,"granHp")) m_configData->granHp=atoi(paramValue);
	if (!strcmp(paramName,"granSpellMana")) m_configData->granSpellMana=atoi(paramValue);
	if (!strcmp(paramName,"vitaSpell")) m_configData->vitaSpell=atoi(paramValue);
	if (!strcmp(paramName,"vitaHp")) m_configData->vitaHp=atoi(paramValue);
	if (!strcmp(paramName,"vitaSpellMana")) m_configData->vitaSpellMana=atoi(paramValue);
	if (!strcmp(paramName,"poisonSpell")) m_configData->poisonSpell=atoi(paramValue);
	if (!strcmp(paramName,"paralysisSpell")) m_configData->paralysisSpell=atoi(paramValue);
	if (!strcmp(paramName,"minPoisonDmg")) m_configData->minPoisonDmg=atoi(paramValue);

	if (!strcmp(paramName,"summon")) m_configData->summon=atoi(paramValue);
	if (!strcmp(paramName,"summonLessThan")) m_configData->summonLessThan=atoi(paramValue);
	if (!strcmp(paramName,"summonMana")) m_configData->summonMana=atoi(paramValue);
	if (!strcmp(paramName,"summonName")) lstrcpyn(m_configData->summonName,paramValue,128);
	
	if (!strcmp(paramName,"strike")) m_configData->strike=atoi(paramValue);
	if (!strcmp(paramName,"ExoriFlam")) m_configData->flam=atoi(paramValue);
	if (!strcmp(paramName,"ExoriFrigo")) m_configData->frigo=atoi(paramValue);
	if (!strcmp(paramName,"ExoriMort")) m_configData->mort=atoi(paramValue);
	if (!strcmp(paramName,"ExoriTera")) m_configData->tera=atoi(paramValue);
	if (!strcmp(paramName,"ExoriVis")) m_configData->vis=atoi(paramValue);
	if (!strcmp(paramName,"manaStrike")) m_configData->manaStrike=atoi(paramValue);
	if (!strcmp(paramName,"defaultStrikeSpell")) lstrcpyn(m_configData->defaultStrikeSpell,paramValue,128);
	if (!strcmp(paramName,"strikeSpellHpMin")) m_configData->strikeSpellHpMin=atoi(paramValue);
}

char *CMod_spellcasterApp::saveConfigParam(char *paramName)
{
	static char buf[1024];
	buf[0]=0;
	
	if (!strcmp(paramName,"mana")) sprintf(buf,"%d",m_configData->mana);
	if (!strcmp(paramName,"manaMana")) sprintf(buf,"%d",m_configData->manaMana);
	if (!strcmp(paramName,"manaSpell")) sprintf(buf,"%s",m_configData->manaSpell);
	
	if (!strcmp(paramName,"life")) sprintf(buf,"%d",m_configData->life);
	if (!strcmp(paramName,"customSpell")) sprintf(buf,"%d",m_configData->customSpell);
	if (!strcmp(paramName,"lifeHp")) sprintf(buf,"%d",m_configData->lifeHp);
	if (!strcmp(paramName,"lifeSpell")) sprintf(buf,"%s",m_configData->lifeSpell);
	if (!strcmp(paramName,"lifeSpellMana")) sprintf(buf,"%d",m_configData->lifeSpellMana);
	if (!strcmp(paramName,"exuraSpell")) sprintf(buf,"%d",m_configData->exuraSpell);
	if (!strcmp(paramName,"exuraHp")) sprintf(buf,"%d",m_configData->exuraHp);
	if (!strcmp(paramName,"exuraSpellMana")) sprintf(buf,"%d",m_configData->exuraSpellMana);
	if (!strcmp(paramName,"granSpell")) sprintf(buf,"%d",m_configData->granSpell);
	if (!strcmp(paramName,"granHp")) sprintf(buf,"%d",m_configData->granHp);
	if (!strcmp(paramName,"granSpellMana")) sprintf(buf,"%d",m_configData->granSpellMana);
	if (!strcmp(paramName,"vitaSpell")) sprintf(buf,"%d",m_configData->vitaSpell);
	if (!strcmp(paramName,"vitaHp")) sprintf(buf,"%d",m_configData->vitaHp);
	if (!strcmp(paramName,"vitaSpellMana")) sprintf(buf,"%d",m_configData->vitaSpellMana);
	if (!strcmp(paramName,"poisonSpell")) sprintf(buf,"%d",m_configData->poisonSpell);
	if (!strcmp(paramName,"paralysisSpell")) sprintf(buf,"%d",m_configData->paralysisSpell);
	if (!strcmp(paramName,"minPoisonDmg")) sprintf(buf,"%d",m_configData->minPoisonDmg);
	
	if (!strcmp(paramName,"summon")) sprintf(buf,"%d",m_configData->summon);
	if (!strcmp(paramName,"summonLessThan")) sprintf(buf,"%d",m_configData->summonLessThan);
	if (!strcmp(paramName,"summonMana")) sprintf(buf,"%d",m_configData->summonMana);
	if (!strcmp(paramName,"summonName")) strcpy(buf,m_configData->summonName);
	
	if (!strcmp(paramName,"strike")) sprintf(buf,"%d",m_configData->strike);
	if (!strcmp(paramName,"ExoriFlam")) sprintf(buf,"%d",m_configData->flam);
	if (!strcmp(paramName,"ExoriFrigo")) sprintf(buf,"%d",m_configData->frigo);
	if (!strcmp(paramName,"ExoriMort")) sprintf(buf,"%d",m_configData->mort);
	if (!strcmp(paramName,"ExoriTera")) sprintf(buf,"%d",m_configData->tera);
	if (!strcmp(paramName,"ExoriVis")) sprintf(buf,"%d",m_configData->vis);
	if (!strcmp(paramName,"manaStrike")) sprintf(buf,"%d",m_configData->manaStrike);
	if (!strcmp(paramName,"defaultStrikeSpell")) sprintf(buf,"%s",m_configData->defaultStrikeSpell);
	if (!strcmp(paramName,"strikeSpellHpMin")) sprintf(buf,"%d",m_configData->strikeSpellHpMin);
	
	return buf;
}

char *CMod_spellcasterApp::getConfigParamName(int nr)
{
	switch (nr)
	{
	case 0: return "mana";
	case 1: return "manaMana";
	case 2: return "manaSpell";
	case 3: return "life";
	case 4: return "lifeHp";
	case 5: return "lifeSpell";
	case 6: return "lifeSpellMana";
	case 7: return "summon";
	case 8: return "summonLessThan";
	case 9: return "summonName";
	case 10: return "strike";
	case 11: return "ExoriFlam";
	case 12: return "ExoriFrigo";
	case 13: return "ExoriMort";
	case 14: return "ExoriTera";
	case 15: return "ExoriVis";
	case 16: return "manaStrike";
	case 17: return "customSpell";
	case 18: return "exuraSpell";
	case 19: return "exuraHp";
	case 20: return "exuraSpellMana";
	case 21: return "granSpell";
	case 22: return "granHp";
	case 23: return "granSpellMana";
	case 24: return "vitaSpell";
	case 25: return "vitaHp";
	case 26: return "vitaSpellMana";
	case 27: return "defaultStrikeSpell";
	case 28: return "summonMana";
	case 29: return "strikeSpellHpMin";
	case 30: return "poisonSpell";
	case 31: return "paralysisSpell";
	case 32: return "minPoisonDmg";
		
	default:
		return NULL;
	}
}

int initalizeCreatures() {
	ifstream creatureFile ("creatures.csv", ios::in);
	if (!creatureFile.is_open()) {	AfxMessageBox("File creatures.csv Not found!"); creatureFile.close(); return 0;}
	char buf[128] = {0};
	int crNum = 0;

	while (!creatureFile.eof()) {
		creatureFile.getline(monstersInfo[crNum].name, 128, ',');
		//AfxMessageBox(monstersInfo[crNum].name);
		creatureFile.getline(buf, 128, ',');
		monstersInfo[crNum].weakEarth = strcmpi(buf, "Earth")==0?1:0;
		//AfxMessageBox(buf);
		creatureFile.getline(buf, 128, ',');
		monstersInfo[crNum].weakFire = strcmpi(buf, "Fire")==0?1:0;
		//AfxMessageBox(buf);
		creatureFile.getline(buf, 128, ',');
		monstersInfo[crNum].weakEnergy = strcmpi(buf, "Energy")==0?1:0;
		//AfxMessageBox(buf);
		creatureFile.getline(buf, 128, ',');
		monstersInfo[crNum].weakIce = strcmpi(buf, "Ice")==0?1:0;
		//AfxMessageBox(buf);
		creatureFile.getline(buf, 128, ',');
		monstersInfo[crNum].weakDeath = strcmpi(buf, "Death")==0?1:0;
		//AfxMessageBox(buf);
		creatureFile.getline(buf, 128, '\n');
		monstersInfo[crNum++].hp = atoi(buf);
		//AfxMessageBox(monstersInfo[max].name);
	}
	creatureFile.close();

	return 1;
}

int isInitializedCreatures() {
	if (strcmpi(monstersInfo[0].name, "Initialized") == 0) return 1;
	return 0;
}

int getMonsterNumberFromName(char *match) {
	int foundNone = -1, max = 0;
	while (true) {
		if (strcmpi(monstersInfo[max].name, match)==0) {			
			return max;
		}
		if (max++ == 250) return foundNone;;		
	}
	
}




