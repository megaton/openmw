
#include "potion.hpp"

#include <components/esm/loadalch.hpp>

#include "../mwbase/environment.hpp"
#include "../mwbase/world.hpp"

#include "../mwworld/ptr.hpp"
#include "../mwworld/actiontake.hpp"
#include "../mwworld/cellstore.hpp"
#include "../mwworld/physicssystem.hpp"

#include "../mwgui/window_manager.hpp"
#include "../mwgui/tooltips.hpp"

#include "../mwrender/objects.hpp"
#include "../mwrender/renderinginterface.hpp"

#include "../mwsound/soundmanager.hpp"

namespace MWClass
{
    void Potion::insertObjectRendering (const MWWorld::Ptr& ptr, MWRender::RenderingInterface& renderingInterface) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        assert (ref->base != NULL);
        const std::string &model = ref->base->model;

        if (!model.empty())
        {
            MWRender::Objects& objects = renderingInterface.getObjects();
            objects.insertBegin(ptr, ptr.getRefData().isEnabled(), false);
            objects.insertMesh(ptr, "meshes\\" + model);
        }
    }

    void Potion::insertObject(const MWWorld::Ptr& ptr, MWWorld::PhysicsSystem& physics) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();


        const std::string &model = ref->base->model;
        assert (ref->base != NULL);
        if(!model.empty()){
            physics.insertObjectPhysics(ptr, "meshes\\" + model);
        }

    }

    std::string Potion::getName (const MWWorld::Ptr& ptr) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        return ref->base->name;
    }

    boost::shared_ptr<MWWorld::Action> Potion::activate (const MWWorld::Ptr& ptr,
        const MWWorld::Ptr& actor) const
    {
        MWBase::Environment::get().getSoundManager()->playSound3D (ptr, getUpSoundId(ptr), 1.0, 1.0, MWSound::Play_NoTrack);

        return boost::shared_ptr<MWWorld::Action> (
            new MWWorld::ActionTake (ptr));
    }

    std::string Potion::getScript (const MWWorld::Ptr& ptr) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        return ref->base->script;
    }

    int Potion::getValue (const MWWorld::Ptr& ptr) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        return ref->base->data.value;
    }

    void Potion::registerSelf()
    {
        boost::shared_ptr<Class> instance (new Potion);

        registerClass (typeid (ESM::Potion).name(), instance);
    }

    std::string Potion::getUpSoundId (const MWWorld::Ptr& ptr) const
    {
        return std::string("Item Potion Up");
    }

    std::string Potion::getDownSoundId (const MWWorld::Ptr& ptr) const
    {
        return std::string("Item Potion Down");
    }

    std::string Potion::getInventoryIcon (const MWWorld::Ptr& ptr) const
    {
          MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        return ref->base->icon;
    }

    bool Potion::hasToolTip (const MWWorld::Ptr& ptr) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        return (ref->base->name != "");
    }

    MWGui::ToolTipInfo Potion::getToolTipInfo (const MWWorld::Ptr& ptr) const
    {
        MWWorld::LiveCellRef<ESM::Potion> *ref =
            ptr.get<ESM::Potion>();

        MWGui::ToolTipInfo info;
        info.caption = ref->base->name + MWGui::ToolTips::getCountString(ptr.getRefData().getCount());
        info.icon = ref->base->icon;

        const ESMS::ESMStore& store = MWBase::Environment::get().getWorld()->getStore();

        std::string text;

        text += "\n" + store.gameSettings.search("sWeight")->str + ": " + MWGui::ToolTips::toString(ref->base->data.weight);
        text += MWGui::ToolTips::getValueString(ref->base->data.value, store.gameSettings.search("sValue")->str);

        info.effects = MWGui::Widgets::MWEffectList::effectListFromESM(&ref->base->effects);

        if (MWBase::Environment::get().getWindowManager()->getFullHelp()) {
            text += MWGui::ToolTips::getMiscString(ref->ref.owner, "Owner");
            text += MWGui::ToolTips::getMiscString(ref->base->script, "Script");
        }

        info.text = text;

        return info;
    }
}
