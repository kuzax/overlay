#pragma once

#include <string>
#include "includes.h"
#include <set>
#include <unordered_map>
 
#include <unordered_set>

class MTAScript
{
public:
    MTAScript(const std::string& name, const std::string& code) : name(name), code(code) {}

    std::string name;
    std::string code;
};


class ScriptsManager
{
public:
    ScriptsManager() {} 
    std::vector<MTAScript*> scripts;
    std::set<std::string> blockedScripts;
    void ClearScripts();
    void BlockScript(const std::string& scriptName);
    bool IsScriptBlocked(const std::string& scriptName) const;
    void UnblockScript(const std::string& scriptName);
    void OnSaveScriptsButtonClick();

    void StoreAnyScript(const std::string& name, const std::string& code);
    void AddLuaCodeToStoredScripts();
    std::string storedScriptName;
    std::string storedScriptCode;
    std::string lastLoadedScriptName;
    bool scriptStored = false;


    void AddScript(const std::string& name, const std::string& code)
    {
        MTAScript* script = new MTAScript(name, code);
        scripts.push_back(script);
    }

 
};




 
bool BlockPacket(int packetID);
bool UnblockPacket(unsigned char packetID);
extern std::unordered_set<unsigned char> blockedPackets;

void ClearStoredScripts();
void OpenGTADirectory();
extern ScriptsManager* scriptsManager;

void DrawPacketTable();