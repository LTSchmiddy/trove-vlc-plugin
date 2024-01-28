#include <sqlite3.h>

#include "MovieView.h"

namespace UI::Views {
    MovieView::MovieView(){
        reloadMovieList();
    }
    MovieView::~MovieView(){}

    void MovieView::reloadMovieList() {
        sqlite3_stmt* stmt = Global::library_db->returnSimpleSqlString("SELECT source, path, title, date, desc, poster_path FROM movie;");

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string source = (char*)sqlite3_column_text(stmt, 0);
            std::string path = (char*)sqlite3_column_text(stmt, 1);
            std::string title = (char*)sqlite3_column_text(stmt, 2);
            std::string date = (char*)sqlite3_column_text(stmt, 3);
            std::string desc = (char*)sqlite3_column_text(stmt, 4);
            std::string poster_path = (char*)sqlite3_column_text(stmt, 5);

            movie_list.emplace_back(source, path, title, date, desc, poster_path);
        }

        sqlite3_finalize(stmt);
    }

    void MovieView::onBackground() {}
    bool MovieView::onEvent(SDL_Event* event) {
        return false;
    }

    void MovieView::onDraw() {
        ImGui::PushID("movie_view");
        drawListLayout();
        ImGui::PopID();
    }
    
    void MovieView::drawListLayout() {
        ImGui::PushID("#movie_list_layout");
        for (auto movie : movie_list) {
            ImGui::PushID(std::format("source_{}_path_{}", movie.source, movie.path).c_str());
            float frame_width = ImGui::GetWindowWidth();
            ImGui::BeginChild("Frame##child_frame", {frame_width, entry_size});
            if (movie.poster_img == NULL) {
                movie.loadPosterImg();
            }

            // Poster ratio = 2:3
            ImGui::Image(movie.poster_img->get(), {(entry_size/3.0f)*2.0f, entry_size});
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::TextWrapped("%s", movie.title.c_str());
            ImGui::TextWrapped("%s", movie.desc.c_str());

            ImGui::EndGroup();
            ImGui::EndChild();
            ImGui::PopID();
        }
        ImGui::PopID();
    }
}