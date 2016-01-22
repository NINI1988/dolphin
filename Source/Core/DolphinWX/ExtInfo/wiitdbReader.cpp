#include "WiitdbReader.h"

#include <iostream>
#include <string>

#include "TinyXml\tinyxml.h"
using namespace std;

//#include "ExtInfo.h"
#include <unordered_map>

void readGameControllers(TiXmlElement* xml, ExtInfo& item) {
	for (xml; xml; xml = xml->NextSiblingElement())
	{
		bool required=true;
		xml->QueryBoolAttribute("required", &required);
		if(required) {
			item.requiredControllers.insert(xml->Attribute("type"));
		}
		else {
			item.optionalControllers.insert(xml->Attribute("type"));
		}
	}

}

void readGameElement(TiXmlElement* xml, ExtInfoList& extInfos) {
	ExtInfo item;

	TiXmlElement* pElem;

	pElem = xml->FirstChildElement("id");
	if (pElem) item.id = pElem->FirstChild()->Value();

	pElem = xml->FirstChildElement("genre");
	if (pElem) item.genre = pElem->FirstChild()->Value();

	pElem = xml->FirstChildElement("wi-fi");
	if (pElem) pElem->QueryIntAttribute("players", &item.onlinePlayers);

	pElem = xml->FirstChildElement("input");
	if (pElem) {
		pElem->QueryIntAttribute("players", &item.players);
		readGameControllers(pElem->FirstChildElement(), item);
	}

	extInfos[item.id]=item;
}

bool WiitdbReader::LoadExtendedInfos()
{
	std::string filename = WiitdbReader::getExtInfoFilename();
	const char* pFilename = filename.c_str();
	TiXmlDocument doc;
	bool loadOkay = doc.LoadFile(pFilename);
	if (loadOkay)
	{

		
		//ExtInfoList extInfos;
		printf("\n%s:\n", pFilename);

		TiXmlElement* pElem;

		{//root
			pElem = doc.FirstChildElement("datafile");
			// should always have a valid root but handle gracefully if it does
			if (!pElem) return false;

		}

		// block: games
		{
			pElem = pElem->FirstChildElement("game");
			for (pElem; pElem; pElem = pElem->NextSiblingElement("game"))
			{
				readGameElement(pElem, m_extInfoList);
			}
		}
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
	return loadOkay;
}

void WiitdbReader::FillExtendedInfo(GameListItem* pGameListItem) {

	string id = pGameListItem->GetUniqueID();
	auto elem = m_extInfoList.find(id);
	if (elem != m_extInfoList.end()) {
		ExtInfo& extInfo = elem->second;

		pGameListItem->SetGenre(extInfo.genre);
		pGameListItem->SetOnlinePlayers(extInfo.onlinePlayers);
		pGameListItem->SetOptionalControls(extInfo.optionalControllers);
		pGameListItem->SetPlayers(extInfo.players);
		pGameListItem->SetRequiredControls(extInfo.requiredControllers);

	}

}

void WiitdbReader::FillExtendedInfos( std::vector<GameListItem*> gameList) {
	
	for (auto &pGameListItem : gameList) // access by reference to avoid copying
	{
		FillExtendedInfo(pGameListItem);
	}

}