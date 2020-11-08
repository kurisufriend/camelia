#include "backtrack.h"
#include "sdk.h"
#include "globals.h"

static std::vector<backtrackRecord> records[64];
/*
bool isValid(float simtime)
{
    static ConVar* svMaxUnlag = interfaces::pacCvar->FindVar("sv_maxunlag");
    const auto network = interfaces::pacEngineClient->GetNetChannelInfo();
    if (!network)
        return false;
    auto delta = std::clamp(network->GetLatency(0) + network->GetLatency(1), 0.f, svMaxUnlag->getFloat()) - (utils::getServerTime() - simtime);
    return std::abs(delta) <= 0.2f;
}

int timeToTicks(float time)
{
	return (int)(.5f + time / interfaces::pacGlobals->intervalPerTick);
}
*/
void modules::backtrack(CUserCmd* cmd)
{/*
    if (!g::pentLocalPlayer)
        return;
    for (int i; i < 64; i++)
    {
        CBasePlayer* curEntity = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(i);
        if (!curEntity || curEntity->getDormant() || (curEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum()))
            continue;

        std::vector<backtrackRecord> curRecords = records[i];

        backtrackRecord newRecord;
        newRecord.tickcount = cmd->tickcount;
        newRecord.simtime = curEntity->getSimulationTime();
        newRecord.headPos = curEntity->getBonePosition(8);

        curRecords.insert(records[i].begin(), newRecord);

        if (curRecords.size() > 12)
            curRecords.pop_back();

        records[i] = curRecords;
    }*/
}