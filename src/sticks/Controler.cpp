#include "Controler.hpp"
#include <imgui.h>

Controler::Controler(Model& model, Renderer& renderer, Application& application) :
    m_model(model), m_renderer(renderer), m_application(application) {}

void Controler::render() {
    if (ImGui::Begin("Config")) {
        if (m_model.m_updating) {
            if (ImGui::Button("Pause")) {
                pause();
            }
        } else {
            if (ImGui::Button("Play")) {
                pause();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Restart")) {
            m_model.start();
        }

        ImGui::SameLine();
        bool showSticks = m_renderer.getShowSticks();
        if (ImGui::Checkbox("stick", &showSticks)) {
            m_renderer.setShowSticks(showSticks);
        }

        ImGui::SameLine();
        bool vsync = m_application.getVSync();
        if (ImGui::Checkbox("vsync", &vsync)) {
            m_application.setVSync(vsync);
        }

        ImGui::SliderFloat("speed", &m_model.m_speed, 0.0f, 3.0f);

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < m_model.m_sticks.size(); ++i) {
                auto& stick = m_model.m_sticks[i];
                std::string index = std::to_string(i + 1);
                if (ImGui::TreeNode(("stick #" + index).c_str())) {
                    if (ImGui::SliderInt(("length##" + index).c_str(), &stick.m_length, 10, 200))
                        m_model.start();
                    if (ImGui::DragFloat(("velocity##" + index).c_str(), &stick.m_angularVelocity, 0.1f, -10.0f, 10.0f))
                        m_model.start();
                    auto f4color = ImGui::ColorConvertU32ToFloat4(*(ImU32*)(&stick.m_color.r));
                    if (ImGui::ColorEdit4(("color##" + index).c_str(), (float*)(&f4color))) {
                        *(ImU32*)(&stick.m_color.r) = ImGui::ColorConvertFloat4ToU32(f4color);
                    }
                    ImGui::Checkbox(("draw##" + index).c_str(), &stick.m_draw);
                    ImGui::TreePop();
                }
            }
        }
    }
    ImGui::End();
}

void Controler::pause() {
    m_model.pause();
}
