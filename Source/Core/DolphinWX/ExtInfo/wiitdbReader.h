#pragma once
#include "../ISOFile.h"
#include <set>
#include "../Core/Common/FileUtil.h"

struct ExtInfo {
	std::string id;
	std::string title;
	std::string genre;
	std::string description;
	int onlinePlayers=-1;
	int players=-1;
	std::set<std::string> requiredControllers;
	std::set<std::string> optionalControllers;
};

typedef std::unordered_map<std::string, ExtInfo> ExtInfoList;

class WiitdbReader
{
public:

	static std::string getExtInfoFilename() {
		return File::GetUserPath(D_LOAD_IDX) + "wiitdb.xml";
	}

	static bool ExtInfoFileExists()
	{
		return wxFileExists(getExtInfoFilename());
	}

	bool LoadExtendedInfos();
	void FillExtendedInfo(GameListItem* pGameListItem);
	void FillExtendedInfos(std::vector<GameListItem*> gameList);

private:
	ExtInfoList m_extInfoList;

};



