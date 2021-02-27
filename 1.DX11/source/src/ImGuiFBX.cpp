//===========================================================================
//! @file   ImGuiFBX.cpp
//!	@brief  ImGuiFBX
//===========================================================================
#include "ImGuiFBX.h"
#include <inttypes.h>

namespace gpu {

const ofbx::IElement* gSelectedElement_ = nullptr;
const ofbx::Object*   gSelectedObject_  = nullptr;

template<typename T>
void showArray(const char* label, const char* format, ofbx::IElementProperty& prop)
{
    if(!ImGui::CollapsingHeader(label))
        return;

    int count = prop.getCount();
    ImGui::Text("Count: %d", count);
    std::vector<T> tmp;
    tmp.resize(count);
    prop.getValues(&tmp[0], int(sizeof(tmp[0]) * tmp.size()));
    for(T v : tmp) {
        ImGui::Text(format, v);
    }
}

template<int N>
void toString(ofbx::DataView view, char (&out)[N])
{
    int len = int(view.end - view.begin);
    if(len > sizeof(out) - 1)
        len = sizeof(out) - 1;
    strncpy(out, (const char*)view.begin, len);
    out[len] = 0;
}

template<int N>
void catProperty(char (&out)[N], const ofbx::IElementProperty& prop)
{
    char tmp[128];
    switch(prop.getType()) {
        case ofbx::IElementProperty::DOUBLE: sprintf_s(tmp, "%f", prop.getValue().toDouble()); break;
        case ofbx::IElementProperty::LONG: sprintf_s(tmp, "%" PRId64, prop.getValue().toU64()); break;
        case ofbx::IElementProperty::INTEGER: sprintf_s(tmp, "%d", prop.getValue().toInt()); break;
        case ofbx::IElementProperty::STRING: prop.getValue().toString(tmp); break;
        default: sprintf_s(tmp, "Type: %c", (char)prop.getType()); break;
    }
    strcat_s(out, tmp);
}

static void showCurveGUI(const ofbx::Object& object)
{
    const ofbx::AnimationCurve& curve = static_cast<const ofbx::AnimationCurve&>(object);

    const int c = curve.getKeyCount();
    for(int i = 0; i < c; ++i) {
        const float t = (float)ofbx::fbxTimeToSeconds(curve.getKeyTime()[i]);
        const float v = curve.getKeyValue()[i];
        ImGui::Text("%fs: %f ", t, v);
    }
}

void showObjectGUI(const ofbx::Object& object)
{
    const char* label;
    switch(object.getType()) {
        case ofbx::Object::Type::GEOMETRY: label = "geometry"; break;
        case ofbx::Object::Type::MESH: label = "mesh"; break;
        case ofbx::Object::Type::MATERIAL: label = "material"; break;
        case ofbx::Object::Type::ROOT: label = "root"; break;
        case ofbx::Object::Type::TEXTURE: label = "texture"; break;
        case ofbx::Object::Type::NULL_NODE: label = "null"; break;
        case ofbx::Object::Type::LIMB_NODE: label = "limb node"; break;
        case ofbx::Object::Type::NODE_ATTRIBUTE: label = "node attribute"; break;
        case ofbx::Object::Type::CLUSTER: label = "cluster"; break;
        case ofbx::Object::Type::SKIN: label = "skin"; break;
        case ofbx::Object::Type::ANIMATION_STACK: label = "animation stack"; break;
        case ofbx::Object::Type::ANIMATION_LAYER: label = "animation layer"; break;
        case ofbx::Object::Type::ANIMATION_CURVE: label = "animation curve"; break;
        case ofbx::Object::Type::ANIMATION_CURVE_NODE: label = "animation curve node"; break;
        default: assert(false); break;
    }

    ImGuiTreeNodeFlags flags = gSelectedObject_ == &object ? ImGuiTreeNodeFlags_Selected : 0;
    char               tmp[128];
    sprintf_s(tmp, "%" PRId64 " %s (%s)", object.id, object.name, label);
    if(ImGui::TreeNodeEx(tmp, flags)) {
        if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            gSelectedObject_ = &object;
        int i = 0;
        while(ofbx::Object* child = object.resolveObjectLink(i)) {
            showObjectGUI(*child);
            ++i;
        }
        if(object.getType() == ofbx::Object::Type::ANIMATION_CURVE) {
            showCurveGUI(object);
        }

        ImGui::TreePop();
    }
    else {
        if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            gSelectedObject_ = &object;
    }
}

void showGUI(ofbx::IElementProperty& prop)
{
    ImGui::PushID((void*)&prop);
    char tmp[256];
    switch(prop.getType()) {
        case ofbx::IElementProperty::LONG: ImGui::Text("Long: %" PRId64, prop.getValue().toU64()); break;
        case ofbx::IElementProperty::FLOAT: ImGui::Text("Float: %f", prop.getValue().toFloat()); break;
        case ofbx::IElementProperty::DOUBLE: ImGui::Text("Double: %f", prop.getValue().toDouble()); break;
        case ofbx::IElementProperty::INTEGER: ImGui::Text("Integer: %d", prop.getValue().toInt()); break;
        case ofbx::IElementProperty::ARRAY_FLOAT: showArray<float>("float array", "%f", prop); break;
        case ofbx::IElementProperty::ARRAY_DOUBLE: showArray<double>("double array", "%f", prop); break;
        case ofbx::IElementProperty::ARRAY_INT: showArray<int>("int array", "%d", prop); break;
        case ofbx::IElementProperty::ARRAY_LONG: showArray<ofbx::u64>("long array", "%" PRId64, prop); break;
        case ofbx::IElementProperty::STRING:
            toString(prop.getValue(), tmp);
            ImGui::Text("String: %s", tmp);
            break;
        default:
            ImGui::Text("Other: %c", (char)prop.getType());
            break;
    }

    ImGui::PopID();
    if(prop.getNext())
        showGUI(*prop.getNext());
}



void showGUI(const ofbx::IElement& parent)
{
    for(const ofbx::IElement* element = parent.getFirstChild(); element; element = element->getSibling()) {
        auto id = element->getID();
        char label[128];
        id.toString(label);
        strcat_s(label, " (");
        ofbx::IElementProperty* prop  = element->getFirstProperty();
        bool                    first = true;
        while(prop) {
            if(!first)
                strcat_s(label, ", ");
            first = false;
            catProperty(label, *prop);
            prop = prop->getNext();
        }
        strcat_s(label, ")");

        ImGui::PushID((const void*)id.begin);
        ImGuiTreeNodeFlags flags = gSelectedElement_ == element ? ImGuiTreeNodeFlags_Selected : 0;
        if(!element->getFirstChild())
            flags |= ImGuiTreeNodeFlags_Leaf;
        if(ImGui::TreeNodeEx(label, flags)) {
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
                gSelectedElement_ = element;
            if(element->getFirstChild())
                showGUI(*element);
            ImGui::TreePop();
        }
        else {
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
                gSelectedElement_ = element;
        }
        ImGui::PopID();
    }
}

void showObjectsGUI(const ofbx::IScene& scene, char fileName_[MAX_PATH])
{
    if(ImGui::BeginTabBar(fileName_)) {
        if(ImGui::BeginTabItem(fileName_)) {
            const ofbx::Object* root = scene.getRoot();
            if(root)
                showObjectGUI(*root);

            int count = scene.getAnimationStackCount();
            for(int i = 0; i < count; ++i) {
                const ofbx::Object* stack = scene.getAnimationStack(i);
                showObjectGUI(*stack);
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void imguiRender(char fileName_[MAX_PATH], ofbx::IScene* scene_)
{
    if(ImGui::BeginTabItem(fileName_)) {
        const ofbx::IElement* root = scene_->getRootElement();
        if(root && root->getFirstChild())gpu::showGUI(*root);
        gpu::showObjectsGUI(*scene_, fileName_);
        ImGui::EndTabItem();
    }

    if(gSelectedElement_) {
        ofbx::IElementProperty* prop = gSelectedElement_->getFirstProperty();
        if(prop)
            gpu::showGUI(*prop);
    }

}

}   // namespace gpu