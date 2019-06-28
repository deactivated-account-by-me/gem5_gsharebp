/*
  LRUIPVRP
  **/

#include "mem/cache/replacement_policies/lru_ipv.hh"

#include <cassert>
#include <memory>

#include "params/LRURP.hh"


int ipv[] = {0,0,1,0,3,0,1,2,1,0,5,1,0,0,1,11,13};

LRUIPVRP::LRUIPVRP(const Params *p)
    : BaseReplacementPolicy(p)
{
}

void
LRUIPVRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    std::shared_pointer_cast<LRUIPVReplData>(replacement_data)->index = 1000;
}

void    
LRUIPVRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    //THIS FUNCTION IS FOR A HIT AND HOW THE MODEL ADJUSTS AFTERWARDS
    int replacement_data_index = std::shared_pointer_cast<LRUIPVReplData>(replacement_data)->index;

    for (int i = ipv[replacement_data_index];i < replacement_data_index; i++)
    {
        recency_stack[i]++;
    }

    std::shared_pointer_cast<LRUIPVReplData>(replacement_data)->index = ipv[replacement_data_index];
}

void
LRUIPVRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // THIS FUNCTION IS FOR EVERY TIME YOU HAVE A NEW INSERTION AND HOW THE SYSTEM WILL ADJUST TO IT
    int replacement_data_index = std::shared_pointer_cast<LRUIPVReplData>(replacement_data)->index;
    for (int i =13; i < 16; i++)
    {
        recency_stack[i]++;
    }

    std::shared_pointer_cast<LRUIPVReplData>(replacement_data)->index = 13;
    recency_stack[13] = replacement_data;
}

ReplaceableEntry*
LRUIPVRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);
    int i = 1
    for (const auto& candidate : candidates)
    {
        //candidates[i]->index = recency_stack[i];
        //recency_stack[i] = candidates[i]->index;
        std::shared_pointer_cast<LRUIPVReplData>(candidate->replacementData)->index = recency_stack[i];

        i = i + 1; 
    }
    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        // Update victim entry if necessary
        if (std::static_pointer_cast<LRUIPVReplData>(candidate->replacementData)->lastTouchTick < std::static_pointer_cast<LRUIPVReplData>(victim->replacementData)->lastTouchTick) {
            victim = candidate;
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
LRUIPVRP::instantiateEntry()
{
    std::vector<int> recency_stack = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600};
    return std::shared_ptr<ReplacementData>(new LRUIPVRPParams());
}

LRUIPVRP*
LRUIPVRPParams::create()
{
    return new LRUIPVRP(this);
}
