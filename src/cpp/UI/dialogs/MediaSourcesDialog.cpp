#include <format>

#include <plog/Log.h>
#include <tinyfiledialogs.h>

#include "ns_abbr/fs.h"
#include "MediaSourcesDialog.h"
#include "ui/ui_globals.h"
#include "settings/settings_globals.h"
#include "media_source/source_globals.h"


namespace UI {

    void MediaSourcesDialog::onDraw() {
        drawMenuBar();
        drawLocations();

    }

    void MediaSourcesDialog::drawMenuBar() {
        if(ImGui::BeginMenuBar()){
            if(ImGui::BeginMenu("Add##media_sources_add_menu")) {
                ImGui::PushID("media_sources_add_menu_choices");
                if (ImGui::MenuItem("File Location")){
                    Global::media_sources.emplace("New File Source", std::make_shared<MediaSource::SourceType::FileSource>());
                }
                ImGui::PopID();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    } 

    void MediaSourcesDialog::drawLocations() {
        // Since we can't rename any media_sources while iterating, we'll need to track any renames and perform them after.
        // In theory, there should never be more than one rename request at a time, but I suppose it doesn't hurt to be careful.
        std::map<std::string, std::string> renames;
        for ( auto &it : Global::media_sources ) {
            // Create an ID for the media_source options:
            std::string id_str = std::format("media_source_display_{}", it.first);
            ImGui::PushID(id_str.c_str());

            ImGui::Text("Source Name:");
            ImGui::SameLine();

            std::string src_rename = it.first;         
            if(ImGui::InputText("##entry_name", &src_rename, ImGuiInputTextFlags_EnterReturnsTrue)) {
                renames.emplace(it.first, src_rename);
            }

            ImGui::Indent();
            
            ImGui::Text("Content Type:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##content_type:", contentTypeLabels[(int)it.second->contentType].c_str())) {
                if (ImGui::Selectable(contentTypeLabels[(int)MediaSource::SourceType::CONTENT_TYPE::MOVIES].c_str(), it.second->contentType == MediaSource::SourceType::CONTENT_TYPE::MOVIES)) {
                    it.second->contentType = MediaSource::SourceType::CONTENT_TYPE::MOVIES;
                }
                if (ImGui::Selectable(contentTypeLabels[(int)MediaSource::SourceType::CONTENT_TYPE::TV_SHOWS].c_str(), it.second->contentType == MediaSource::SourceType::CONTENT_TYPE::TV_SHOWS)) {
                    it.second->contentType = MediaSource::SourceType::CONTENT_TYPE::TV_SHOWS;
                }
                if (ImGui::Selectable(contentTypeLabels[(int)MediaSource::SourceType::CONTENT_TYPE::OTHER].c_str(), it.second->contentType == MediaSource::SourceType::CONTENT_TYPE::OTHER)) {
                    it.second->contentType = MediaSource::SourceType::CONTENT_TYPE::OTHER;
                }
                ImGui::EndCombo();
            }


            // Now to draw the config options for the source type:
            if (it.second->getType() == "file") {
                // Well, that's kind of a convoluted way to do this, but it SHOULD work:
                drawFileLocation((MediaSource::SourceType::FileSource*)Global::media_sources.at(it.first).get());
            }

            if (ImGui::Button("Scan")) {
                it.second->startScan();
            }

            ImGui::Unindent();
            ImGui::PopID();

            // Adding some spacing:
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
        }

        // Now, actually do the rename tasks.
        for ( const auto &it : renames ) {
            if (!Global::rename_media_source(it.first, it.second)) {
                PLOGE.printf("Can't rename media source '%s' to '%s'. New name already exists.", it.first.c_str(), it.second.c_str());
            }
        }
    } 

    void MediaSourcesDialog::drawFileLocation(MediaSource::SourceType::FileSource* source) {
        ImGui::Text("Source Type: File");
        ImGui::Text("Root Path:");
        ImGui::SameLine();
        ImGui::InputText("##rootPath", &source->rootPath);
        ImGui::SameLine();
        if (ImGui::Button("Browse")) {
            const char* selection = tinyfd_selectFolderDialog("Select Root Path", "");

            if (selection != NULL) {
                source->rootPath = selection;
            }
        }

        if (ImGui::BeginCombo("File Parser##selectable_file_parser:", source->parserScriptPath.c_str())) {
            for (const fs::directory_entry &entry : fs::directory_iterator(Global::settings.scripts.movie_parsers_path)) {
                if (ImGui::Selectable(entry.path().string().c_str(), entry.path().string() == source->parserScriptPath)) {
                    source->parserScriptPath = entry.path().string();
                }
            }
            ImGui::EndCombo();
        }

    }

    void create_media_sources_dialog() {
        Global::sub_window_handler->addSubWindow(
            "media_souces",
            std::make_shared<FWindow>("Media Sources##media_sources", std::make_shared<MediaSourcesDialog>(), true, ImGuiWindowFlags_MenuBar)
        );
    }
}