#ifndef battle_energy_hpp
#define battle_energy_hpp
class battle_energy
{
protected:
	float _current_energy;
	float _grow_per_frame;
	int _last_update_frame;
	int _fast_start_frame;
	float _fast_ratio;
	float _max_energy;
public:
	void reset(float max_energy, float current, float grow_per_frame, int fast_start_frame, float fast_ratio)
	{
		_current_energy = current;
		_last_update_frame = 0;
		_max_energy = max_energy;
		_grow_per_frame = grow_per_frame;
		_fast_start_frame = fast_start_frame;
		_fast_ratio = fast_ratio;
	}
	inline float get_current_energy()
	{
		return _current_energy;
	}
	inline bool cast_energy(float energy)
	{
		if (energy > _current_energy)
			return false;
		_current_energy -= energy;
		return true;
	}
	inline void set_energy(float energy)
	{
		_current_energy = energy;
	}
	inline void update(int frame)
	{
		if (frame <= _last_update_frame)
			return;
		float tmp = 0;
		if (frame >= _fast_start_frame)
			tmp = _current_energy + _grow_per_frame*(frame - _last_update_frame)*_fast_ratio;
		else
			tmp = _current_energy + _grow_per_frame*(frame - _last_update_frame);

		_last_update_frame = frame;

		if (tmp > _max_energy)
			_current_energy = _max_energy;
		else
			_current_energy = tmp;
	}
	inline void grow_per_frame(float v)
	{
		_grow_per_frame = v;
	}
	inline float grow_per_frame()
	{
		return _grow_per_frame;
	}
	inline float max_energy()
	{
		return _max_energy;
	}
	inline void max_energy(float v)
	{
		_max_energy = v;
	}
	inline int fast_start_frame()
	{
		return _fast_start_frame;
	}
	inline void fast_start_frame(int v)
	{
		_fast_start_frame = v;
	}

};
#endif // !battle_energy_hpp
