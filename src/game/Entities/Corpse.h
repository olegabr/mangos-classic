/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOSSERVER_CORPSE_H
#define MANGOSSERVER_CORPSE_H

#include "Common.h"
#include "Entities/Object.h"
#include "Database/DatabaseEnv.h"
#include "Maps/GridDefines.h"

enum CorpseType
{
    CORPSE_BONES             = 0,
    CORPSE_RESURRECTABLE_PVE = 1,
    CORPSE_RESURRECTABLE_PVP = 2
};
#define MAX_CORPSE_TYPE        3

// Value equal client resurrection dialog show radius.
#define CORPSE_RECLAIM_RADIUS 39

enum CorpseFlags
{
    CORPSE_FLAG_NONE        = 0x00,
    CORPSE_FLAG_BONES       = 0x01,
    CORPSE_FLAG_UNK1        = 0x02,
    CORPSE_FLAG_UNK2        = 0x04,
    CORPSE_FLAG_HIDE_HELM   = 0x08,
    CORPSE_FLAG_HIDE_CLOAK  = 0x10,
    CORPSE_FLAG_LOOTABLE    = 0x20
};

class Corpse : public WorldObject
{
    public:
        explicit Corpse(CorpseType type = CORPSE_BONES);
        ~Corpse();

        void AddToWorld() override;
        void RemoveFromWorld() override;

        bool Create(uint32 guidlow);
        bool Create(uint32 guidlow, Player* owner);

        void SaveToDB();
        bool LoadFromDB(uint32 lowguid, Field* fields);

        void DeleteBonesFromWorld();
        void DeleteFromDB() const;

        ObjectGuid const& GetOwnerGuid() const override { return GetGuidValue(CORPSE_FIELD_OWNER); }
        void SetOwnerGuid(ObjectGuid guid) override { SetGuidValue(CORPSE_FIELD_OWNER, guid); }

        uint8 getRace() const { return GetByteValue(CORPSE_FIELD_BYTES_1, 1); }
        uint32 getRaceMask() const { return 1 << (getRace() - 1); }
        uint8 getGender() const { return GetByteValue(CORPSE_FIELD_BYTES_1, 2); }

        // faction template id
        uint32 GetFaction() const;

        time_t const& GetGhostTime() const { return m_time; }
        void ResetGhostTime() { m_time = time(nullptr); }
        CorpseType GetType() const { return m_type; }

        GridPair const& GetGrid() const { return m_grid; }
        void SetGrid(GridPair const& grid) { m_grid = grid; }

        bool isVisibleForInState(Player const* u, WorldObject const* viewPoint, bool inVisibleList) const override;

        Player* lootRecipient;
        bool lootForBody;

        GridReference<Corpse>& GetGridRef() { return m_gridRef; }

        bool IsExpired(time_t t) const;
        Team GetTeam() const;

        uint8 GetRankSnapshot() const { return m_rankSnapshot; }
    private:
        GridReference<Corpse> m_gridRef;

        CorpseType m_type;
        time_t m_time;
        GridPair m_grid;                                    // gride for corpse position for fast search
        uint8 m_rankSnapshot;
};
#endif
