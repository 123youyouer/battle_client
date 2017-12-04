#ifndef EXPORT_H
#define EXPORT_H
extern "C" _declspec(dllexport) int		__stdcall frame_bgn(int frame_index, int millisecond);
extern "C" _declspec(dllexport) int		__stdcall frame_upd(int frame_index, int millisecond);
extern "C" _declspec(dllexport) int		__stdcall frame_end(int frame_index, int millisecond);
extern "C" _declspec(dllexport) int		__stdcall init_battle(int map, int w, int h, int millisecond_per_frame);
extern "C" _declspec(dllexport) char*	__stdcall create_object(int id, int type, int x, int y, char* base_data);
extern "C" _declspec(dllexport) void	__stdcall get_object_data(char* handle, char* data);
extern "C" _declspec(dllexport) void	__stdcall remove_object(char* handle);
extern "C" _declspec(dllexport) bool	__stdcall death_object(char* handle);
extern "C" _declspec(dllexport) void	__stdcall demage(char* tar, int effect_flag, int value);

extern "C" _declspec(dllexport) void	__stdcall aoe(int camps, int attack_flags, int effect_flags, int x, int y, int radius, int value);
extern "C" _declspec(dllexport) void	__stdcall aoe2(int camps, int attack_flags, int effect_flags, int x, int y, int radius, int value, int scale_to_zhujidi);
extern "C" _declspec(dllexport) void	__stdcall set_callback(void* func);
extern "C" _declspec(dllexport) float	__stdcall get_current_energy(int camps);
extern "C" _declspec(dllexport) int		__stdcall get_stat_flags(char* handle);
extern "C" _declspec(dllexport) bool	__stdcall cast_energy(int camps,float v);
extern "C" _declspec(dllexport) void	__stdcall add_path(int* data, int len);
extern "C" _declspec(dllexport) void	__stdcall add_obstacle(int* data, int len);
extern "C" _declspec(dllexport) void	__stdcall set_energy_data(int camps, int max, int first, float grow_perframe, int fast_start_frame, float fast_ratio);
extern "C" _declspec(dllexport) void	__stdcall use_trajectory(char* tar, bool use);
extern "C" _declspec(dllexport) bool	__stdcall put_down_check(int x, int y, int radius);
extern "C" _declspec(dllexport) void	__stdcall set_energy(int camps, int value);
extern "C" _declspec(dllexport) void	__stdcall set_building_shild(char* handle, bool value);
extern "C" _declspec(dllexport) int		__stdcall get_cell(int x, int y);

extern "C" _declspec(dllexport) int		__stdcall get_object_HP(char* handle);
extern "C" _declspec(dllexport) void	__stdcall set_object_BeAttackEffect(char* handle, int effect);
extern "C" _declspec(dllexport) void	__stdcall set_object_curHP(char* handle,int value);
extern "C" _declspec(dllexport) void	__stdcall set_object_location(char* handle, int x, int y);
extern "C" _declspec(dllexport) void	__stdcall set_object_movment(char* handle, int speed, int face, int aiming_face);
extern "C" _declspec(dllexport) void	__stdcall set_pvp_flag(int flag);
extern "C" _declspec(dllexport) bool	__stdcall aoe_check(char* handle, int attack_flags, int x, int y, int radius);
extern "C" _declspec(dllexport) void	__stdcall set_object_status(char* handle, int flags);
#endif