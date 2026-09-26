#include "mod.h"
#include "patch.h"
#include "evtpatch.h"

#include <spm/camdrv.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <spm/itemdrv.h>
#include <spm/evtmgr.h>
#include <spm/hitdrv.h>
#include <spm/evt_npc.h>
#include <spm/map_data.h>
#include <spm/evt_mario.h>
#include <wii/os/OSError.h>
#include <wii/mtx.h>
#include <wii/gx.h>

namespace mod {
/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
spm::hitdrv::HitObj * ( * func_8007bee4)(double param_1, double param_2, double param_3, spm::itemdrv::ItemEntry *itemEntry, float *param_5, float *param_6, float *param_7, float *param_8);

extern "C"
{
    f32 _speedBuff = 0.2f;
    f32 _speedBuff2 = 0.15f;
    f32 _speedBuff3 = 2580.0f;
    f32 _speedBuff4 = 8.8f;
    f32 daisySpeed = 0.5f;
    f32 daisyTimeout = 0.01f;
    f32 wheelBuff = 15.0f;
    void frackPatch();
    asm
    (
      ".global frackPatch\n"
      "frackPatch:\n"
      "lis 5, _speedBuff2@ha\n"
      "lfs 2, _speedBuff2@l(5)\n"
      "stfs 2, 0x444(31)\n"
      "blr\n"
    );
    void dimentioPatch();
    asm
    (
      ".global dimentioPatch\n"
      "dimentioPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 2, _speedBuff@l(5)\n"
      "stfs 2, 0x444(30)\n"
      "blr\n"
    );
    void superDimentioPatch();
    asm
    (
      ".global superDimentioPatch\n"
      "superDimentioPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 1, _speedBuff@l(5)\n"
      "stfs 1, 0x444(30)\n"
      "blr\n"
    );
    void bonechillPatch();
    asm
    (
      ".global bonechillPatch\n"
      "bonechillPatch:\n"
      "lwz 28, 0x168(3)\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 2, _speedBuff@l(5)\n"
      "stfs 2, 0x444(28)\n"
      "blr\n"
    );
    void shellPatch();
    asm
    (
      ".global shellPatch\n"
      "shellPatch:\n"
      "lis 5, _speedBuff3@ha\n"
      "lfs 0, _speedBuff3@l(5)\n"
      "blr\n"
    );
    void mrLpatch();
    asm
    (
      ".global mrLpatch\n"
      "mrLpatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 0, _speedBuff@l(5)\n"
      "stfs 0, 0x444(29)\n"
      "blr\n"
    );
    void peachPatch();
    asm
    (
      ".global peachPatch\n"
      "peachPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 1, _speedBuff@l(5)\n"
      "stfs 1, 0x444(29)\n"
      "blr\n"
    );
    void marioPatch();
    asm
    (
      ".global marioPatch\n"
      "marioPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 1, _speedBuff@l(5)\n"
      "stfs 1, 0x444(30)\n"
      "blr\n"
    );
    void boomPatch();
    asm
    (
      ".global boomPatch\n"
      "boomPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 5, _speedBuff@l(5)\n"
      "stfs 5, 0x444(31)\n"
      "blr\n"
    );
    void daisyPatch();
    asm
    (
      ".global daisyPatch\n"
      "daisyPatch:\n"
      "lis 5, daisySpeed@ha\n"
      "lfs 1, daisySpeed@l(5)\n"
      "stfs 1, 0x45c(30)\n"
      "blr\n"
    );
    void daisyTimePatch();
    asm
    (
      ".global daisyTimePatch\n"
      "daisyTimePatch:\n"
      "lis 5, daisyTimeout@ha\n"
      "lfs 0, daisyTimeout@l(5)\n"
      "blr\n"
    );
    void fartPatch();
    asm
    (
      ".global fartPatch\n"
      "fartPatch:\n"
      "lis 5, _speedBuff3@ha\n"
      "lfs 5, _speedBuff3@l(5)\n"
      "stfs 1, 0x444(31)\n"
      "blr\n"
    );
    void longatorPatch();
    asm
    (
      ".global longatorPatch\n"
      "longatorPatch:\n"
      "lis 5, _speedBuff@ha\n"
      "lfs 0, _speedBuff@l(5)\n"
      "stfs 0, 0x444(30)\n"
      "blr\n"
    );
    void _wheelPatch();
    asm
    (
      ".global _wheelPatch\n"
      "_wheelPatch:\n"
      "lis 5, wheelBuff@ha\n"
      "lfs 5, wheelBuff@l(5)\n"
      "fmul 1, 1, 5\n"
      "fsubs 1, 1, 2\n"
      "blr\n"
    );
}

spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  spm::evtmgr::EvtScriptCode* arg = evtpatch::getInstructionArgv(link)[instruction];
  return arg;
}

