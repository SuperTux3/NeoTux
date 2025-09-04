//  SuperTux 
//  Copyright (C) 2025 Hyland B. <me@ow.swag.toys> 
// 
//  This program is free software: you can redistribute it and/or modify 
//  it under the terms of the GNU General Public License as published by 
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version. 
// 
//  This program is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
//  GNU General Public License for more details. 
// 
//  You should have received a copy of the GNU General Public License 
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "gui_test.hpp"
#include "gui/gui_reader.hpp"
#include "input_manager.hpp"
#include "math/size.hpp"
#include "camera.hpp"
#include "video/font_manager.hpp"
#include "video/video_system.hpp"

void
GuiTest::run()
{
	FontManager::load_builtin_fonts();
	Widget::register_all_widgets();
	Size winsize = g_video_system->get_window_size();
	g_rtcontext.width = winsize.width;
	g_rtcontext.height = winsize.height;
	
	g_mixer.play_music("music/antarctic/chipdisko.ogg");
	
	Painter* painter = g_video_system->get_painter();
	painter->register_camera(&g_rtcontext);
	
	GuiReader gui_reader;
	Widget *box = gui_reader.open("guis/gui_test.stui");
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		box->update();
		box->draw();
	END_GAME_LOOP
	
	delete box;
}
