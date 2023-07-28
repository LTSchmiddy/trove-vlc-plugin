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
        for (auto movie : movie_list) {
            if (movie.poster_img == NULL) {
                movie.loadPosterImg();
            }

            ImGui::Text("%s", movie.title.c_str());
            ImGui::Image(movie.poster_img->get(), {50.0f, 50.0f});
        }
    }
}