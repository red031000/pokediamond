#include "global.h"
#include "filesystem.h"
#include "itemtool.h"
#include "msgdata.h"

THUMB_FUNC NARC * OpenNutsDataNarc(u32 heap_id)
{
    return NARC_ctor(NARC_ITEMTOOL_ITEMDATA_NUTS_DATA, heap_id);
}

THUMB_FUNC struct NutData * ReadNutDataFromNarc(NARC * narc, u32 berry_idx, u32 heap_id)
{
    return NARC_AllocAndReadWholeMember(narc, berry_idx, heap_id);
}

THUMB_FUNC void CloseNutsDataNarc(NARC * narc)
{
    NARC_dtor(narc);
}

THUMB_FUNC struct NutData * LoadNutDataSingle(s32 berry_idx, u32 heap_id)
{
    return AllocAndReadWholeNarcMemberByIdPair(NARC_ITEMTOOL_ITEMDATA_NUTS_DATA, berry_idx, heap_id);
}

THUMB_FUNC struct NutData * LoadNutDataSingleByItemId(s32 item_id, u32 heap_id)
{
    return LoadNutDataSingle(item_id - FIRST_BERRY_IDX, heap_id);
}

THUMB_FUNC u32 GetNutAttr(struct NutData * nut, u32 attr)
{
    switch (attr)
    {
    case 0:
        return nut->unk0;
    case 1:
        return nut->unk2;
    case 2:
        return nut->unk3;
    case 3:
        return nut->unk4;
    case 4:
        return nut->unk5;
    case 5:
        return nut->unk6;
    case 6:
        return nut->unk7;
    case 7:
        return nut->unk8;
    case 8:
        return nut->unk9;
    case 9:
        return nut->unkA;
    case 10:
        return nut->unkB;
    default:
        return 0;
    }
}

THUMB_FUNC struct String * GetNutName(u32 berry_idx, u32 heap_id)
{
    struct MsgData * msgData = NewMsgDataFromNarc(1, NARC_MSGDATA_MSG, 373, heap_id);
    struct String * ret = NewString_ReadMsgData(msgData, berry_idx);
    DestroyMsgData(msgData);
    return ret;
}

THUMB_FUNC struct String * GetNutDesc(u32 berry_idx, u32 heap_id)
{
    struct MsgData * msgData = NewMsgDataFromNarc(1, NARC_MSGDATA_MSG, 372, heap_id);
    struct String * ret = NewString_ReadMsgData(msgData, berry_idx);
    DestroyMsgData(msgData);
    return ret;
}
