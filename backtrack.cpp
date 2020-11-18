#include "backtrack.h"
#include "sdk.h"
#include "globals.h"
#include "settings.h"

static std::vector<backtrackRecord> records[64];

void modules::backtrack(CUserCmd* cmd)
{
    if (!g::pentLocalPlayer)
        return;
    if (!settings::bBacktrack)
        return;
    for (int i = 0; i < 64; i++)
    {
        CBasePlayer* curEntity = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(i);
        if (!curEntity || curEntity->getDormant() || (curEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum()))
            continue;

        std::vector<backtrackRecord> curRecords = records[i];

        backtrackRecord newRecord;
        newRecord.tickcount = cmd->tickcount;
        newRecord.headPos = curEntity->getBonePosition(8);

        curRecords.insert(curRecords.begin(), newRecord);

        if (curRecords.size() > 12)
            curRecords.pop_back();

        records[i] = curRecords;
    }
    if (cmd->buttons & IN_ATTACK)
    {
        float bestDistance = FLT_MAX;
        backtrackRecord bestRecord;
        for (int i = 0; i < 64; i++)
        {
            std::vector<backtrackRecord> curRecords = records[i];
            for (auto record : curRecords)
            {
                Vector idealAngle = utils::calcAngle(g::pentLocalPlayer->getEyePosition(), record.headPos);
                Vector curAngle;
                interfaces::pacEngineClient->GetViewAngles(curAngle);
                float curDistance = curAngle.DistTo(idealAngle);
                if (curDistance < bestDistance)
                {
                    bestDistance = curDistance;
                    bestRecord = record;
                }
            }
        }
        cmd->tickcount = bestRecord.tickcount;
    }
}