static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor green = {0, 255, 0, 255};
    f32 scale = 0.8f;
    const char * msg = "Super Paper Mario but they're on SPEEEEEED";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}
  f32 new_evt_npc_walk_to(spm::evtmgr::EvtEntry *evtEntry, spm::evtmgr::EvtVar data)
  {

    f32 speed = spm::evtmgr_cmd::evtGetFloat(evtEntry, data);

    if (speed == 0)
    {
      speed = 50.5f;
    }

    wii::os::OSReport("float %f\n", speed * 10.0f);

    return speed * 10.0f;
  }

  s32 new_evt_npc_jump_to(spm::evtmgr::EvtEntry *evtEntry, spm::evtmgr::EvtVar data)
  {

    s32 speed = spm::evtmgr_cmd::evtGetValue(evtEntry, data);

    //wii::os::OSReport("float %f\n", speed * 10.0f);

    return speed * 10;
  }

  spm::hitdrv::HitObj * new_func_8007bee4(double param_1, double param_2, double param_3, spm::itemdrv::ItemEntry *itemEntry, float *param_5, float *param_6, float *param_7, float *param_8)
  {
    param_1 *= 10.0;
    return func_8007bee4( param_1,  param_2,  param_3, itemEntry, param_5,  param_6, param_7, param_8);
  }

EVT_BEGIN(hammerBroPatch1)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
RETURN_FROM_CALL()

EVT_BEGIN(hammerBroPatch2)
  SET(LW(0), 1)
RETURN_FROM_CALL()

EVT_BEGIN(rubeePatch)
  MUL(GW(2), 10)
RETURN_FROM_CALL()

EVT_BEGIN(wheelPatch)
  MUL(LW(0), 10)
RETURN_FROM_CALL()

static void hookEvents()
{
  writeBranchLink(spm::evt_npc::evt_npc_walk_to, 0xAC, new_evt_npc_walk_to);
  writeBranchLink(spm::evt_npc::evt_npc_arc_to, 0xE0, new_evt_npc_walk_to);
  writeBranchLink(spm::evt_npc::evt_npc_glide_to, 0xD8, new_evt_npc_walk_to);
  writeBranchLink(spm::evt_npc::evt_npc_jump_to, 0xA8, new_evt_npc_jump_to);
  //writeBranchLink(0x801e8260, 0x0, dimentioPatch);
  writeBranchLink(0x801e8260, 0x0, dimentioPatch);
  writeBranchLink(0x801dae54, 0x0, frackPatch);
  writeBranchLink(0x801f034c, 0x0, superDimentioPatch);
  writeBranchLink(0x802312b4, 0x0, bonechillPatch);
  writeBranchLink(0x80207cec, 0x0, mrLpatch);
  writeBranchLink(0x802248a8, 0x0, peachPatch);
  writeBranchLink(0x8021557c, 0x0, marioPatch);
  writeBranchLink(0x801e33c8, 0x0, shellPatch);
  writeBranchLink(0x8021f8b8, 0x0, dimentioPatch);
  writeBranchLink(0x801fa034, 0x0, boomPatch);
  writeBranchLink(0x80223640, 0x0, daisyPatch);
  writeBranchLink(0x802236ac, 0x0, daisyTimePatch);
  writeBranchLink(0x8024b6bc, 0x0, fartPatch);
  writeBranchLink(0x8010b0c4, 0x0, longatorPatch);
  writeBranchLink(0x80c50938, 0x0, _wheelPatch);
  writeWord(0x801608e0, 0x0, NOP);
  func_8007bee4 = patch::hookFunction(spm::itemdrv::func_8007bee4, new_func_8007bee4);
  evtpatch::hookEvtReplace(spm::npcdrv::npcEnemyTemplates[199].moveScript, 8, hammerBroPatch1);
  evtpatch::hookEvtReplace(spm::npcdrv::npcEnemyTemplates[199].moveScript, 2, hammerBroPatch2);

  spm::map_data::MapData * mi3_03_md = spm::map_data::mapDataPtr("mi3_03");
  spm::evtmgr::EvtScriptCode* talkScript = getInstructionEvtArg(mi3_03_md->initScript, 14, 3);
  spm::evtmgr::EvtScriptCode* wheelScript = getInstructionEvtArg(mi3_03_md->initScript, 30, 0);
  //wii::os::OSReport("script is dum dum %p\n", talkScript);
  evtpatch::hookEvt(wheelScript, 88, wheelPatch);
  evtpatch::hookEvt(wheelScript, 72, wheelPatch);
  evtpatch::hookEvt(wheelScript, 15, wheelPatch);
  evtpatch::hookEvt(talkScript, 62, wheelPatch);
  evtpatch::hookEvt(talkScript, 5, rubeePatch);

}

/*
    General mod functions
*/
void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
    evtpatch::evtmgrExtensionInit();
    hookEvents();
}

}
