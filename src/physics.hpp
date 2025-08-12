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

#ifndef SUPERTUX_SRC_PHYSICS_HPP
#define SUPERTUX_SRC_PHYSICS_HPP

#include "game.hpp"
#include "math/vector.hpp"
class Physics
{
public:
	Physics() :
		ax(), ay(),
		vx(), vy(),
		gravity_enabled_flag(true),
		gravity_modifier(1.0)
	{}
	~Physics() = default;
	
	//void reset();
	void set_x_vel(double _vx) { vx = _vx; }
	void set_y_vel(double _vy) { vy = _vy; }
	
	void set_x_accel(double _ax) { ax = _ax; }
	void set_y_accel(double _ay) { ay = _ay; }
	
	double get_x_vel() const { return vx; }
	double get_y_vel() const { return vy; }
	
	double get_x_accel() const { return ax; }
	double get_y_accel() const { return ay; }
	
	void enable_gravity(bool enable) { gravity_enabled_flag = enable; }
	bool gravity_enabled() const { return gravity_enabled_flag; }
	
	void set_gravity_modifier(double mod) { gravity_modifier = mod; }
	double get_gravity_modifier() const { return gravity_modifier; }
	
	Vec2 get_movement()
	{
		vx += ax * g_dtime;
		vy += ay * g_dtime;
		return { Vec2::type(vx * g_dtime), Vec2::type(vy * g_dtime) };
	}
private:
	double ax, ay;
	double vx, vy;
	
	bool gravity_enabled_flag;
	double gravity_modifier;
};

#endif
