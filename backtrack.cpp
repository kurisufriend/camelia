#include "backtrack.h"
#include "sdk.h"
#include "globals.h"
#include "settings.h"

//polak
bool isRecordOutOfBounds(backtrackRecord record)
{
    float correct = 0;

    correct += ((INetChannelInfo*)(interfaces::pacEngineClient->GetNetChannelInfo()))->GetLatency(0);// cool it with the anti semetic remarks
    correct += ((INetChannelInfo*)(interfaces::pacEngineClient->GetNetChannelInfo()))->GetLatency(1);

    correct = std::clamp(correct, 0.f, .2f);

    float deltaTime = correct - (interfaces::pacGlobals->currenttime - TICKS_TO_TIME(record.tickcount));

    if (fabsf(deltaTime) > 0.2f)
    {
        return true;
    }

    return false;
}

void modules::backtrack(CUserCmd* cmd)
{
    if (!g::pentLocalPlayer)
        return;
    if (!settings::bBacktrack)
        return;
    for (int i = 0; i < 64; i++)
    {
        CBasePlayer* curEntity = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(i);
        if (!curEntity || curEntity->getDormant() || curEntity->getHealth() <= 0 || (curEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum()))
            continue;

        //add new records to deque
        backtrackRecord curRecord;
        curRecord.tickcount = cmd->tickcount;
        curRecord.headPos = curEntity->getBonePosition(8);
        curRecord.chestPos = curEntity->getBonePosition(6);
        curRecord.player = curEntity;
        g::records[i].push_front(curRecord);

        //clamp at x number of ticks
        while (g::records[i].size() > settings::iBacktrackTicks)
            g::records[i].pop_back();

        //cleanse the deque of invalid ticks
        for (int poo = 0; poo < g::records[i].size(); poo++)
        {
            if (isRecordOutOfBounds(g::records[i].at(poo)) || !(g::records[i].at(poo).player) || (g::records[i].at(poo).player->getHealth() <= 0))
            {
                std::deque<backtrackRecord>::iterator iterator = g::records[i].begin();
                std::advance(iterator, poo);
                g::records[i].erase(iterator);
            }
        }
    }
    //find the closest record to the crosshair
    float bestDistance = FLT_MAX;
    backtrackRecord bestRecord;
    for (int i = 0; i < 64; i++)
    {
        std::deque<backtrackRecord> curRecords = g::records[i];
        for (backtrackRecord record : curRecords)
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
    if (bestRecord.tickcount && (cmd->buttons & IN_ATTACK))
    {
        //std::cout << "BTing from " << cmd->tickcount << " to " << bestRecord.tickcount << std::endl;
        cmd->tickcount = bestRecord.tickcount;
    }
}